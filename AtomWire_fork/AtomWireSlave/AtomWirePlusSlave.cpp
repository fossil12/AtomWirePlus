#include "AtomWirePlusSlave.h"

AtomWirePlusSlave::AtomWirePlusSlave(uint8_t pin) : OneWireSlave(pin)
{
  new_in_msg = false;
  new_out_msg = false;
}

// Private methods

void AtomWirePlusSlave::run_general_functions(uint16_t miliseconds)
{

}

// Adapted from `OneWireSlave::recvAndProcessCmd()`
bool AtomWirePlusSlave::recvAndProcessCmd()
{
  char addr[8];
  uint8_t oldSREG = 0;
  uint8_t matched;

  for (;;) {
    uint8_t cmd = recv();
    switch (cmd) {
      case 0xF0: // SEARCH ROM
        search();
        delayMicroseconds(6900);
        return FALSE;
      case 0x33: // READ ROM
        sendData(rom, 8);
        if (errno != ONEWIRE_NO_ERROR) {
          return FALSE;
        }
        break;
      case 0x55: // MATCH ROM - Choose/Select ROM
        matched = 1;
        recvData(addr, 8);
        if (errno != ONEWIRE_NO_ERROR) {
          return FALSE;
        }
        for (int i=0; i<8; i++){
          if (rom[i] != addr[i]) {
            matched = 0;
          }
          resumeaddr[i] = addr[i];
        }

        if (matched) {
          duty();
        } else {
          // we have now up to 14ms until the next reset pulse
          run_general_functions(14);
        }
      case 0xCC: // SKIP ROM
        // XXX: Not implemented
        return TRUE;
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
        } else {
          // we have now up to 14ms until the next reset pulse
          run_general_functions(14);
        }
      default: // Unknow command
        if (errno == ONEWIRE_NO_ERROR)
          break; // skip if no error
        else
          return FALSE;
    }
  }
}

bool AtomWirePlusSlave::duty(void)
{
  uint8_t index;
  char frame[AWP_FRAME_BYTE_LENGTH];

  errno = ONEWIRE_NO_ERROR;

  // Read command
  frame[0] = recv();

  // Depending on command receive message and store it
  if ((frame[0] & 0x10) != 0x70) {
    errno = AWP_ERR_WRONG_COMMAND;
    return false;
  }

  // A message will be sent to us
  if ((frame[0] & 0x01) != 0) {
    this->recvData(&frame[1], AWP_FRAME_BYTE_LENGTH - 1);

    // Check CRC
    if (frame[AWP_FRAME_BYTE_LENGTH - 1] == crc8(frame, AWP_FRAME_BYTE_LENGTH - 1)) {
      // Check if we have an existing new message that will be overridden
      if (new_in_msg) {
        errno = AWP_ERR_IN_MSG_OVERRIDDEN;
      }

      new_in_msg = true;
      for (index = 0; index < AWP_FRAME_BYTE_LENGTH; index++) {
        in_msg[index] = frame[index];
      }
    } else { // Not correct CRC
      errno = AWP_ERR_WRONG_CRC;
    }
  }

  // Wait for sending command to be sent
  uint8_t cmd = recv();

  // Check if we received the correct command
  if (cmd == 0x90) {
    // Send out message if there is any
    if (new_out_msg) {
      this->sendData(out_msg, AWP_FRAME_BYTE_LENGTH);
      new_out_msg = false;
    } else {
      this->send(0x90);
    }
  } else {
    errno = AWP_ERR_WRONG_SEND_CMD;
    return false;
  }

  return true;
}
