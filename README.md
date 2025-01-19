# esp32-light

thing i made so that i dont have to stand up at night to turn off the lights \
it's just a servo mechanically attached to my wall switch. ESP32 connects \
through wifi and sets up a REST API along with a web interface at `cookie.local`. \
*no authentication!!!* what are they gonna do, turn my lights on and off?

### Usage
##### Firmware
this project uses [PlatformIO](https://platformio.org) \
simply run `pio run -t upload` with the ESP32 plugged in

##### Hardware
i just soldered on a WEMOS D1 Mini ESP32 to a protoboard. \
servo signal pin is soldered onto pin 33. i may or may not release a pcb \
in the future. i dont know. nobody except future me is reading this right?
