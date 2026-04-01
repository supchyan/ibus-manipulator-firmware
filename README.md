> [!IMPORTANT]
> This firmware is for Arduino UNO family.

### Assembly list
* Receiver FLYSKY FS-IA6B
* Transmitter FLYSKY FS-I6X
* Piranha UNO controller
* DFROBOT Power Shield V1.1
* Motor driver as Trema Module (iArduino) x5
* Trema set shield (iArduino) x1
* Linear actuator (V12) x4
* Gear motor (V12, 90RPM) x1

### Pre-requirements
* *READ README, PLEASE!*
* Install **[libraries](/libraries)**
* Set transmitter's channel 5 source as `SwC`
* Other channels have to be attached to their default sources.

### Additional info
iBus receiver sends data signals over RX0 pin, so make sure to plug or unplug it during/after/before flashing the firmware through serial.

6, 10 pins are reserved by a gear motor since only they can smoothly control transmitter's input in this firmware. FYI, other pins operates with a discrete input.

Trema modules' jumpers have to be set in `VIN` state to work with 12V input provided by a power shield.

If you use trema set shield to build the assembly, use the 1st field for a motor driver attached to a gear motor, 2nd field for an ibus receiver and other fields for motor drivers attached to linear actuators.

That's it 🐳


### Visual proof of concept
<image src=/poc.jpg width=500 height=auto />
