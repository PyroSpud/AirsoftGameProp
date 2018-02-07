# Airsoft Multi-Gamemode Prop
This repository contains the code and 3D design files to create an airsoft game prop to support a number of gamemodes

# TODO
* Move gamemodes into reusable classes
* Export Fusion360 files
* Add Pin-Disarm Mode
* Add Dead man's Switch Mode
* Add support for an ESP8266 WiFi Sub-Board
	* Allow remote viewing of status

# Required parts

Displays:
* TM1638 8 Digit Display Module
	* Measurements: 20mmx120mm
	* ["Jy-MCU" Branded, eBay UK](https://www.ebay.co.uk/itm/JY-MCU-8-bit-digital-display-module-Suitable-for-51-avr-arduino-program-cable/312027693063)
* Generic 20x4 LCD with PCF8574 Module
	* Sometimes described as an "i2c LCD"
	* Available in various colours
	* [eBay UK](https://www.ebay.co.uk/itm/With-Without-IIC-I2C-2004-20X4-Character-LCD-Module-Display-Blue-Green-Arduino/263128525818)
* WS2811 LED Strip
	* 60LEDs/M Preferred for greater light output
	* Available in 12V and 5V Variants, get 12V
	* [eBay UK](https://www.ebay.co.uk/itm/WS2811-5050-RGB-Full-color-Addressable-Strip-30-48-60-LED-M-IP30-IP67-12V/152112790851)
	
Switches:
* 4x4 Matrix keypad
	* [eBay UK](https://www.ebay.co.uk/itm/4x4-3x4-Matrix-Array-16-12-Keys-Switch-Keypad-Keyboard-Module-for-Arduino-2178/202150023141)
* 2 x Arcade buttons with backlight
	* LED Backlit, Usually with integrated resistor for 12V
	* [eBay UK](https://www.ebay.co.uk/itm/1X-round-lit-illuminated-arcade-video-game-push-button-switch-LED-light-lamp-GH/292344774086)

Modules:
* LM386 Amplifier
	* Can be pwered from either 5V or 12V
	* [eBay UK](https://www.ebay.co.uk/itm/LM386-Audio-Amplifier-Module-5V-12V-With-Adjustable-Volume/132319902056)
* Arduino Mega 2560
	* Can be sourced easily from anywhere
* Buck Converter (or other 12V to 5V step down)
	* LM2596 Modules readily available
	* Be sure to trim the correct voltage before connecting the outputs
	* [eBay UK](https://www.ebay.co.uk/itm/LM2596-DC-DC-Buck-Converter-Adjustable-Power-Supply-Step-Down-Module-Multi/263022838841)	

Misc. Parts:	
* Fuse and holder
	* An inline fuse with the battery terminal, automotive fuses work well
* Small Speaker
* Battery Connector (XT60/Deans/Tamiya Etc.)
* Crimp spade connectors (For arcade switches)
	* You could also solder the connections
* Wire
	
Hardware:
* M3 Bolts, Screws and Nylon Locknuts
* M4 Threaded rod (Cut to length to hold enclosure together)
* M4 Washers

Please note, links are not n endorsement of the associated seller. The links are merely an example of the part.

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
