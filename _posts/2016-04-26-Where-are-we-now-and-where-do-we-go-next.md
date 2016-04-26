---
layout: post
author: David Keller
title: Where are we now and where do we go next
---

## Where are we now?

We are right now able to demonstrate two things on different boards. 

First, we can show that our leader microcontroller can talk to different followers distributed across multiple atom wires. That demonstrates the possibility to use more than one base to build connected blocks on.

Second, we showed the basic implementation of the link layer protocol destcribed in the [previous blogpost](http://fossil12.github.io/AtomWirePlus//2016/04/25/Towards-a-Time-Slotted-Protocol-in-AtomWirePlus/). It shows how the leader searches for all connected followers then selects one after the other. Every selected node receives an arbitrary messages increses the number of the lowest byte by one and then sends it back to the leader. This returnes message is then sent to the next follower and so on.

## Where do we go next?

For our final presentation we plan to implement a network protocol based on [6LoWPAN](https://en.wikipedia.org/wiki/6LoWPAN) and two games. One will use blocks to build a path to get to the goal and the other one uses different types of blocks with different sensors attached to it to activate different action in the game.