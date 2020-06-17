# MSP432 Weather Station
A simple weather station for the MSP432.

## Behavior Description
When powered on, the system will display "Waiting for input…". Keys on the keypad can adjust this output to display different sensor data.

The '1' key will display the current humidity, and display it as a percentage on the LCD.
The '2' key will display the current temperature, and display it in degrees Celsius on the LCD.
The '3' key will display whether or not rain has been detected on the rain sensor module.
The '4' key will display current wind speed on the LCD.
Any other key will reset the device.

This device uses a DHT11 sensor to retrieve its humidity and temperature data. Its accuracy is as follows:
Humidity: ranges from 20-80%, 5% accuracy
Temperature: ranges from 0-50 degrees Celsius, 2 degrees accuracy
This device could have much better accuracy very easily -- if the user replaces the DHT11 sensor with a DHT22 (which uses the same wiring, input voltage and software that our DHT11 is running on) these ranges and accuracy ratings could be improved significantly. The DHT11 was selected due to its price for this project.

The device also uses a Raindrops module, which detects if rain is present on the circuit. This device will report specifically how much rain is on it and report it with an analog voltage, but our project will just indicate whether or not rain is present. This could easily be modified in software by adjusting a few lines of code, but it seemed more pertinent to know whether or not rain was detected at all, not how much rain was present on the sensor.

A Wind Sensor Rev. C is used to detect current wind speed. This device can measure up to 50 mph of wind. This sensor requires floating point operations, so when running, it will slightly slow down the weather station. These calculations will only be used if the '4' key is pressed, however, and therefore will not slow down any of the other functionality of the weather station.

On the attached LCD, proper data will be displayed based on the user’s request.

## Hardware Used
1. MSP432401R
2. DHT11/22 (see dht.h for connections to MSP432)
3. Raindrops Module (see raindrops.h for conecctions to MSP432)
4. Wind Sensor Rev. C (uses ADC, see adc.h for connections to MSP432)
5. BMP230 (not used in final project, but code is present in bmp230.c and bmp230.h)

## The Nitty Gritty (Software Description)
This project was essentially an excuse to interface multiple related sensors to the MSP432. 

The most challenging was the DHT module, as it required its own communication protocol with precise timing. This involved determining how long a pin was high, which indicated whether or not the transmitted bit was a 0 or a 1.

The Raindrops module was very straightforward, as it was just reading whether or not a pin was high. As mentioned in the Behavior Description, this could be expanded to determine exactly how much water is present on the sensor using its built-in analog out pin. This would be simple -- you would simply have to expand the ADC code to use another pin as an analog input for the sensor.

The Wind Sensor is just ADC. It's pretty simple. It's kind of cool how it works, though -- it heats up a resistor to a certain temperature, and it determines wind speed based on how much cooler the resistor is getting from where it should be. Of course, we can't take credit for that design, as we didn't build the sensor, but we felt it was a very creative way to determine wind speed.

The BMP230 uses SPI, but it's a tad bit more complicated, as the data it sends is in a special format. We do have code which works for it, but we chose to exclude it as our readings seemed off, and we felt the project was complete (enough) without including a pressure sensor.

#### Acknowledgements
Written by Michael Georgariou and Connor McKee

Written for Dr. John Oliver's Computer Architecture class
