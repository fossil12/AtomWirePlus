#include "AtomWirePlus.h"

AtomWirePlus awm(12);
int c = 0;
byte data[8];
byte data_out[8];

// Needed for games
#define NUM_LINES 4

AtomWirePlus **awms;

void setup(void) {
  Serial.begin(9600);
  data_out[6] = 0x00;
  data_out[7] = 0x00;

  // Games...
  awms = new AtomWirePlus*[NUM_LINES];
  byte offset = 10;
  for (int i = 0; i < NUM_LINES; i++) {
    awms[i] = new AtomWirePlus(i + offset);
  }
  
  Serial.print("\n======== Begin ========\n");
}

void loop(void) {
  byte i;
  byte addr[8];
  byte pos;

  Serial.print("\n-----------------------\n\n");

  /* ================================================================================
   * Send messge around
   * ================================================================================ */
  /*
  data_out[6] += 1;
  data_out[7] += 1;


  if (!awm.get_next_node_addr(addr)) {
    Serial.print("\nNo more nodes detected...\n");
    return;
  }

  Serial.print("\nAddress = ");
  for (i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n\n");

  awm.send_msg(addr, data_out);
  awm.run();
  
  if (awm.recv_msg_from(addr, data) == -2) {
    Serial.print("\nNo message received...\n");
    return;
  }

  Serial.print("\nMsg = ");
  for (i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");

  delay(1500); // 1500 miliseconds (1.5 sec)
  */

  /* ================================================================================
   * Check of any incoming message
   * ================================================================================ */

  /*if (!awm.get_next_node_addr(addr)) {
    Serial.print("\nNo more nodes detected...\n");
    return;
  }

  Serial.print("\nAddress = ");
  for (i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n\n");

  awm.run();
  
  if (awm.recv_msg_from(addr, data) == -2) {
    Serial.print("\nNo message received...\n");
    return;
  }

  Serial.print("\nMsg = ");
  for (i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");

  delay(1500); // 1500 miliseconds (1.5 sec) */

  /* ================================================================================
   * Messqe queue
   * ================================================================================ */
  /*

  awm.run_all();

  while (awm.recv_msg(addr, &pos, data)) {
    Serial.print("\nAddress = ");
    for (i = 0; i < 8; i++) {
      Serial.print(addr[i], HEX);
      Serial.print(" ");
    }

    Serial.print("Pos = ");
    Serial.print(pos);
    Serial.print("\n");

    Serial.print("Msg = ");
    for (i = 0; i < 8; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.print("\n\n");
  }

  Serial.print("No more messages...\n\n");

  delay(1500); // 1500 miliseconds (1.5 sec) */

  /* ================================================================================
   * Games
   * ================================================================================ */

  for (i = 0; i < NUM_LINES; i++) {
    byte num_nodes = awm.run_all();

    Serial.write(num_nodes);

    // send changed status if available
    while (awm.recv_msg(addr, &pos, data)) {
      Serial.print(addr[1]); // Send node id
      Serial.print(addr[2]); // Send node type (0x00 is default)
      Serial.print(data[1]); // Send changed node pin status
    }
  }
  
}




