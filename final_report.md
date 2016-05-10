---
layout: page
title: Final Report
author: David Keller
permalink: /final_report
---

- Introduction / Overall idea (David)
- system design (David)
    - how the hw/software interacts
    - Explain the protocol & timings
- hardware API (Angelina)
- software API (David)
- mechanical design (Angelina)
- functioning on the system
    - How well does it work?
- Conclusion (Angelina)

***

## Introduction / Overall idea

Since decades kids like to play with brick toys like Legos and other brands to build physical buildings. In recents years a lot of virtual worlds in games became more popular along kids. But we miss today a seamless connection between those two worlds. Why shouldn’t figures build with the brick appear in a game? Why can’t we have different blocks with different meanings that trigger actions depending on the current game? Why can’t blocks «react» to events in a game?

The answer is easy. Currently no easy programmable, low cost and energy efficient solution exists for that. Our project - AtomWire+ - aims to overcome these issues and provides a platform that can easily be adopted by brick builders and game creators.

AtomWire+ builds on the foundations laid by AtomWire which is based on the 1-Wire protocol defined in the [DS2413 data sheet](https://datasheets.maximintegrated.com/en/ds/DS2413.pdf).

## System Design

## Software API

The software is divided in two parts - like the over all system - and encapsulated in C++ classes. The `AtomWirePlus` class contains the code functionality for the master, `AtomWirePlusSlave` is the class exposing the slave functionality.

### AtomWirePlus



### AtomWirePlusSlave

