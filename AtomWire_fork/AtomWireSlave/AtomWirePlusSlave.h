#ifndef ATOMWIREPLUSSLAVE_H
#define ATOMWIREPLUSSLAVE_H

#include "AtomWireSlave.h"
#include "AWP_constants.h"

// AtomWirePlus errnos start at 10
#define AWP_ERR_WRONG_COMMAND 10
#define AWP_ERR_WRONG_CRC 11
#define AWP_ERR_IN_FRAME_OVERRIDDEN 12
#define AWP_ERR_OUT_FRAME_OVERRIDDEN 13
#define AWP_ERR_WRONG_SEND_CMD 14
#define AWP_ERR_IN_MSG_OVERRIDDEN 15
#define AWP_ERR_OUT_MSG_OVERRIDDEN 16

class AtomWirePlusSlave : public OneWireSlave
{
  protected:
    // Run general functions for up to `miliseconds` ms
    // (this a cooparative multitasking)
    void run_general_functions(uint16_t miliseconds);

    // Methods from base class overridden here
    bool recvAndProcessCmd() override;

    bool new_in_frame;
    char in_frame[AWP_FRAME_BYTE_LENGTH];

    bool new_out_frame;
    char out_frame[AWP_FRAME_BYTE_LENGTH];

    bool new_in_msg;
    uint8_t in_msg[8];

    bool new_out_msg;
    uint8_t out_msg[8];

    void parse_in_frame();
    void create_out_frame();

    // ======================== //
    // Message specific methods //
    // ======================== //

    // Reads a new message and increments the value of the last byte by 1
    void increment_last_value();

    // Checks if the GPIO pins changed since the last call and if that's true
    // creates a new message with the current GPIO pin state
    uint8_t gpio_pin_state;
    void check_all_gpio_pins();

  public:
    AtomWirePlusSlave(uint8_t pin);

    bool duty() override;
};

#endif // ATOMWIREPLUSSLAVE_H