---
layout: post
title: "Circuitry So Far"
author: Angelina Risi
---

As of right now AtomWirePlus can only be described as buggy and severely limited, but at least works when limited to three nodes. The "bugs" with our current implementation are as follows:

* Adding a fourth node causes the system to spaz out with a stream of "No further slaves detected," with some rare successful detections of one of the nodes occuring after a ridiculous number of search cycles. Removing the fourth node does not fix it. It must be removed and then the original three nodes removed to be added again successfully.
* Adding multiple nodes at once can cause one or more of them to never be successfully detected until all nodes are added and removed individually.
* When we tried powering all the blocks directly with an external supply, the system similarly had problems with node detection. As more nodes were added to the system, fewer were detected.

In the midst of these setbacks, though, we have determined some things that do work. For example, the system works with one-wire power when using a 10uF capacitor and a 5.1V zener diode instead of a normal 1N4004 diode. In spite of the previous group's success, we could not get multiple nodes functioning with the 1N4004 diodes. Additionally, by adding a 500 Ohm pull-up resistance between the master's 5V supply and the one-wire line, the success rate when connecting nodes to the line appears to have increased. 
The system so far is successful when only up to three nodes are connected. When added and removed individually, the nodes are almost always detected. When two nodes are removed and readded, sometimes it does not detect one or both of the nodes, but detects them both more often than not.

Currently we are trying to determine why the number of nodes is so limited and what needs to change to make the circuit capable of supporting a sizeable number of nodes on an AtomWire line.
