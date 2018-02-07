# Airsoft Multi-Gamemode Prop
This repository contains the code and 3D design files to create an airsoft game prop to support a number of gamemodes

# TODO
* Move gamemodes into reusable classes
* Create a diagram detailing connections
* Export Fusion360 files
* Add Pin-Disarm Mode
* Add Dead man's Switch Mode
* List all parts required

# Required parts
* Generic Standard 20x4 LCD with PCF8574 Module
* 2 x Arcade buttons with backlight
* WS2811 LED Strip
* 4x4 Matrix keypad
* Arduino Mega
* TM1638 8-Digit Display Module
* Step-Down converter (or other 12V to 5V step down)
* Fuse and holder
* Small Speaker
* LM386 Amplifier Module

# Arduino Libraries Required
* [ButtonDebounce](https://github.com/maykon/ButtonDebounce)
* [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
* [TM1638](https://github.com/rjbatista/tm1638-library)
* [CountUpDownTimer](https://playground.arduino.cc/Main/CountUpDownTimer)
* [Keypad](https://playground.arduino.cc/Code/Keypad)
* [ArduinoMenu](https://github.com/neu-rah/ArduinoMenu)
* [LiquidCrystal_PCF8574](https://github.com/mathertel/LiquidCrystal_PCF8574)
* [ArduBoyTones](https://github.com/MLXXXp/ArduboyTones)
* [Optional] MemoryFree
