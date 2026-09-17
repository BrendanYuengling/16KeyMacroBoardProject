– Pico H Connection
Installed VSCode
Installed Raspberry Pi Pico Extension
Pico SDK 2.3.1


– Testing
Test 1 
Created new example project “blink”
Compiled default project
Connected Pico H while holding the BOOTSEL button (as stated in Pico H Documentation)
This allows for files to be uploaded to the Pico H
Uploaded the blink.uf2 compiled file
Pico H drive auto disconnected and led started flashing as intended
Unplugged the Pico’s USB cable
Changed the LED blink delay
Recompiled and uploaded the file as before
Observed the intended delay change on the Pico H board
Test 2
Using A Button, Two jumper wires and the Pico board
Tested the terminals of my button for continuity when the button is pressed
Used ohm meter, when pressed shows a resistance of 0, when not pressed shows an infinite resistance (represents an open circuit)
Connects GPIO6 to one terminal of the button
Connected the Ground (Pin 8) of the Pico board to the other terminal
Used buttonLEDtest.c
Observed when button was pressed, LED is on, When button is not press, LED is off

