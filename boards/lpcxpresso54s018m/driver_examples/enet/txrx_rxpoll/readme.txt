Overview
========

The enet_rxtx_rxinterrupt example shows the simplest way to use ENET functional tx/rx API for simple frame receive and transmit.

1. This example shows how to initialize the ENET.
2. How to use ENET to receive frame in polling and to transmit frame.

The example transmits 20 number broadcast frame, print the number and the mac address of 
the recieved frames.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- MCUXpresso  11.1.0
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso54S018M board
- Personal Computer
- RJ45 Network cable

Board settings
==============
For LPCXpresso54S018M V2.0:JP11, JP12, JP14 and JP15 1-2 on. 

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to a router (or other DHCP server capable device).
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
This demo is an external flash plain load demo, ROM will copy image in external flash to SRAMX to run:
1. Build the demo to generate a bin file.
   Note: If need to generate bin file using MCUXpresso IDE, below steps need to be followed:
         Set in example Properties->C/C++ Build->Settings->Build steps->Post-build steps->Edit
         enbable arm-none-eabi-objcopy -v -O binary "&{BuildArtifactFileName}" "&{BuildArtifactFileBaseName}.bin" 
         
         This plainload example linked the vector table to 0x00000000, but program to external flash 0x10000000.

2. Program the bin file to external on board flash via SEGGER J-FLASH Lite(V6.22 or higher):

   a. Open SEGGER J-FLASH Lite, select device LPC54S018M.

   b. Click the 'Erase Chip' to erase the extrenal flash.(if can not success, press SW4 button and reset the board, and try to erase again)

   c. Select the bin data file, set the '.bin/Erase Start' address to 0x10000000, then click 'Program Device'
Note: Please use above way to program the binary file built by armgcc tool chain to external flash. 
      For IAR, KEIL, MCUXpresso IDE, you can use the IDE tool to program the external flash.  
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ENET example start.


Transmission start now!
The 1-th frame transmitted success!
The 2-th frame transmitted success!
The 3-th frame transmitted success!
The 4-th frame transmitted success!
The 5-th frame transmitted success!
The 6-th frame transmitted success!
The 7-th frame transmitted success!
The 8-th frame transmitted success!
The 9-th frame transmitted success!
The 10-th frame transmitted success!
The 11-th frame transmitted success!
The 12-th frame transmitted success!
The 13-th frame transmitted success!
The 14-th frame transmitted success!
The 15-th frame transmitted success!
The 16-th frame transmitted success!
The 17-th frame transmitted success!
The 18-th frame transmitted success!
The 19-th frame transmitted success!
The 20-th frame transmitted success!
 One frame received. the length 346
 Dest Address ff:ff:ff:ff:ff:ff Src Address 28:f1: e:12:1f:10
 One frame received. the length 64
 Dest Address ff:ff:ff:ff:ff:ff Src Address 28:f1: e:12:1f:10
 One frame received. the length 64
 Dest Address ff:ff:ff:ff:ff:ff Src Address 28:f1: e:12:1f:10
...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

