# Heat-Insert-Press (Work in progress!)
3D-printable Heat-Insert Press with custom PID controller PCB

![preview_image](/cad/press.png)

## Introduction
Heat inserts are small brass cylinders with a thread in it. Typically used for injection moulding, they quickly became a popular way to connect 3D-printed parts. You use them by melting the inserts into a slightly smaller hole, thus making a strong connection. 
I found them very hard to install, but before making a final judgement, I wanted to build a rig to be able to properly install them. Inspired by previous projects by [Adafruit] (https://learn.adafruit.com/heat-set-rig) and [maker.moekoe] (https://github.com/makermoekoe/Heat_Insert_Press), I planned on using a regular soldering iron to heat up the inserts. 
However, I quickly decided not to misuse the soldering iron, because it would be bad for the tips. I had a cheap hotend lying around, which was totally unusable for melting plastic, because of the poor machining quality. Being able to reuse it for the press gives it a good use. 
I could have also used a spare 3D-printer mainboard, but like with other projects, I decided to make my own PCB to control it. 

## Current build status
### CAD
At the moment I am finishing the mechanical parts in Fusion 360, there is only the top part left to draw. Once I confirmed that all parts are working as intended, I will re-draw all the files to have a clean project and upload the Fusion 360 file on GitHub. Although I do not recommend building the press right now, I have uploaded the STL-files. 
### PCB
I have already a finished PCB design, however I will drastically change it. I am planning to add: 
- [ ] USB-C Power Delivery
- [ ] Add alternative microcontroller (Piksey Atto is hard to buy)
- [ ] Remove Mosfet for fan
- [ ] Optimize for SMD components
### Code
No code available yet.
