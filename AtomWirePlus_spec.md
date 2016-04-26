# AtomWire+ TDMA

_Work in progress..._

- How do we handle adding and deleting of nodes after/during a new search?
- Where who gets the next transmission spot?
- How long does a search take?
- What is the minimum delay for from adding one block to adding the next block? (i.e. how often do we need to search for new nodes?)
- What happens when a node is removed before or during communication with it?

## Timings and sizes

### General times

- Reset/presence pulse time (`t_RP`): t_F + t_RSTL + t_RSTH
	480µs + 480µs = 960µs
- Sending or receiving 1 bit (`t_1B`): t_F + t_SLOT
	70µs
	
_Based on timings of the DS2413 data sheet and AtomWire code. See [this table](https://docs.google.com/spreadsheets/d/1yPVSfb1niXWbVkhceOln84PVQOyeUlXaM6V_9AKL58c/edit?usp=sharing) for all timing values._

### Protocol timings & sizes

- Message size: 64 bits (8 bytes)
	7 bytes content & 1 byte crc

- Time for sending a message (64 bits):
	64 * t_1B = 4480µs = 4.48ms

- Time for selecting a follower (= time to send command & message):
	8 bit select command + 64 bit address
	8 * t_1B + 64 * t_1B = 5040µs = 5.04ms

- Min time of a time slot: 
	reset/presence pulse + select follower + 
	8 bit command + send message + 
	8 bit command + receive message
	960µs + 5040µs + 2 * (5040µs) = 16080µs = 16.08ms

- Search time per node one line:
	reset/presence pulse + sending & receiving 3 * 64 bits
	960µs + 3 * (4480µs) = 14400µs = 14.4ms

- Min time a slave has after it is not selected until the next time slot: 
	2 * 8 bit command + 2 * send/receive message
	2 * (5040µs) = 10080µs = 10.08ms

## Message format

- 64 bits (8 bytes): 7 bytes content + 1 byte crc

		msb                                 lsb
		0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff
		cmd  |----------content----------|  crc

- cmd: commands
	- 0xxx: do x

***

## ATtiny85

8 MHz = 8 clock cycles per µs