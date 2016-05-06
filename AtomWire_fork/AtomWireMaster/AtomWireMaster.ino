#include "AtomWirePlus.h"

AtomWirePlus awm(12);
int c = 0;
byte data[8];
byte data_out[8];

void setup(void) {
  Serial.begin(9600);
  data_out[6] = 0x00;
  data_out[7] = 0x00;
  Serial.print("\n======== Begin ========\n");
}

void loop(void) {
  byte i;
  byte addr[8];

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
  byte pos;

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
  
}




