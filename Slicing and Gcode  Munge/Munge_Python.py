###Munge File for #MillerLab OpenSLS
#
# Ian Kinstlinger
# Revised 08/2015
#
#

import math
import re
 
from Tkinter import Tk
from tkFileDialog import askopenfilename
from tkFileDialog import asksaveasfilename
from tkFileDialog import asksaveasfile


################################
################################
#DEFINITIONS
#
TRAVEL_SPEED = 8000 #mm/min
LAYER_HEIGHT_SLIC3R = 0.05 #mm
FEED_MULTIPLIER = 0.15 #Change from 0 to use feed piston
MOTOR_SPEED = 80 #PWM value 0-255
PLOW_SPEED = 2000 #mm/min
SINTER_SPEED = 500 #mm/min
POWER_1 = 21.5 #PWM % 0-100
POWER_2 = 20 #PWM % 0-100
POWER_3 = 21 #PWM % 0-100


print("\n\nChoose the original G-code file")


Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing
orig_gcode = askopenfilename() # show an "Open" dialog box and return the path to the selected file

print(orig_gcode)

Munged = asksaveasfile(mode='w', defaultextension=".gcode")

#outname = raw_input("Name for the output file: ")

#Munged = open(outname + ".gcode", 'a')

prevline = ''

with open(orig_gcode, 'r') as original_gcode:
    for line in original_gcode:

        #Remove the carriage return that ends each line (for editing ease)
        line = line.replace('\n', '')

        #Get rid of anything involving temperature control
        if re.search('M104', line) or re.search('M106', line) or re.search('M109', line) \
           or re.search('M190', line) or re.search('M140', line):

            line = ''

        #Set power based on extruder
        if re.search('T0', line):
            line = 'M649 S' + str(POWER_1)

        if re.search('T1', line):
            line = "M649 S" + str(POWER_2)

        if re.search('T2', line):
            line = "M649 S" + str(POWER_3)



        #Fix XY motion commands

        if re.search('G1', line) and not(re.search('X', line) or re.search('Y', line) or re.search('Z', line)):
            #Only an extrude - no motion
            line = ''



         
        if re.search('G1', line): #this line involves XY motion

            if re.search('E[0-9.]+', line): #Extrude move

                               
                e_value = re.search('E[0-9.]+', line)
                e_capture = e_value.group(0) #Isolates the "E#.######"
                value_only = float(e_capture[1:]) #Remove the E and convert string to floating pt number

                if value_only < EXTRUDE_MIN or re.search('Layer', prevline): #This should actually be a G0 move
                    line = re.sub('E[0-9.]+', '', line) #Get rid of the extrude
                    line = re.sub('G1', 'G0', line)

                    if re.search('F[0-9.]+', line): #Speed specified
                        line = re.sub('F[0-9.]+','', line)
                        line = line + "F" + str(TRAVEL_SPEED)

                    else: 
                        line = line + "F" + str(TRAVEL_SPEED)


                else: #This is a valid extrude move
                    if re.search('F[0-9.]+', line): #Speed specified
                        line = re.sub('F[0-9.]+','', line)
                    
                    line = re.sub('E[0-9.]+', '', line) #Get rid of the extrude
                    line = line + "F" + str(SINTER_SPEED)

                
            else: #No extrude; G0
                line = re.sub('G1', 'G0', line)

        #Implement powder handling code
        if re.search('Z', line): #This line has a Z move
            line = ''
            #NewLayer()
            line = '''

;Begin distributing 
T0 ;Build platform active 
G0 E''' + str(3*LAYER_HEIGHT_SLIC3R) + ''' F200 
T1 ;Feed piston active 
G0 E''' + str(FEED_MULTIPLIER*LAYER_HEIGHT_SLIC3R) + ''' F200 
G0 X-242 Y38 F8000 

G0 X200 F8000 
M106 S''' + str(MOTOR_SPEED) + ''' 
G0 Z160 F''' + str(PLOW_SPEED) + ''' 
G0 Z20 F''' + str(PLOW_SPEED) + ''' 
M107 \n
;End distribution
'''


        prevline = line
        
        #Add back the carriage return
        line += '\n'
        Munged.write(line)
                

                

                

                
