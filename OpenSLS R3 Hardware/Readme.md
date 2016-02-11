Laser Cutting 
-Laser cut in 5.6mm acrylic
-These parts required ~3 sheets (18x24) for us

Marlin FW
- This version of Marlin is almost identical to our R1/R2 version. The only update is defining a MOSFET for controlling the DC motor which rotates the distributor. The DC motor is controlled by the HEAT0 pin on the RAMBo (see wiring diagram and [RAMBo wiki] (http://reprap.org/wiki/Rambo_development))
G-code for spinning the motor is M106 S### to turn on, M107 to turn off. S### is an integer 0-255 which controls speed via PWM. 

We do not have an assembly guide for this powder module at this time. But, if you are interested in building it we will be happy to help where we can.