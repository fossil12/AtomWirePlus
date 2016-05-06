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

  data_out[6]++;
  data_out[7]++;

  Serial.print("\n-----------------------\n\n");

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
  
  if (awm.recv_msg(addr, data) == -2) {
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
}




