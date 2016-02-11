# OpenSLS

##### An open-source Selective Laser Sintering platform developed at Rice University
##### Miller Lab: Physiologic Systems Engineering and Advanced Materials
##### [millerlab.rice.edu] (http://millerlab.rice.edu)
&nbsp;

###Updates 02/10/16

- We are excited to annouce that OpenSLS has been published! You can find our paper at [PLOS ONE's website] (http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0147399).
Many of the hardware-related details (wiring diagrams, G-code editing, Bill of Materials) are in the supporting information, which we have uploaded here.  
- We are releasing our R3 hardware update and the corresponding firmware. What's new? See below.
- A Python version of our G-code munging utility is now available, which may be more convenient than our earlier PHP implementation

###Build Documentation
This git repository includes:
- Designs for lasercut and 3D printed parts for our first (R1), second (R2), and third (R3) generations of powder handing modules
- Current firmware (with thanks to Tim Schmidt!)
- Slic3r configuration and G-code munging script for G-code creation and processing
- Wiring diagram for electronics configuration (based on RAMBo electronics)
- Model geometries we showcased in our PLOS ONE paper
- Bill of Materials

###Hardware Versions
Basic summary of our hardware verions to date:

1. R1, designed by Andreas Bastian: Proof-of-concept build for OpenSLS. Buld is almost entirely acrylic + 3D printed parts. Powder distributor is a metal rod which counter-rotates, driven by belt motion. This build has everything you need to do SLS, but it lacks a simple powder collection system.
2. R2, designed by Andreas Bastian: This was the hardware used for the entirety of our PLOS ONE paper. It is mostly made from acrylic, but has an aluminum build piston and platform amenable to heating. Distributor is a static plow (a lab spatula). Powder is collected into a hopper through ducts built into the acrylic design.
You can read more about the design upgrades Andreas made from R1 hardware to R2 hardware on the [RepRap wiki page] (http://reprap.org/wiki/OpenSLS#R1_Hardware). 
3. R3, designed by Ian Kinstlinger, Anderson Ta, and David Yalacki: This is an all-acrylic build designed for low-cost and simplicity. Larger powder ducts facilitate powder collection and the overall form factor is a bit smaller. Note that this build will not be compatible with any powder pre-heating, but it's been working great for our purposes. Distributor is a steel 8mm rod mounted on a pair of 608 bearings. A [DC Motor](http://www.amazon.com/Science-Wiz-Motors-Working-Voltage/dp/B000BRUHW8/ref=sr_1_2?ie=UTF8&qid=1455162773&sr=8-2&keywords=dc+motor) spins the rod to give you a rotating distributor and the whole thing is mounted on springs to adjust height.

###Links
[PLOS ONE OpenSLS paper] (http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0147399).
[RepRap OpenSLS wiki page] (http://reprap.org/wiki/OpenSLS): Documentation of early OpenSLS builds including video

 

![MillerLab logo](https://github.com/MillerLabFTW/OpenSLS/blob/master/MillerLab_logo.jpg)

###Acknowledgements
- Thanks to [SeeMeCNC](http://www.seemecnc.com/) for helping us install and configure our laser cutter
- Thanks to Ultimachine for RAMBo support
- Thanks to the Marlin dev team and to Tim Schmidt for specially modified laser firing firmware
- Thanks to all the open-source 3D printing software that has helped us build and operate OpenSLS: Arduino, RepRap, Python, Slic3r, Netfabb, Pronterface, OpenSCAD, and more!