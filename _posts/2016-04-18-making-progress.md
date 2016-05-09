---
layout: post
author: Angelina Risi
title: Making Progress!
---

This past week we've been testing the circuit with various different components in an effort to improve node detection. 
Through this, we've upped the one-wire node limit from 3 nodes to 5 nodes.

On Wednesday, April 13th, I took a closer look at the power supplied to the ATtinys. Using a multimeter, I saw that each operates using about 9 mA. When they are powered by the same 5V power supply, a little over 50 mA is used on average.
Also, I tested using between 0 and 3 buffers on the line. Even when powered by an external supply, only one node can run successfully when no buffers are used. Even with two buffers, only one node works all the time, while 2 will have long gaps between successes. 
Using the 3 buffers as usual with more than three nodes resulted in some error in detecting nodes, with a couple nodes being detected periodically. 
This problem was amplified the more nodes we added. At six nodes it could detect node A2 sometimes, but not the rest. If four nodes were left on the line long enough, none would be detected anyore.

I then switched from the external 5V supply to the Arduino's 5V pin. With all nodes powered, communicating with up to four was possible, and any more would lead to errors. At six nodes one address was corrupted, followed by eternal "No further slaves detected" spam (from now on refered to as "NFSD spam").
One of the reasons this setup may have worked slightly better than with the external supply was that I changed to a 1 MOhm pull-up resistor on the one-wire line. A the very least powering more than 3 nodes with the Arduino was proved posible. 
However, when powered by the one-wire line (with the 5.1V Zener and 10 uF capacitor), plugging in a fourth node causes a NFSD spam. Looking at the voltage provided, we could see a significant drop. 
What sould be a 5V line drops down to about 2.93V across the capacitor on average when 3 nodes are connected, still within operating voltage, but drops down to 2.3V with 4 nodes, significantly lower than the necessary 2.7V.

Next I tested switching out the capacitors and diodes, as with then-current circuit the one-wire line was incapable of powering all the nodes itself.
Changing capacitors did not provide very useful results. Decreasing or increasing capacitance caused the circuit to work worse if the new capacitance differed too much from the 10uF used, likely due to the timeing constraints dictated by the software.
Changing the diodes again, however, was a breakthrough. Changing from the 5.1V Zener to a different diode labelled 1N5817 resulted in the circuit surviving a fourth one-wire node.
Adding a fifth node causes a pattern of a couple nodes being detected, but adding a sixth causes NFSD spam and the system recovers once nodes above 4 are removed. 

The next day we did tests on adding and removing nodes in groups. Removing nodes was never a problem, as confirmed by these tests, but adding more than one node yields worse results both the more nodes that are already connected and the more nodes you are trying to connect.
For example, with no nodes connected, one or even two blocks can be added without any problems, but with one block already on the line adding two has complete success 8 out of 10 trials, and with no blocks on the line adding three is far less successful. Adding all four at once successfully is possible, but only happened once out of 10 trials and missed node two on the first pass for that trial. 
The problem may be related to the timing of adding the group of new nodes.

We also used LEDS connected to nodes via transistors to visualize and compare the states of live nodes compared to "dead" (undetected) nodes. We then took an oscilloscope image of the pin output of a live one compared to a dead one. The dead node may actually be stuck in the reset state. The voltage fluctuations of the dead node are likely due to the fluctuations of its VCC input due to the one-wire voltage not being constant.

Yesterday I played around a bit with the reed relays. It would be useful to add switching between branches of nodes to the circuit so block configurations can be detected with multi-node blocks. However, so far connecting a relay to a node consumes too much power, as each requires about 10mA and 5V (around 3.75V minimum) which can only be supplied when only two nodes total are connected.

Finally, today we got 5 nodes working together simply by adding another op-amp buffer to the line. While having so many extra bufferes connected to each line may be problematic, even on the master's side, the circuit has so far worked flawlessly.
