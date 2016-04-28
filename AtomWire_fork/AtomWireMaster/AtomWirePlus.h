#ifndef ATOMWIREPLUS_H
#define ATOMWIREPLUS_H

#include "AtomWire.h"

class AtomWirePlus : public OneWire
{
  private:
    // store all nodes (followers) on the line
    uint8_t *addrs[];

    // sending buffer
    uint8_t *sender_addr;
    uint8_t *sender_msg;

    // receving buffer
    uint8_t *recvr_addr;
    uint8_t *recvr_msg;

  public:
    AtomWirePlus(uint8_t pin);

    // runs a time slot and returns
    uint8_t run(void); // needs scheduling
    uint8_t get_node(uint8_t pos, uint8_t *addr);

    uint8_t send_msg(uint8_t *addr, uint8_t *msg);
    uint8_t recv_msg(uint8_t *addr, uint8_t *msg);

    void full_search(void);
};

#endif // ATOMWIREPLUS_H