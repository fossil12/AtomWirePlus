#include "AtomWirePlusSlave.h"

#define CHECK_TIMEOUT_MS(timeout) do { if (timeout <= millis()) { return; } } while(0)

AtomWirePlusSlave::AtomWirePlusSlave(uint8_t pin) : OneWireSlave(pin)
{
  new_in_frame = false;
  new_out_frame = false;
  new_in_msg = false;
  new_out_msg = false;
  gpio_pin_state = 0x00;
}

// Protected methods
void AtomWirePlusSlave::increment_last_value()
{
  if (new_in_msg) {
    new_in_msg = false;

    for (int index = 0; index < 7; index++) {
      out_msg[index] = in_msg[index];
    }

    out_msg[7] = ++in_msg[7];

    if (new_out_msg) {
      errno = AWP_ERR_OUT_MSG_OVERRIDDEN;
    }
    new_out_msg = true;
  }
}

void AtomWirePlusSlave::check_all_gpio_pins()
{
  // Part of the code similar to `OneWireSlave::gpioRead()`
  uint8_t gpioRvalue = 0x00;
  uint8_t bitShift = 0x01;
    
  cli();
  for(int i=0; i<4; i++){
    DIRECT_MODE_INPUT(portInputRegister(digitalPinToPort(i)), digitalPinToBitMask(i));
    if (DIRECT_READ(portInputRegister(digitalPinToPort(i)), digitalPinToBitMask(i))) {
      gpioRvalue |= bitShift;
    }
    bitShift <<= 1;          
  }
  sei();

  // check if one of the value changed
  if (gpio_pin_state != gpioRvalue) {
    gpio_pin_state = gpioRvalue;

    // Create message
    // Slave uses normally ids from 0x70 upwards
    out_msg[0] = 0x71;
    out_msg[1] = gpioRvalue;

    if (new_out_msg) {
      errno = AWP_ERR_OUT_MSG_OVERRIDDEN;
    }

    new_out_msg = true;
  }
}

void AtomWirePlusSlave::parse_in_frame()
{
  if (new_in_frame) {
    new_in_frame = false;

    // TODO: allow fragmentation

    for (int index = 0; index < 8; index++) {
      in_msg[index] = in_frame[index + 4];
    }

    if (new_in_msg) {
      errno = AWP_ERR_IN_MSG_OVERRIDDEN;
    }
    new_in_msg = true;
  }
}

void AtomWirePlusSlave::create_out_frame()
{
  if (new_out_msg) {
    new_out_msg = false;

    // Send follower bits are 0x9
    // We currently only support unfragmented messages
    out_frame[0] = 0x98;

    // Size/offset
    out_frame[1] = 0x01; // 8 bytes;
    out_frame[2] = 0x00; // end of size/offset

    // payload of message
    for (int index = 0; index < 8; index++) {
      out_frame[index + 4] = out_msg[index];
    }

    out_frame[12] = this->crc8(out_frame, AWP_FRAME_BYTE_LENGTH - 1);

    if (new_out_frame) {
      errno = AWP_ERR_OUT_FRAME_OVERRIDDEN;
    }
    new_out_frame = true;
  }
}

void AtomWirePlusSlave::run_general_functions(uint16_t miliseconds)
{
  unsigned long timeout;

  timeout = millis() + miliseconds - 1; // stay safe by removing 1 ms

  // Loop through possible task (one task should last for less than 1 ms)
  // Check after every task if the time is expired
  while (millis() < timeout) {
    // TODO: use a switch and global variable to do real round robin w/ the
    // methods called here
    parse_in_frame();
    CHECK_TIMEOUT_MS(timeout);

    // do some other work
    //increment_last_value();
    check_all_gpio_pins();

    CHECK_TIMEOUT_MS(timeout);

    create_out_frame();
  }
}

// Adapted from `OneWireSlave::recvAndProcessCmd()`
bool AtomWirePlusSlave::recvAndProcessCmd()
{
  char addr[8];
  uint8_t oldSREG = 0;
  uint8_t matched;
  bool result = true;

  uint8_t cmd = recv();
  switch (cmd) {
    case 0xF0: // SEARCH ROM
      search();
      delayMicroseconds(6900);
      result = false;
      break;
    case 0x33: // READ ROM
      sendData(rom, 8);
      if (errno != ONEWIRE_NO_ERROR) {
        result = false;
      }
      break;
    case 0x55: // MATCH ROM - Choose/Select ROM
      matched = 1;
      recvData(addr, 8);
      if (errno != ONEWIRE_NO_ERROR) {
        result = false;
        break;
      }
      for (int i=0; i<8; i++){
        if (rom[i] != addr[i]) {
          matched = 0;
        }
        resumeaddr[i] = addr[i];
      }

      if (matched) {
        duty();
        // we have now 8ms unitl the next reset pulse
        run_general_functions(8);
      } else {
        // we have now up to 14ms + 8ms until the next reset pulse
        run_general_functions(22);
      }
    case 0xCC: // SKIP ROM
      // XXX: Not implemented
      break;
    case 0x69: // RESUME ROM
      matched = 1;
      for (int i = 0; i < 8; i++) {
        if (resumeaddr[i] != rom[i]) {
          matched = 0;
          break;
        }
      }
      if (matched) {
        duty();
        // we have now 8ms unitl the next reset pulse
        run_general_functions(8);
      } else {
        // we have now up to 14ms + 8ms until the next reset pulse
        run_general_functions(22);
      }
    default: // Unknow command
      if (errno == ONEWIRE_NO_ERROR)
        break; // skip if no error
      else
        result = false;
        break;
  }

  return result;
}

bool AtomWirePlusSlave::duty(void)
{
  uint8_t index;
  char frame[AWP_FRAME_BYTE_LENGTH];
  bool result = true;

  errno = ONEWIRE_NO_ERROR;

  // Read command
  frame[0] = recv();

  // Depending on command receive message and store it
  if ((frame[0] & 0xF0) != 0x70) {
    errno = AWP_ERR_WRONG_COMMAND;
    return false;
  }

  // A message will be sent to us
  if ((frame[0] & 0x0F) != 0) {
    this->recvData(&frame[1], AWP_FRAME_BYTE_LENGTH - 1);
  }

  // Wait for sending command to be sent
  uint8_t cmd = recv();

  // Check if we received the correct command
  if (cmd == 0x90) {
    // Send out message if there is any
    if (new_out_frame) {
      this->sendData(out_frame, AWP_FRAME_BYTE_LENGTH);
      new_out_frame = false;
    } else {
      this->send(0x90);
    }
  } else {
    errno = AWP_ERR_WRONG_SEND_CMD;
    result = false;
  }

  // Copy data if we received data
  // After receving and sending otherwise we have a timing issue
  if ((frame[0] & 0x0F) != 0) {
    // Check CRC
    // With 1 node we always have 2 packets with correct CRC and then
    // 2 packets with incorrect CRC
    //if (frame[AWP_FRAME_BYTE_LENGTH - 1] == crc8(frame, AWP_FRAME_BYTE_LENGTH - 1)) {
      // Check if we have an existing new message that will be overridden
      if (new_in_frame) {
        errno = AWP_ERR_IN_FRAME_OVERRIDDEN;
      }

      new_in_frame = true;
      for (index = 0; index < AWP_FRAME_BYTE_LENGTH; index++) {
        in_frame[index] = frame[index];
      }
    // } else { // Not correct CRC
    //   errno = AWP_ERR_WRONG_CRC;
    // }
  }


  return result;
}
