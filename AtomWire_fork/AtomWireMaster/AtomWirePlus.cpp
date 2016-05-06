#include "AtomWirePlus.h"

AtomWirePlus::AtomWirePlus(uint8_t pin) : OneWire(pin) {
  // Does that work like that?
  num_nodes = 0;
  round = 0;
  num_consecutive_search_errors = 0;
  current_node = 0;
  front_queue_node = 0;
  next_queue_node = 0;
}

// Assumes msg is 64 bits (8 bytes) long
uint8_t AtomWirePlus::send_msg_p(uint8_t msg[8])
{
  int not_empty_message;
  uint8_t frame[AWP_FRAME_BYTE_LENGTH];

  // Send bits are 0x7
  uint8_t cmd_frag = 0x70;

  // Check if message is empty
  for (not_empty_message = 8; not_empty_message > 0; not_empty_message--) {
    if (msg[not_empty_message - 1]) {
      break;
    }
  }

  if (!not_empty_message) {
    this->write(cmd_frag); // send command indicating empty message
    return TRUE;
  }

  // We currently only support unfragmented messages
  cmd_frag |= 0x08;

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

  frame[12] = this->crc8(frame, AWP_FRAME_BYTE_LENGTH - 1);

  this->write_bytes(frame, AWP_FRAME_BYTE_LENGTH);

  return TRUE;
}

/* Return:
 * - 2: no new message
 * - 1: new message
 * - 0: error
 */
uint8_t AtomWirePlus::recv_msg_p(uint8_t msg[8])
{
  uint8_t frame[AWP_FRAME_BYTE_LENGTH];

  // recev bits 0x90
  this->write(0x90); // Receive request sent to sender
  this->read_bytes(frame, 1);

  // Check if there is anything to receive
  if (frame[0] == 0x90) {
    return 2;
  }

  // Get rest of frame
  this->read_bytes(&frame[1], AWP_FRAME_BYTE_LENGTH - 1);

  // Check if it's a send request
  if ((frame[0] & 0xF0) != 0x90) {
    return 0;
  }

  // TODO: Check CRC
  // TODO: Check for fragmentation and start/continue reassembly if true

  // Copy received message into message buffer
  for (int i = 0; i < 8; i++) {
    msg[i] = frame[i + 4];
  }

  return 1;
}

uint8_t AtomWirePlus::run(bool doSearch)
{
  uint8_t err;
  int result = TRUE;

  if ((round % AWP_REDUE_SEARCH_INTERVAL) == 0 && doSearch) {
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
    result = FALSE;
  }

  // Time to receive message
  // TODO: Check for fragmenting and start assembling incoming message
  err = recv_msg_p(in_msg_queue[current_node]);
  if (err == 0) {
    result = FALSE;
  }

  // Check if there was a new message and add it to queue
  if (err == 1) {
    msg_queue[next_queue_node].addr = addrs[current_node];
    msg_queue[next_queue_node].pos = current_node;
    msg_queue[next_queue_node].msg = in_msg_queue[current_node];

    next_queue_node = (next_queue_node + 1) % MAX_MSG_QUEUE_LENGTH;
  }

  // XXX: give the node some time to do calculations if it's only one
  // Currently solve by give everyone 8 additional ms
  delayMicroseconds(8);

  return result;
}

uint8_t AtomWirePlus::run_all(void)
{
  full_search();

  // We don't care with which node we start. We just want to connect to each
  // node once
  for (int i = 0; i < num_nodes; i++) {
    this->run(false);
  }

  return num_nodes;
}

uint8_t AtomWirePlus::get_next_node_addr(uint8_t *addr)
{
  int next_node_pos;

  if (num_nodes == 0 || (round % AWP_REDUE_SEARCH_INTERVAL) == 0) {
    full_search();
  }

  round++;

  if (num_nodes == 0) {
    return FALSE;
  } else {
    next_node_pos = (current_node + 1) % num_nodes;
    for (int index = 0; index < 8; index++) {
      addr[index] = addrs[next_node_pos][index];
    }
    return TRUE;
  }
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
inline int8_t AtomWirePlus::get_pos_of_node(uint8_t addr[64])
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

uint8_t AtomWirePlus::recv_msg_from(uint8_t *addr, uint8_t *msg)
{
  int8_t pos, index, not_empty_message;

  pos = get_pos_of_node(addr);

  if (pos == -1) {
    return FALSE;
  }

  not_empty_message = 0;
  for (index = 0; index < 8; index++) {
    msg[index] = in_msg_queue[pos][index];
    in_msg_queue[pos][index] = 0x00;

    if (msg[index]) {
      not_empty_message = 1;
    }
  }

  if (not_empty_message) {
    return TRUE;
  } else {
    return -2;
  }
}

uint8_t AtomWirePlus::recv_msg(uint8_t *addr, int8_t *pos, uint8_t *msg)
{
  if (front_queue_node != next_queue_node) {
    // we have a message in the queue
    int index;

    for (index = 0; index < 8; index++) {
      addr[index] = msg_queue[front_queue_node].addr[index];
      msg[index] = msg_queue[front_queue_node].msg[index];
    }

    pos[0] = msg_queue[front_queue_node].pos;

    front_queue_node = (front_queue_node + 1) % MAX_MSG_QUEUE_LENGTH;

    return TRUE;
  } else {
    return FALSE;
  }
}

int8_t AtomWirePlus::full_search(void)
{
  int8_t temp_num_nodes, highest_pos_found, pos, index;
  uint8_t found_addr[AWP_ADDR_BYTE_LENGTH];

  this->reset_search();

  highest_pos_found = 0;

  // Search all nodes
  for (temp_num_nodes = 0; this->search(found_addr) 
                           && temp_num_nodes <= AWP_MAX_BLOCKS_ON_LINE; 
       temp_num_nodes++) {
    pos = get_pos_of_node(found_addr);

    if (pos == -1) { // New address
      if (num_nodes == AWP_MAX_BLOCKS_ON_LINE) {
        return FALSE;
      }

      // Copy address into address array
      for (index = 0; index < AWP_ADDR_BYTE_LENGTH; index++) {
        addrs[num_nodes][index] = found_addr[index];
      }

      highest_pos_found = num_nodes;
      num_nodes++;
    } else if (pos > highest_pos_found) { // found address at highter position
      highest_pos_found = pos;
    }
  }

  // Check if we found all nodes
  // or no node
  if (temp_num_nodes == highest_pos_found  + 1 || 
      (temp_num_nodes == 0 && highest_pos_found == 0)) {
    // Found the right number of nodes
    num_consecutive_search_errors = 0;
    num_nodes = temp_num_nodes;
    return TRUE;
  } else {
    // Error during search assume we have no new
    // Reset nodes after 5 consecutive errors (allows some instability)
    if (num_consecutive_search_errors > 5) {
      num_nodes = 0;
    }
    num_consecutive_search_errors++;
    return FALSE;
  }
}
