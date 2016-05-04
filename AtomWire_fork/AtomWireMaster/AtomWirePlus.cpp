#include "AtomWirePlus.h"

AtomWirePlus::AtomWirePlus(uint8_t pin) : OneWire(pin) {
  // Does that work like that?
  num_nodes = 0;
  round = 0;
}

// Assumes msg is 64 bits (8 bytes) long
uint8_t AtomWirePlus::send_msg_p(uint8_t msg[64])
{
  uint8_t frame[FRAME_BYTE_LENGTH];

  // Send bits are 0x7
  uint8_t cmd_frag = 0x70;

  // Check if message is empty
  if (msg == NULL) {
    this->write(cmd_frag); // send command indicating empty message
    return TRUE;
  }

  // We currently only support unfragmented messages
  cmd_frag &= 0x8;

  frame[0] = cmd_frag;

  // Size/offset
  frame[1] = 0x01; // 8 bytes;
  frame[2] = 0x00; // end of size/offset

  // frag_id: for now the round variable
  frame[2] |= (round & 0x10) >> 4;
  frame[3] = 0x00;
  frame[3] |= (round & 0x01) << 4;

  // payload of message
  for (int i = 0; i < 8; i++) {
    frame[i + 4] = msg[i];
  }

  frame[12] = this->crc8(msg, 8);

  return TRUE;
}

uint8_t AtomWirePlus::recv_msg_p(uint8_t msg[64])
{
  uint8_t frame[FRAME_BYTE_LENGTH];

  // recev bits 0x90
  this->write(0x90); // Receive request sent to sender
  this->read_bytes(frame, 1);

  // Check if there is anything to receive
  if (frame[0] == 0x90) {
    return TRUE;
  }

  // Get rest of frame
  this->read_bytes(&frame[1], FRAME_BYTE_LENGTH - 1);

  // Check if it's a send request
  if ((frame[0] & 0x10) != 0x90) {
    return FALSE;
  }

  // TODO: Check for fragmentation and start/continue reassembly if true

  // Copy received message into message buffer
  for (int i = 0; i < 8; i++) {
    msg[i] = frame[i + 4];
  }

  return TRUE;
}

uint8_t AtomWirePlus::run(void)
{
  if ((round % REDUE_SEARCH_INTERVAL) == 0) {
    full_search();
  }

  round++;

  // Nothing to do if there are no nodes on the line
  if (num_nodes == 0) {
    return TRUE;
  } else if (current_node >= num_nodes) {   // If nodes were removed start at node 0 again
    current_node = 0;
  } else {
    current_node = ++current_node % num_nodes;
  }

  // reset/presence pulse
  if (!this->reset()) {
    return FALSE;
  }

  // select address
  select(addrs[current_node]);

  // Time to send message if there is any
  // TODO: Do fragmenting before call of send_msg_p and pass important parameters
  if (!send_msg_p(out_msg_queue[current_node])) {
    return FALSE;
  }

  // Time to receive message
  // TODO: Check for fragmenting and start assembling incoming message
  if (!recv_msg_p(in_msg_queue[current_node])) {
    return FALSE;
  }

  return TRUE;
}

// uint8_t AtomWirePlus::get_node(uint8_t pos, uint8_t *addr)
// {
//   // bool serachExpired = 0;

//   // if (serachExpired) {
//   //   full_search();
//   // }

//   return this->search(addr);
// }

// Returns postion of `addr` in `addrs` array or -1 when not found
int8_t AtomWirePlus::get_pos_of_node(uint8_t addr[64])
{
  int i, j;
  uint8_t addr_ref[64];

  // Because the array is (on purpose) not sorted there is no faster way than O(n).
  for (i = 0; i < num_nodes; i++) {
    for (j = 0; j < 8; j++) {
      if (addr[j] != addrs[i][j]) {
        break;
      }
    }

    if (j == 8) {
      return i;
    }
  }

  return -1;
}


uint8_t AtomWirePlus::send_msg(uint8_t addr[AWP_ADDR_LENGTH], uint8_t *msg)
{
  int8_t is_not_broadcast, index1, index2;
  // Broadcast if *addr = 0x0000 0000 0000 0000
  for (is_not_broadcast = AWP_ADDR_LENGTH; is_not_broadcast > 0; is_not_broadcast--) {
    if (addr[is_not_broadcast] != 0x00) {
      break;
    }
  }

  if (is_not_broadcast) {
    // queue message
    int8_t pos = get_pos_of_node(addr);
    if (pos == -1) {
      return FALSE;
    }
    for (index1 = 0; index1 < 8; index1++) {
      out_msg_queue[pos][index1] = msg[index1];
    }
  } else {
    // queue message for everyone
    for (index1 = 0; index1 < num_nodes; index1++) {
      for (index2 = 0; index2 < 8; index2++) {
        out_msg_queue[index1][index2] = msg[index2];
      }
    }
  }

  return TRUE;
}

uint8_t AtomWirePlus::recv_msg(uint8_t *addr, uint8_t *msg)
{
  // if (recvr_addr != NULL && recvr_msg != NULL) {
  //   addr = recvr_addr;
  //   msg = recvr_msg;

  //   recvr_addr = NULL;
  //   recvr_msg = NULL;

  //   return TRUE;
  // } else {
  //   return FALSE;
  // }

  return FALSE;
}

void AtomWirePlus::full_search(void)
{
  // uint8_t *addr;

  // for (int i = 100; i < 0; i--) {
  //   this->search(addr);
  // }
}
