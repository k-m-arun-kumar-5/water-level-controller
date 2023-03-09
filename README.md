Description :
============
I, K.M. Arun Kumar alias Arunkumar Murugeswaran, just shared my works, which I worked as learning path and practiced Embedded C programming using LPC2138 (ARM7TDMI-S 32 bit RISC microcontroller) such as overhead water flow controller using level sensors. 

Design
======
Monitor water level in a overhead tank by using water level sensors at positioned at represent 0%, 20%, 40%, 60%, 80% & 100% water filled level in overhead tank. Mode of operation is selected by a SPST(single pole single throw switch)(MODE_SW). FUNC_SW is a push button switch. Water level in overhead controller is processed by LPC2138, in our case, RESET_SW is a push button switch, to reset the Water level controller (in our case, LPC2138). If mode of operation is automatic, then if total water level in overhead tank is below a minimum preset water filled level (in %), then water pump is automatically turned ON. Either if mode of operation is Manual or Automatic, if total water level in overhead water tank is above 99%, then water pump is automatically turned OFF. If mode of operation is Manual, then water pump can be manually turned ON or OFF, by using a FUNC_SW. If mode of operation is Manual and if water pump is ON, then by pressing FUNC_SW, water pump is turned OFF, and if water pump is turned OFF and total water level in overhead tank is below 80%, then by pressing FUNC_SW, water pump is turned ON. If mode of operation is automatic, then if preset minimum water filled level in overhead tank is <= 20% and Water pump is turned OFF, then for each FUNC_SW press, then preset minimum of total water filled level in overhead tank (in %) is increased by 1% of total water level filled in overhead tank, till the preset minimum of total water filled level in overhead tank < 80%. If preset minimum of total water filled level in overhead tank (in %) > 80% and then for press of FUNC_SW, then preset minimum of total water filled level in overhead tank is set to 20% and for each FUNC_SW press, then preset minimum water filled level in the overhead water tank is increased by 1% of water level filled in overhead tank. When the water pump is turned ON, then  water filled level in overhead tank (in %) must reach a next water level filled sensor by a within preset timeout, to detect and prevent water pump from dry run. Eg. If water pump is turned ON and if total of water filled level >= 20% and < 40%, then total water filled level in overhead tank should reach 40%, before a preset timeout for water level filled between next sensor is reached. If water pump is turned ON and when total of water filled level in overhead tank (in %) == 40%, then timer is automatically reset and timer runs for preset timeout for total of water filled level to reach next sensor. If timer timeouts for preset timeout for water filled level to reach next sensor with one method to detect and prevent water pump from dry run, then water pump is automatically turned OFF. Whenever setting in preset water level and preset water level setting is automatically locked due to preset timeout or whenever change in 
operation mode, whenever water pump is triggered on or off state or due to press of RESET_SW.    

CAUTION:
========
Schematics and simulation is done by Proteus CAD. NOT EXPERIMENTED IN REAL TIME ENVIRONMENT.

Purpose :
=========
In all my respective repositories, I just shared my works that I worked as the learning path and practiced, with designed, developed, implemented, simulated and tested, including some projects, assignments, documentations and all other related files and some programming that might not being implement, not being completed, lacks some features or have some bugs. Purpose of all my repositories, if used, can be used for LEARNING AND EDUCATIONAL PURPOSE ONLY. It can be used as the open source and freeware. Kindly read the LICENSE.txt for license, terms and conditions about the use of source codes, binaries, documentation and all other files, located in all my repositories. 
       
My Thanks and Tribute :
========================

I thank to my family, Friends, Teachers, People behind the toolchains and references that I used, all those who directly or indirectly supported me and/or helped me and/or my family, Nature and God. My tribute to my family, Friends, Teachers, People behind the toolchains and references that I used, Nature, Jimmy Dog, God and all those, who directly or indirectly help and/or support me and/or my family.

Toolchains that I used for LPC2138 Application design and development are as follows :
======================================================================================
1: IDE for Application development of LPC2138                               - MDK-ARM Professional Keil uvision 5. 
2: PC tool for Programming Flash of LPC2138                                 - Flash Magic.
3: LCD                                                                      - JHD162A.
4: Desktop Computer Architecture and OS for development                     - Intel X64 & Windows 7 (32 bit).
5: CAD and Simulations                                                      - Proteus 8.3 Pro SP2 and/or Proteus 8.11 SP1. 
6: Code editor                                                              - Notepad++.
7: Documentation                                                            - Microsoft Office 2007 (alternative LibreOffice) and Text Editor. 

Some reference that I refered for LPC2138 Application design and development, are as follows :
==============================================================================================
1: Schaum's Outline of Programming with C, 2nd Edition - Book authored by Byron Gottfried.
2: Understanding and Using C Pointers: Core Techniques for Memory Management - Book authored by Richard M. Reese. 
3: Embedded C - Book authored by Michael J. Pont.
4: Reusable Firmware Development: A Practical Approach to APIs, HALs and Drivers - Book authored by Jacob Beningo. 
5: https://www.embedded.com/building-reusable-device-drivers-for-microcontrollers/ - Building reusable device drivers for microcontrollers.
6: https://www.nxp.com/docs/en/data-sheet/LPC2131_32_34_36_38.pdf - LPC2131/32/34/36/38 - Single-chip 16/32-bit microcontrollers; 32/64/128/256/512 kB ISP/IAP flash with 10-bit ADC and DAC,Rev. 5.1 — 29 July 2011 Product data sheet. 
7: Hitachi HD44780U - LCD product data sheet.
8: http://www.electronicwings.com/arm7  - ARM 7 Inside - LPC2148.


Note :
======
Kindly read in the source codes, if mentioned, about the Program Description or Purpose, Known Bugs, Caution & Notes and Documentations. Some applications are also tested in a LPC2138 development board.

My Repositories Web Link :
==========================
https://github.com/k-m-arun-kumar-5

