#include "AtomWirePlus.h"

OneWire awm(12); 
int c = 0;
byte out_msg[8];

void setup(void) {
  Serial.begin(9600); 

  out_msg[0] = 0xff;
  out_msg[1] = 0xff;
  out_msg[2] = 0xff;
  out_msg[3] = 0xff;
  out_msg[4] = 0xff;
  out_msg[5] = 0xff;
  out_msg[6] = 0xff;
  out_msg[7] = 0xff;
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  byte in_msg[8];
  
  if (!awm.search(addr)) {
    Serial.print("\nNo further Slaves Detected.\n");
    awm.reset_search();
    return;
  }

  Serial.print("\nAddress = ");
  for (i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if (addr[0] == 0x3A) {
    Serial.print("\nDevice belongs to AtomWire family.");
  } else {
    Serial.print("\nDevice family is not recognized.");
    return;
  }
  
  Serial.print("\nSend msg ");
  for (i = 0; i < 8; i++) {
    Serial.print(out_msg[i], HEX);
  }
  Serial.print("...\n");
  awm.reset();
  awm.select(addr);
  awm.sendMsg(out_msg);

  Serial.print("Receive msg...\n");
  awm.receiveMsg(in_msg);
  Serial.print("Received msg ");
  for (i = 0; i < 8; i++) {
    Serial.print(in_msg[i], HEX);
  }
  Serial.print(".\n");

  for (i = 0; i < 8; i++) {
    out_msg[i] = in_msg[i];
  }

  delay(1500); // 1500 miliseconds (1.5 sec)

  
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




