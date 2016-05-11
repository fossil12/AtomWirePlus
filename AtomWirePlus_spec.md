# AtomWire+ protocol

## Timings and sizes

### General times

- Reset/presence pulse time (`t_RP`): t_F + t_RSTL + t_RSTH
	480µs + 480µs = 960µs
- Sending or receiving 1 bit (`t_1B`): t_F + t_SLOT
	70µs
	
_Based on timings of the DS2413 data sheet and AtomWire code. See [this table](https://docs.google.com/spreadsheets/d/1yPVSfb1niXWbVkhceOln84PVQOyeUlXaM6V_9AKL58c/edit?usp=sharing) for all timing values._

### Protocol timings & sizes (64 bit messages)

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

### Protocol timings & sizes (104 bit messages)

- Message size: 104 bits
	see below for details

- Time for sending a message (104 bits):
	104 * t_1B = 7280µs = 7.28ms

- Min time of a time slot: 
	reset/presence pulse + select follower + 
	send message + receive message
	960µs + 5040µs + 2 * (7280µs) = 20560 = 20.56ms

- Search time per node one line:
	reset/presence pulse + sending & receiving 3 * 64 bits
	960µs + 3 * (4480µs) = 14400µs = 14.4ms

- Min time a slave has after it is not selected until the next time slot: 
	2 * send/receive message
	2 * (7280µs) = 14560µs = 14.56ms

## Message format

### 64 bits (8 bytes) 

7 bytes content + 1 byte crc

		msb                                 lsb
		0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff
		cmd  |----------content----------|  crc

	+ cmd: commands
		+ 0xxx: do x

### 104 bits

(based on [RFC 4944](https://tools.ietf.org/html/rfc4944) section [5.3.](https://tools.ietf.org/html/rfc4944#section-5.3))

							 1					 2					 3
		 0 1 2 3|4 5 6 7|8 9 0 1|2 3 4 5|6 7 8 9|0 1 2 3|4 5 6 7|8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|  cmd  | frag? |     size/offset     |     frag_id     |  res. |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		| payload...
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  															 ...payload |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      CRC      |
		+-+-+-+-+-+-+-+-+

	- `cmd`: send (`0x7`)/recv (`0x9`) command
	- `frag?` fragmented message?
		+ `0000`: No message to send (stop receving/sending here)
		+ `1000`: No fragmentation
		+ `1100`: First fragmentation fragment
		+ `1110`: Subsequent fragmentation fragment
	- `size/offset`: 
		+ No fragmentation & first fragementation fragment: size
			* complete length of datagram from link layer in bytes. It needs to support up to 1280 bytes (IPv6 packet) and thus is 11-bit wide.
		+ offset into fragment for subsequent fragmentation fragments. (This is an optimization, becasue the fragments should normally arrive in order the size of the reassembled packet can be determined by the first fragmentation fragment)
	- `frag_id` fragmentation identifier: is the same for every fragmented fragemnt for one link layer fragment (9 bits)
	- `res.`: reserved padding space. Currently unused should be set to 0.
	- `payload`: 64 bits (8 bytes)
	- `CRC`: 8 bit CRC checksum

	_Note_: The leader always sends a `0x90` command before a follower starts it's sending sequence

***

## ATtiny85

8 MHz = 8 clock cycles per µs

## Questions to think about

- How do we handle adding and deleting of nodes after/during a new search?
- Where who gets the next transmission spot?
- How long does a search take?
- What is the minimum delay for from adding one block to adding the next block? (i.e. how often do we need to search for new nodes?)
- What happens when a node is removed before or during communication with it?
- Reliable link layer with acks?