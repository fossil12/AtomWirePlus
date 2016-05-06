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

  
//  if (!awm.search(addr)) {
//    Serial.print("\nNo further Slaves Detected.\n");
//    awm.reset_search();
//    return;
//  }
//
//  Serial.print("\nAddress = ");
//  for (i = 0; i < 8; i++) {
//    Serial.print(addr[i], HEX);
//    Serial.print(" ");
//  }
//
//  if (addr[0] == 0x3A) {
//    Serial.print("\nDevice belongs to AtomWire family.");
//  } else {
//    Serial.print("\nDevice family is not recognized.");
//    return;
//  }
//   
//  awm.reset();
//  awm.select(addr);
//  
//  // Writing data into the scratchpad - Block, Node IDs as well as to GPIO output pins.
//  if (addr[1] == 0xA1) {
//    awm.write(0x22); // Write Block and Node ID into scratchpad of selected slave. 
//  } else if (addr[1] == 0xA2) {
//    
//    if (c % 4 == 0) {
//      awm.write(0x8A); // Write "E" to the GPIO pins. 
//      //awm.write(0x21);
//    } else if (c % 4 == 1) {
//      awm.write(0x8C);
//    } else if (c % 4 == 2) {
//      awm.write(0x86);
//    } else {
//      awm.write(0x8E);
//    }
//  } else {
//    awm.write(0x43); // Set block size to 2x2 and block/node id to 3
//    //awm.depower();
//  }
//  
//  delay(1500); // 1500 miliseconds (1.5 sec)
//
//  // Reading scratchpad data - Block, Node IDs as well as GPIO input pins.
//  present = awm.reset();
//  awm.select(addr);
//    
//  //if(addr[1] == 0xA1)  
//  //awm.write(0x8D);  // Read Scratchpad - 0xBE for just read & 0xA1 for GPIO read.
//  
//  if(addr[1] == 0xA1) {
//    awm.write(0xA1); // read gpio pins
//  } else {
//    awm.write(0xBE); // read scratchpad (without updating gpio pins)
//  }
//
//  Serial.print("\nP = ");
//  Serial.print(present, HEX);
//  Serial.print(" ");
//  for (i = 0; i < 9; i++) {           // we need 9 bytes
//    data[i] = awm.read();
//    Serial.print(data[i], HEX);
//    Serial.print(" ");
//  }
//  Serial.print("\nCRC = ");
//  Serial.print( OneWire::crc8( data, 8), HEX);
//  Serial.println();
//
//  c++; 
}




