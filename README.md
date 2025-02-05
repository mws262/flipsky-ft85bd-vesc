# VESC on Flipsky FT85BD

**TL;DR - You can run VESC firmware on the FT85BD, but probably only if you replace the microcontrollers on the board or create a barebones version of the VESC firmware.**

This dual motor ESC uses Flipsky’s own closed-source firmware and PC tuning software. Given available features and wiring, I’m pretty sure their firmware is based on VESC. They probably made changes in order to:

1. Support extra outputs, e.g. lights, and inputs like “gearing” levels
2. Use cheaper MCU components (more on this later)
3.  Possibly to lock people into using Flipsky products once they become accustomed to the software.

### Can the FT85BD run normal VESC firmware and the VESC Tool?

Kinda. I will warn you that it probably isn’t worth the effort. I’m in too deep at this point, and y’all deserve instructions or at least a warning. Be sure to look at the “What’s the catch?” part before committing. 

### Configuring the VESC firmware properly

The architecture and pins going to the microcontroller are similar to other VESC-compatible boards. I am including a board config that maps the pins correctly. There are small pads (no connectors) near each motor controller for the VCC, SWDIO, SWCLK, and GND pins. You can solder to those pads and use an STLINK programmer to flash the MCUs. So, you’d clone the VESC firmware repo, put my config files in the hwconf/flipsky directory, build and flash with the following:

```jsx
make fw_ft85bd
make fw_ft85bd_flash
```

Do this for both microcontrollers on the board. The same config should be adequate for both. The config.h file lists some of the other connections that I haven’t implemented. There are several MOSFETs for controlling accessories. I don’t know the best way to use those within the VESC framework (maybe a custom user app?). Let me know if you know some simple method. 

### What’s the catch?

The FT85BD uses an STM32 knockoff, the AT32F403ARCT7. The AT32F403 has the same pinout as the STM32F405RGT6 that the normal VESCs use. The biggest problem is that the AT32F403 has substantially less flash memory. The VESC binary won’t fit in 256Kb. As far as I know, this leaves two options:

1. **(What I did) Change the MCUs out**. Use a hot air rework station to desolder both AT32F403 from the motor controller, clean the contacts, apply a bit of solder paste, and replace with STM32F405 MCUs. If you’ve never soldered Tsomething that small, I don’t recommend trying it.
2. **(What I ultimately didn’t do)** T**rim the firmware down.** First you should see if you can get the VESC firmware binary small enough. Strip out as many drivers and communication protocols, etc. as you can. I got frustrated and stopped. Assuming you succeed, the AT32F403 can be flashed (via STLINK) by installing the OpenOCD config from the manufacturer: https://github.com/ArteryTek/OpenOCD. You’ll need to figure out file paths, but it’s something like this:
    
    ```jsx
    artery-openocd -f ~/artery-openocd/tcl/interface/stlink.cfg -f ~/artery-openocd/tcl/target/at32f4xx.cfg
    ```
    
    If OpenOCD connects properly, you can:
    
    ```jsx
    telnet localhost 4444
    ```
    
    in another terminal. The MCUs have write protection enabled, so you’ll need to disable (using telnet):
    
    ```jsx
    reset halt
    at32f4xx disable_access_protection 0
    reset
    ```
    
    and finally flash the firmware: 
    
    ```jsx
    flash write_image erase /path/to/vesc_firmware.bin 0x08000000
    reset run
    exit
    ```
    
    I can’t guarantee that it will work even if you do all this, but I figured I’d describe my progress in case someone else goes down this road.
    
3. **(What I ultimately didn’t do)** T**rim the firmware down.** First you should see if you can get the VESC firmware binary small enough. Strip out as many drivers and communication protocols, etc. as you can. I got frustrated and stopped. Assuming you succeed, the AT32F403 can be flashed (via STLINK) by installing the OpenOCD config from the manufacturer: . You’ll need to figure out file paths, but it’s something like this:
    
    ```jsx
    artery-openocd -f ~/artery-openocd/tcl/interface/stlink.cfg -f ~/artery-openocd/tcl/target/at32f4xx.cfg
    ```
    
    If OpenOCD connects properly, you can:
    
    ```jsx
    telnet localhost 4444
    ```
    
    in another terminal. The MCUs have write protection enabled, so you’ll need to disable (using telnet):
    
    ```jsx
    reset halt
    at32f4xx disable_access_protection 0
    reset
    ```
    
    and finally flash the firmware: 
    
    ```jsx
    flash write_image erase /path/to/vesc_firmware.bin 0x08000000
    reset run
    exit
    ```
    
    I can’t guarantee that it will work even if you do all this, but I figured I’d describe my progress in case someone else goes down this road.