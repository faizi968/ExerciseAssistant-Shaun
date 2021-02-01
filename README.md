# ExerciseAssistant-Shaun
 Firmware Repository for a Smart Weight Assistant to enable user to precisely carry out activities and be aware of how much Calories they burned
## Instructions

These instructions assume you already have ESP32 BSP (Board Support installed on your Arduino IDE). If the ESP32 BSP is not installed, please refer to the "Library Installation" section below for assistance.

1. Start by cloning the repository to your drive.
2. Open the folder, Test_Code_MPU6050 and open the sketch "Test_Code_MPU6050.ino".
3. When the file opens, compile the sketch in Arduino IDE with "ESP32 Dev Module" selected as the Board. There are some libraries that need to be installed prior to compilation in order for the code to compile properly. Skip to "Library Installation" section for further info.
4. After successful compilation, build up the same circuit as in the sketch and afterwards, press the upload button with the ESP32 connected to the system via MicroUSB cable (remember to select the correct COM port on which the ESP32 is communicating).
When the code is uploaded, open serial monitor by navigating towards the top right icon or by pressing CTRL+SHIFT+M keys.
Observe the response and debugging information on the Serial Monitor.
5. For testing the load cell, open the sketch "Test_Code_HX711" and follow the steps above to see the response of the load cell.
6. Remember to put the calibration value calculated in load cell test in the set_scale() function in setup().
6. Use the nRF Connect App to view the data in your smartphone over BLE.

### Library Installation

#### ESP32 Board Support
1. In your Arduino IDE, go to File> Preferences
2. Enter the following link into the “Additional Board Manager URLs” field. Then, click the “OK” button
                       https://dl.espressif.com/dl/package_esp32_index.json
3. Go to Tools > Board > Boards Manager. This will open the Boards Manager.
4. Search for ESP32 and press install button for the “ESP32 by Espressif Systems“:
5. In case of confusion, please follow the link provided below for a detailed guide to setting up ESP32 BSP.
          https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
          
#### MPU-6050
1. Open your Arduino IDE and go to Sketch > Include Library > Manage Libraries. The Library Manager should open.
2. Type “adafruit mpu6050” on the search box and install the library.
3. Then, search for “Adafruit Unified Sensor”. Scroll all the way down to find the library and install it.
4. Finally, search for “Adafruit Bus IO” and install it.
5. After installing the libraries, restart your Arduino IDE. The MPU-6050 has now been installed into the Arduino IDE.

#### HX711 Load Cell
1. Clone the repository in .zip format from the following link. Remember to remove the "-master" from .zip file and the inside folder name. This may cause issues inside the IDE.
                  https://github.com/bogde/HX711
2. After successful cloning, open your Arduino IDE and go to Sketch > Include Library > Add .zip library. A browse windows will open.
3. Navigate to where you saved the .zip library file and double click on it to add it to your list of libraries.
4. Restart the Arduino IDE for changes to take effect.

### Load Cell Calibration
1. Upload the sketch "Load_Cell_Calibration" to the ESP32 and follow the steps below.
2. Open the serial monitor and wait for the IDE to show the message "Place a known weight on the load cell".
3. After placing the weight, wait for the value to show up on the serial monitor.
4. Divide the value shown on the Serial monitor with the known weight of the object in grams. We will call it "Calibration Factor".
5. Once again, run the code and this time, inside the set_scale() function, write the value of calibration factor calculated in step 4. For example,
   known weight = 200g
   value shown on monitor after first run = 600
   calibration factor = 600 / 200g = 3
   scale.set_scale(3);
6. Repeat from step 3 to step 5 until the serial monitor shows the correct weight of the object by changing the calibration factor. Note the value. This will be used in the load    cell test sketch.


