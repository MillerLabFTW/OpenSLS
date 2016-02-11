The settings used for laser sintering with OpenSLS differ considerably from the typical defaults used for melt extrusion printing. 
A configuration file for Slic3r is included here.  


An important adjustment necessary in preparing geometries for OpenSLS involves scaling the model in z. 
Slic3r settings do not allow a layer height taller than the nozzle diameter, so we treat the layer height as 0.05mm in Slic3r. 
In actuality, the layer height is 0.15 (nylon) or 0.30mm (PCL). 
To compensate, the geometry needs to be scaled down in z by the scaling factor between Slic3r layer height and actual layer height. 
(For a geometry being sliced with layer height 0.05mm and sintered with layer height 0.15, 
the geometry should be shrunk by a factor of 3 in the z-axis (Netfabb Basic))

Summary of configuration:

Parameter	Value		Parameter	Value
Layer height	0.05mm		   		Infill density	Variable – we often used 20%
First layer height	0.05mm			Fill pattern	Concentric
Perimeters (min)	0		I	Infill every 1 layers
Horizontal Shells	0 (Top & Bottom)	Solid infill every 0 layers
Extra perimeters if needed	No		Support material	Disable
Detect thin walls	Yes			Skirt	0 loops
Detect bridging perimeters	Yes		Brim width	0mm
Speed: perimeters	30mm/s			Bed Size	X: 70 Y: 70 mm
Speed: infill	60mm/s				Bed Center 	X: 35 Y: 35 mm
Speed: travel	130mm/s				Nozzle Diameter	0.075mm (laser spot size)
First layer speed	50%			

G-code Munging
The G-code output from slicing an STL model is meant to be interpreted by a melt extrusion printer, not SLS. 
Thus, the G-code must be modified so that Z changes invoke the addition of a new powder layer and XY motion is 
assigned the appropriate laser firing behavior. 

##New Python munging script

We wrote a simple Python script to perform the necessary G-code edits. It is described in detail in our Supporting Information file for our PLOS ONE paper and posted here.
Simply run the Python script and you will be prompted to choose a file to munge.

A neat function of the new munge script is the ability to slice laser power settings into the geometry. This is done by associating extruders in Slic3r with power settings in the munge script. 


## Old PHP script

We performed munging (data editing) of the G-code with a custom 
PHP script that is available on the OpenSLS Github repository. The PHP script is run from the command line as 
follows (example in Windows operating system):

> cd C:\TargetDirectory
> “C:\...\PHP\php.exe” “C:\...\Munge\munge.php” –i “C:\...\file.gcode”

This results in the creation of the modified G-code file in TargetDirectory with the name output.gcode. 
