Overview
========
The SDCARD FatFs FreeRTOS project is a demonstration program that uses the SDK software. It reads/writes
/erases the SD card continuously. The purpose of this example is to show how to use SDCARD driver
with FatFs and freeRTOS in SDK software to access SD card.

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
- SD card

Board settings
==============
Insert the card into the card slot

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Insert sdcard into sdcard.
5.  Reset the SoC and run the project.
Note:Do not try to use DATA3 to detect card, the demo board pull up the DATA3 pin, but DATA3 should be pull down when used as card detect pin.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SDCARD fatfs freertos example.

Card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......
TASK1: write file successed.
TASK1: file access is blocking.
TASK1: file access is blocking.
TASK1: file access is blocking.
TASK1: file access is blocking.
TASK2: write file successed.
TASK2: file access is blocking.
TASK2: file access is blocking.
TASK2: file access is blocking.
TASK2: file access is blocking.
TASK1: finsied.
TASK2: finsied.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

