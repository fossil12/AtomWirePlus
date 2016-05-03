#ifndef ATOMWIREPLUS_H
#define ATOMWIREPLUS_H

#include "AtomWire.h"

#define REDUE_SEARCH_INTERVAL 4
#define MAX_BLOCKS_ON_LINE 10
#define FRAME_BYTE_LENGTH 13
#define AWP_ADDR_LENGTH 64

class AtomWirePlus : public OneWire
{
  private:
    // number of connected nodes
    uint8_t num_nodes;

    // Node selected for this round (index in `addrs`)
    uint8_t current_node;

    // Incremented at after every run of a time slot
    uint8_t round;

    // store all nodes (followers) on the line
    uint8_t addrs[MAX_BLOCKS_ON_LINE][8];

    // Store up to one message per node to be sent out
    // Same index as in `addrs` 
    uint8_t *out_msg_queue[]; // nothing if entry is NULL

    // Queue up to one message per node that was received
    // Same index as in `addrs`
    uint8_t *in_msg_queue[]; // nothing if entry is NULL

    // sending buffer
    //uint8_t *sender_addr;
    //uint8_t *sender_msg;

    // receving buffer
    //uint8_t *recvr_addr;
    //uint8_t *recvr_msg;

    uint8_t send_msg_p(uint8_t msg[64]);
    uint8_t recv_msg_p(uint8_t msg[64]);
    uint8_t get_pos_of_node(uint8_t addr[64]);

  public:
    AtomWirePlus(uint8_t pin);

    // runs a time slot and returns
    uint8_t run(void); // needs scheduling
    //uint8_t get_node(uint8_t pos, uint8_t *addr);

    // No guarantee, that the message will be immediatly sent, it might get queued
    uint8_t send_msg(uint8_t *addr, uint8_t *msg);
    uint8_t recv_msg(uint8_t *addr, uint8_t *msg);

    // We assume that only node at the end of the array are removed or added
    void full_search(void);
};

#endif // ATOMWIREPLUS_H