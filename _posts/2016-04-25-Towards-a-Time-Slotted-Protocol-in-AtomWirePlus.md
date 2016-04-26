---
layout: post
author: David Keller
title: Towards a Time-Slotted Protocol in AtomWire+
---

We did a thorough study of the exiting AtomWire protocol based on the 1-Wire protocol (described in the [DS2413 data sheet](https://datasheets.maximintegrated.com/en/ds/DS2413.pdf)). The outcome is a AtomWire Specification that can be found [here](https://github.com/fossil12/AtomWirePlus/blob/master/AtomWire_spec.md) and [a table](https://docs.google.com/spreadsheets/d/1yPVSfb1niXWbVkhceOln84PVQOyeUlXaM6V_9AKL58c/edit?usp=sharing) containing a summary of the different times for the protocol described in the data sheet and examined from the AtomWire code.

This examination showed us one two issues of the existing protocol. First, the timing for every iterations/rounds of the protocol is of arbitrary length. Every iteration starts with the reset/presence pulse sequence. This is needed to synchronize the followers with the leader. Because this pulse can happen at any time the followers always have to listen on the wire and it’s hard to do independent and inverleaving calculations on the followers. Second, every command after the reset/presence pulse and the selection of a node has different parameters and thus takes a different amount of time and to add a new command all the timings for this command need to be added specifically to the code of the leader and followers to be supported.

## The AtomWire+ Protocol

To overcome this issue AtomWire+ implements a time-slotted protocol with messages based on the existing protocol.

### Time Slots

There exists two time slots: the search time slot and the message time slot. Every iteration of the protocol starts with the reset pulse by the leader following a presence pulse of the followers. If no presence pulse is received by the leader, he will continue to check for followers (every _xx_ ms) with the reset/presence pulse sequences until a follower shows up.

#### Search Time Slot

If follower(s) show up the leader starts an search[^sends command `0xF0`] to find all the followers and their addresses. The search is the same as in the 1-Wire protocol and described [here](https://www.maximintegrated.com/en/app-notes/index.mvp/id/187). Every search iteration will take 14400µs (14.4ms). If a follower is excluded until the next iteration it can do other calculations or go in a low power mode until the next time slot expires. The search is finished when all followers on the wire are detected and followed by at least one message time slot.

The search is repeated in a specific interval to detect added or removed followers.

#### Message Time Slot

This time slot is used to exchange messages between the leader and one follower. After the reset/presence pulse the leader selects a follower[^`0x55` followed by the AtomWire ID of the follower] and then does two message exchanges. First it sends a command (1 byte) followed by a message (8 bytes) to the follower and the it issues another command (1 byte) indicating to the follower that it can reply with a message (2 bytes). This concludes a message times slot and takes 16080µs (16.08ms). In the next iteration the leader selects another follower on a fair ordering basis. Followers that don’t take part in the existing time slot can do other calculation or go to sleep after the selection command and check again after the time slot is expired.

### Messages

The transmitted messages are 8 bytes (64 bits) long and can theoretically contain arbitrary information. To make parsing and error detection of messages possible we introduce the following message format.

	msb   7    6    5    4    3    2    1    0    lsb
	     0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff
	     cmd-|-----------content-----------|-crc

For error detection the last byte (least significant byte) is a CRC-8 check value (like in the 1-Wire protocol). The most significant byte encodes the command indicating to the receiver how to parse the content of the message.

_This specification is a "work in progress". The most recent specification can be found [here.](https://github.com/fossil12/AtomWirePlus/blob/master/AtomWirePlus_spec.md)_