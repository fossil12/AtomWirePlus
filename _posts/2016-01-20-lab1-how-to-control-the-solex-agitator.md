---
layout: post
title: "Lab 1: How to control the Solex Agitator"
author: Angelina Risi
---

![Circuit labeled]({{ site.baseurl }}/img/lab1/Lab1_Circuit_Labeled.jpg)

In this this lab we built a circuit that produces sinusoidal signals of varying amplitude and frequency. The purpose of this exercise was to re-familiarize ourselves with useful Operational Amplifier (Op-Amp) circuit configurations and the Detkin lab equipment.

![schematic]({{ site.baseurl }}/img/lab1/schematic.png)

The first stage of the circuit is the astable multivibrator. It is similar to a comparison Op-Amp circuit and functions by charging and discharging a capacitor to alternate between hitting the positive voltage rail for a fixed period of time and hitting the negative voltage rail for the same period of time. The frequency at which this occurs is adjusted via the 100 kOhm potentiometer in this part of the circuit - the rate of charge and discharge of a capacitor is based on its time constant, which is dependent on capacitance and resistance, so changing the resistance changes the frequency.

The second stage is an inverting amplifier Op-Amp circuit. The output voltage has a gain approximately equal to the feedback resistance divided by the input resistance. In this circuit, the input resistor is constant and the feedback resistor is replaced by a 100 kOhm potentiometer, allowing one to adjust the output wave's amplitude.

The final two stages are integrator Op-Amp circuits. The first integrates the square wave into a triangular, or saw-tooth wave, and then the second integrates that into near-sinusoidal quadratic waves. Like the astable multivibrator, the integrators function by charging and discharging a capacitor, causing a varying voltage over time. The output wave is approximately directly proportional to the integral of the input, with a gain equal to the negative reciprocal of the time constant of the capacitor. 

The final output of the completed circuit is a sinusoidal wave with a frequency and amplitude adjustable using the potentiometers in the first and second stage, respectively. The oscilloscope image below displays the final sinusoid, the triangular wave, and the square wave just before integration.

![oscilloscope outputs]({{ site.baseurl }}/img/lab1/OscilloscopeOutputs.png)

In this lab, we refreshed our memory on the functions of common Op-Amp configurations and used them to create a simple electrical device. Of note is the fact that the square wave is not perfectly square. This is due to the fact that real devices cannot work instantaneously - the astable multivibrator's capacitor's charge-up time limited how ideal the output appears. Otherwise, the final product appears to work as intended.