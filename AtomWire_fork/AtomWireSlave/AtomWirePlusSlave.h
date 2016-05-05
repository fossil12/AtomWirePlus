#ifndef ATOMWIREPLUSSLAVE_H
#define ATOMWIREPLUSSLAVE_H

#include "AtomWireSlave.h"
#include "AWP_constants.h"

// AtomWirePlus errnos start at 10
#define AWP_ERR_WRONG_COMMAND 10
#define AWP_ERR_WRONG_CRC 11
#define AWP_ERR_IN_MSG_OVERRIDDEN 12
#define AWP_ERR_OUT_MSG_OVERRIDDEN 13
#define AWP_ERR_WRONG_SEND_CMD 14

class AtomWirePlusSlave : public OneWireSlave
{
  protected:
    // Run general functions for up to `miliseconds` ms
    // (this a cooparative multitasking)
    void run_general_functions(uint16_t miliseconds);

    // Methods from base class overridden here
    bool recvAndProcessCmd() override;

    bool new_in_msg;
    char in_msg[AWP_FRAME_BYTE_LENGTH];

    bool new_out_msg;
    char out_msg[AWP_FRAME_BYTE_LENGTH];

  public:
    AtomWirePlusSlave(uint8_t pin);

    bool duty() override;
};

#endif // ATOMWIREPLUSSLAVE_H