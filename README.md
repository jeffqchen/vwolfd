# vwolfd - Taito Wolf System VxD driver for Windows 98

This is a VxD driver for the Taito Wolf System running Windows 98. It provides a hardware watchdog kicking service, as well as video output switching functionality through `DeviceIOControl` messages.

-------------

## Functionality

### JAMMA Output Switching

The Taito Wolf System arcade board shows a static splash screen when powered on. You need to feed values into `address 0xCB600` in order to switch it.

This VxD provides various DeviceIOControl message handling for switching between different outputs (Splash / Grid / Vodooo).

### Watchdog Kicking

The Taito Wolf System arcade board has a 3-pin output `Connector R` that connects to the PC motherboard's reset switch pins. It triggers every 64 seconds if the watchdog on the management CPLD is not kicked. This also resets the JAMMA output to the default splash screen.

By setting up an auto renewing watchdog kicking callback, the video switch state will be maintained.

### Early Watchdog Kicking

When system boots, during the real mode initialization stage `REAL_INIT`, the VxD will switch the JAMMA video output to calibration grid mode and kick the watchdog once, before the actual device initialization `DEVICE_INIT` stage, in order to prevent the auto-triggered system hard reset.

-------------

## Installation

Copy the file to the Windows directory and use one of the typical means to load a VxD.
- Adding a line in SYSTEM.INI
- Add an VxD entry in sytem registry.

I have provided an INF file to install the VxD alongside the Windows utility as a system device in the `TaitoUtl` project.

You can download the package over [Here](https://github.com/jeffqchen/TaitoUtl/releases)

-------------

## How To Build On Your Own

This code compiles under `Windows 98 Driver Development Kit`, aka `98DDK`, `version 5.0.2516.1900`.

Place all the files under `98DDK\src\vwolfd`.

Compile with the build command `build -cZ`. The compiled filed can be found under .\vwolfd\obj\i386

You can find Visuial Studio 6 Workspace files in the directory. I created them so you can open the project and view them in Visual Studio. The code cannot finish compiling in VS environment, but compiling individual files seem to work ok for spotting coding errors.

-------------

## Technical Details

The Taito Wolf System is controlled by the onboard CPLD U46. It's responsible of various funcctionalities of the system. This project invovled the video switching and hardware watchdog functionalities.

## Video Switching

The JAMMA edge video output of this board can be switched among 3 modes - Logo Splash / Test Grid / Voodoo

On power up, `U73 CPLD` will read the splash screen data on the `U71 EPROM` and present it on the video output on the JAMMA video output. The output will stay in this state unless `U46 CPLD` received certain commands on `address 0xCB600`.

From my experiments, odd numbers below 0x30 will trigger the splash screen, and even numbers below 0x30 will trigger the screen test grid screen.

When fed a value equal or greater than 0x30, the video output is switched to the 3Dfx Voodoo, which will show the actual 3D image generated. More specifically, the following values were used by the game code:
- `0x30`: During 3Dfx spinning logo
- `0x3D`: During normal gameplay
- `0x31`: During the test mode

## Watchdog

The arcade board has a built-in timer-based watchdog that kicks in every 64 seconds to reset the video output as well as triggering `Connector R`, which is connected to the reset pins to the PC motherboard below. In order to surpress the trigger, `address 0xCB200` needs to be fed SOMETHING freqently.

In the game code, this address is fed `0xFF` on every frame. However from the testing, it seems every few seconds is fine. Also considering not interfering with the actual game code, I settled with once per second. This value can be adjusted easily in the source code.

Curiously, `address 0xCB200` is also the 2P button data is read from. My guess is that it's not the value but the action of writing to this address that kicks the watchdog.
