#include "AtomWire.h"

#define NUM_LINES 4

OneWire **awms;

int nodes[NUM_LINES];


void setup(void) {
  Serial.begin(9600); 

  awms = new OneWire*[NUM_LINES];

  uint8_t offset = 10;
  uint8_t i;
  for (i = 0; i < NUM_LINES; i++) {  
    awms[i] = new OneWire(i + offset);
    nodes[i] = 0;
  }
}

void loop(void) {
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  //String msg = "";

  for (int j = 0; j < NUM_LINES; j++) {
    while(awms[j]->search(addr)){
      delay(30);
      //if ( addr[0] != 0x3A ) return;
      nodes[j]++;
    }
    Serial.write(j); //Send Line #
    Serial.write(nodes[j]); //Send # Nodes
    
    //Parts that should be repeated for each node on line
    //Need to store nodes on line + implement in game
    if (nodes[j] > 0) {
      Serial.write(addr[1]); //Node ID
      Serial.write(0x00); //Send type ('0' is default)
      Serial.write(0x00); //Send node pin status ('0' default)
    }
    nodes[j] = 0;
    awms[j]->reset_search();
  }
  
  //Serial.print(msg);
}



