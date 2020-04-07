Overview
========
The IAP project is a simple demonstration program of the SDK IAP
driver. It reads part id, boot code version, unique id and reinvoke ISP. A message
a printed on the UART terminal as various bascial iap operations are performed.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- MCUXpresso  11.1.0
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54S018M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.

Running the demo
================
1.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.

IAP example

PartID:	XXXXX

The major version is:	XXXX

The minor version is:	XXXX

Unique ID:	XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

End of IAP Example 
Customization options
=====================

