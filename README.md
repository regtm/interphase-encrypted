# Interphase Keyboard
The [interphase](https://github.com/Durburz/interphase) keyboard is an iso layout adaptation of reversebias' [mitosis](https://github.com/reversebias/mitosis-hardware) keyboard by durburz.

# Interphase Encrypted Keyboard Firmware
Firmware for Nordic MCUs used in the Interphase Keyboard, contains precompiled .hex files, as well as sources buildable with the Nordic SDK.
This firmware adapts rossica's [crypto](https://github.com/rossica/mitosis/tree/feature-crypto) firmware for the original mitosis for the use with the interphase keyboard.

## Install dependencies

Tested on Ubuntu 20.04.3, but should be able to find alternatives on all distros. 

```
sudo apt install openocd gcc-arm-none-eabi
```

## Download Nordic SDK

Nordic does not allow redistribution of their SDK or components, so download and extract from their site:

https://www.nordicsemi.com/eng/nordic/Products/nRF5-SDK/nRF5-SDK-v12-zip/54291

Firmware written and tested with version 11

```
unzip nRF5_SDK_11.0.0_89a8197.zip  -d nRF5_SDK_11
cd nRF5_SDK_11
```

## Toolchain set-up

A cofiguration file that came with the SDK needs to be changed. Assuming you installed gcc-arm with apt, the compiler root path needs to be changed in /components/toolchain/gcc/Makefile.posix, the line:
```
GNU_INSTALL_ROOT := /usr/local/gcc-arm-none-eabi-4_9-2015q1
```
Replaced with:
```
GNU_INSTALL_ROOT := /usr/
```

## Clone repository
Inside nRF5_SDK_11/
```
git clone https://github.com/reversebias/mitosis
```

## Install udev rules
```
sudo cp mitosis/49-stlinkv2.rules /etc/udev/rules.d/
```
Plug in, or replug in the programmer after this.

## OpenOCD server
The programming header on the side of the interphase keyboard, from top to bottom:
```
3.3v
swclck
swdio
gnd
```
It's best to remove the battery during long sessions of debugging, as charging non-rechargeable lithium batteries isn't recommended.

Launch a debugging session with:
```
openocd -f mitosis/nrf-stlinkv2.cfg
```
Should give you an output ending in:
```
Info : nrf51.cpu: hardware has 4 breakpoints, 2 watchpoints
```
Otherwise you likely have a loose or wrong wire.


## Manual programming
From the factory, these chips need to be erased:
```
echo reset halt | telnet localhost 4444
echo nrf51 mass_erase | telnet localhost 4444
```
From there, the precompiled binaries can be loaded:
```
echo reset halt | telnet localhost 4444
echo flash write_image `readlink -f precompiled-basic-left.hex` | telnet localhost 4444
echo reset | telnet localhost 4444
```

## Automatic make and programming scripts
To use the automatic build scripts:
```
cd mitosis/mitosis-keyboard-basic
./program.sh
```
An openocd session should be running in another terminal, as this script sends commands to it.

## Programming Pro Micro Receiver
The precompiled qmk hex file can be flashed with avrdude. The path to the hex and the usb port may have to be adapted.
```
avrdude -p atmega32u4 -c avr109 -U flash:w:precompiled-basic-qmk.hex -P /dev/ttyACM0
```



















