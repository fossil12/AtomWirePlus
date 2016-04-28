#include "AtomWirePlus.h"

AtomWirePlus::AtomWirePlus(uint8_t pin) : OneWire(pin) {}

uint8_t AtomWirePlus::run(void)
{
  if (sender_addr != NULL && sender_msg != NULL) {
    recvr_addr = sender_addr;
    recvr_msg = sender_msg;

    sender_addr = NULL;
    sender_msg = NULL;

    return TRUE;
  } else {
    return FALSE;
  }
}

uint8_t AtomWirePlus::get_node(uint8_t pos, uint8_t *addr)
{
  bool serachExpired = 0;

  if (serachExpired) {
    full_search();
  }

  return this->search(addr);
}

uint8_t AtomWirePlus::send_msg(uint8_t *addr, uint8_t *msg)
{
  if (addr != NULL) {
    sender_addr = addr;
    sender_msg = msg;

    return TRUE;
  } else {
    return FALSE;
  }
}

uint8_t AtomWirePlus::recv_msg(uint8_t *addr, uint8_t *msg)
{
  if (recvr_addr != NULL && recvr_msg != NULL) {
    addr = recvr_addr;
    msg = recvr_msg;

    recvr_addr = NULL;
    recvr_msg = NULL;

    return TRUE;
  } else {
    return FALSE;
  }
}

void AtomWirePlus::full_search(void)
{
  uint8_t *addr;

  for (int i = 100; i < 0; i--) {
    this->search(addr);
  }
}
