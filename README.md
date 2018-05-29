# Telia IoT Workshop
Welcome to the Telia IoT Workshop! Today we will learn some basic electronics and create a wifi-based temperature and humidity sensor.

Learn more about IoT in Telia at https://telia.no/iot and https://telia.io.

## Electric Safety
* The kit used in this workshop operates at 5 Volts and below. This means there is no risk for electric shocks, however incorrect use may still break the components themselves.
* Do not put the electronics on conducting surfaces such as metal laptops, powerbanks or other things made from metal.
* Always disconnect power before changing connections.
* Double check connections before connecting power to avoid breaking the components.

## 1. Install software
<img src=img/hardware.jpg width=400px>

The NodeMCU is a circuit board containing the popular ESP8266 module with built-in wifi connectivity, compatible with the popular [Arduino echosystem](https://www.arduino.cc/en/Guide/Introduction). To program it, we need some software on our computer.

* Install the latest version of the Arduino IDE for you computer: https://www.arduino.cc/en/Main/Software
* Install NodeMCU CP201x USB Driver
	* [OS X](http://www.silabs.com/Support%20Documents/Software/Mac_OSX_VCP_Driver.zip)
	* [Windows](http://www.silabs.com/Support%20Documents/Software/CP210x_Windows_Drivers.zip)
	* [Linux](http://www.silabs.com/Support%20Documents/Software/Linux_3.x.x_VCP_Driver_Source.zip)
	* [Other](http://www.silabs.com/products/mcu/pages/usbtouartbridgevcpdrivers.aspx)
* Now, we need to install support for the ESP8266 module. Open up your newly installed Arduino software and click `Arduino > Preferences...`. Insert the following URL in the box titled `Additional Boards Manager URLs`:

	```
	http://arduino.esp8266.com/stable/package_esp8266com_index.json
	```
	<img src=img/preferences.png width=500px>
* Go to `Tools > Board > Boards Manager`. Search for "esp8266", click on `esp8266 by ESP8266 Community` and then `Install`. 

	<img src=img/board.png width=500px>


## 2. Blink a light
Make sure everything is working by running a simple example.

* Click `File > Examples > Basics > Blink`. This should open up a new window with some code.
* Tell the computer what board we are using by clicking `Tools > Board > NodeMCU 1.0`
* Connect the NodeMCU to a USB port. Be careful to not place the board on metal as the pins may short out and break the board.
* Click `Tools > Port` and select the port. On Mac it is called `/dev/cu.SLAB_USBtoUART`. On Windows it is something like `COM3`, the number may vary. 
* Press the upload button <img src=img/upload.png width=20px> to compile and send the program to the NodeMCU.
* You should now see a blue LED flashing rapidly on the opposite side from the USB connector. It is indicating data being transferred as the program is uploaded to the board. 

After uploading, the blue "programmable" LED closer to the USB connector should start blinking once per second.

If uploading the code fails, try pushing the `RST` button beside the USB connector right after clicking the upload button.

**Bonus:** try changing the delays and adding in more lines to create other blinking patterns.


## 3. Print a message
It is sometimes helpful to print out a message to yourself to figure out how the code works. Try out this program and see if you understand how it works. Create a new file in the Arduino software and paste the code below, then press upload <img src=img/upload.png width=20px>. After uploading, open the serial monitor (button on the top right).

```c
int messageNumber = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting program");
}

void loop() {
  Serial.println("This is message number " + String(messageNumber));
  messageNumber++;
  delay(1000);
}
```

<img src="img/counter.png" width=500px>

You should see a window with text messages appearing once per second:

<img src="img/serial_monitor.png" width=500px>


**Bonus:** try adding another line with ```SerialUSB.print("Hi! ");``` and upload again. See the difference between ```print``` and ```println```?

## 4. Get connected
The hardware is working, now let's make it talk to the Internet! For today, we will use an existing IoT platform to play with the data, but it could of course also be sent directly to your own app or website.

* Go to https://maker.allthingstalk.com and sign up for an account.
* Log in, press `Connect a device` and select `WIFI/LAN devices`, then `Arduino`.

	<img src="img/new_device.png" width=500px>

* Each device can have many different sensors, or "assets". Add a new asset to your device by clicking `Create asset`. Call the asset `counter` (important!) and set the type to `Integer`.

	<img src="img/new_asset.png" width=400px>

	You should now end up at you device page with one asset called "counter".

	<img src="img/devices.png" width=800px>

	As you can see, your brand new counter has the value "--". Let's fix that!

* You now need some code libraries and examples from AllThingsTalk. Install their SDK by following the guide at: 
  
  http://docs.allthingstalk.com/developers/sdk/arduino/

* Download the code in this repository by clicking the link below:

  https://github.com/TeliaSoneraNorge/telia-iot-workshop-wifi/archive/master.zip
  

* In Arduino, click "Sketch", "Include library", "Add .ZIP library" and coose the file you just downloaded.
-->
* Open the example by clicking `File  > Examples > Telia-AllThingsTalk > AllThingsTalk_counter`
	<img src="img/example.png" width=500px>
* In the code, replace `XXXXXX` with proper values in the two variables at the top.
  
  
	```c
	// AllThingsTalk device credentials
	char deviceId[] = "XXXXXX";
	char token[] = "maker:XXXXXXXXXXXXXX";
	```


	Device ID and token can be found in AllThingsTalk. Go back to your device page and click `Settings` in the top right corner, then `Authentication`.

	<img src="img/authentication.png" width=700px>

* Upload the code to the device, then go to the website to see your data updating! **Note:** it may take up to a minute before the data appears the first time. You can check the status in the serial monitor.

**Bonus:** Try setting up boolean, number and string assets in AllThingsTalk and see if you can send in other types of data. All assets must have different names, but they can be updated the same way in the code.

## 5. Read sensors

The Sodaq NB-IoT shield not only has connectivity, it also has several sensors;

* HTS221 temperature and humidity sensor
* LPS22HB air pressure sensor
* LSM303C accelerometer, compass and temperature sensor
* Ublox SAM-M8Q GPS

There are examples for how to use each of these in the examples menu where we found the counter example earlier. Let´s start by reading the temperature from the HTS221 and send it to AllThingsTalk.

* Open the example code by clicking "File", "Examples", "Telia-AllThingsTalk", "HTS221_HumidityTemperature". Upload it and check that you see temperature readings in the serial monitor.
* Create a new asset in AllThingsTalk with the name "temperature" and the type "Number"
	<img src="img/temperature.png" width=300px>
* Copy in the relevant parts of the code from the previous example into the new one. Insert the device id, token and server information like before but use the new asset.

* Upload the code and check out your new weather station of the future!

**Bonus:** Cerate a pinboard in AllThingsTalk and set up a graph or a gauge for you sensor

## 6. Do your thing
That's it, you are now a certified IoT hardware maker! Go ahead and think of a project you would like to make. You can also add more sensors, lights or switches if you like. Here are some suggestions:

* Weather station - read all the air-related sensors and publish on a dashboard.
* Movement alarm - read the accelerometer values and send a message if it changes. Bonus: also add a PIR motion sensor.
* Compass - read the compass in the IMU and make a graph that plots where the device is headed.

General hints:
* Note that the GPS probably will not work indoors, but you can of course try if you like.
* Try not to write all the code at once, start simple and check that it works along the way.
* If you want to connect more things to the Crowduino, note that most pins are already in use by the NB-IoT shield. Available pins are D1, D2, D8-D12 and A0-A3. See the full schematic here:
	http://support.sodaq.com/wp-content/uploads/2017/02/nb-iot_shield_rev3b_sch-1.pdf


## Sources

To make all the needed libraries available with a single install, this repository contains software for the Sodaq NB-IoT shield borrowed from Sodaq's own examples and a copy of the Sparkfun IMU library. Some modifications have been made to make the examples work on the Crowduino. The original repositories can be found here:

https://github.com/janvl1234/Sodaq_NBIoT_examples

https://github.com/sparkfun/SparkFun_LSM303C_6_DOF_IMU_Breakout_Arduino_Library
