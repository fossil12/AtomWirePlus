#ifndef ATOMWIREPLUS_H
#define ATOMWIREPLUS_H

#include "AtomWire.h"
#include "AWP_constants.h"

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
    uint8_t addrs[AWP_MAX_BLOCKS_ON_LINE][AWP_ADDR_BYTE_LENGTH];

    // Store up to one message per node to be sent out
    // Same index as in `addrs` 
    uint8_t out_msg_queue[AWP_MAX_BLOCKS_ON_LINE][8]; // nothing if entry is NULL

    // Queue up to one message per node that was received
    // Same index as in `addrs`
    uint8_t in_msg_queue[AWP_MAX_BLOCKS_ON_LINE][8]; // nothing if entry is NULL

    uint8_t send_msg_p(uint8_t msg[64]);
    uint8_t recv_msg_p(uint8_t msg[64]);
    int8_t get_pos_of_node(uint8_t addr[64]);

    uint8_t num_consecutive_search_errors;

  public:
    AtomWirePlus(uint8_t pin);

    // runs a time slot and returns
    uint8_t run(void); // needs scheduling
    uint8_t get_next_node_addr(uint8_t *addr);
    //uint8_t get_node(uint8_t pos, uint8_t *addr);

    // No guarantee, that the message will be immediatly sent, it might get queued
    uint8_t send_msg(uint8_t *addr, uint8_t *msg);

    /* 
     * Check if there is a new message from a certain node
     *
     * \warning Don't use it together with `recv_msg()`
     */
    uint8_t recv_msg_from(uint8_t *addr, uint8_t *msg);
};

#endif // ATOMWIREPLUS_H
