#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "HX711.h"


/***** BLE-Protocol Declaration *****/
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" // Defining Random 128bit UUID for Unique Identification of the Service.
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" // Defining Random 128bit UUID for Unique Identification of the Characteristic.


/***** Accelerometer Definitions *****/
#define SDA_1 21
#define SCL_1 22

#define SDA_2 33
#define SCL_2 32

/***** HX711 circuit wiring *****/
const int LOADCELL1_DOUT_PIN = 25;
const int LOADCELL1_SCK_PIN = 26;
const int LOADCELL2_DOUT_PIN = 12;
const int LOADCELL2_SCK_PIN = 14;

/***** Constructors for Sensors *****/
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

Adafruit_MPU6050 mpu1;
Adafruit_MPU6050 mpu2;
Adafruit_Sensor *mpu1_accel, *mpu1_gyro;
Adafruit_Sensor *mpu2_accel, *mpu2_gyro;

HX711 scale1;
HX711 scale2;

/***** Custom Variable for use in Program *****/
uint8_t sensorId;
float aX1=0, aY1=0, aZ1=0, gX1=0, gY1=0, gZ1=0, aX2=0, aY2=0, aZ2=0, gX2=0, gY2=0, gZ2=0;
String mydata = "";
char payload[80];

/***** Callback Function for BLE-Stack *****/
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void setup() {
  
  while(!Serial);
  Serial.begin(115200);
  delay(10);                                        // Delay for the Serial Console to properly boot-up
  Serial.println("started");

  /***** HX-711 Initializing and Calibration *****
  Serial.println("Initializing the scales");
  scale1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);

  Serial.println("Before setting up the scale 01:");
  Serial.print("read: \t\t");
  Serial.println(scale1.read());

  Serial.print("read average 01: \t\t");
  Serial.println(scale1.read_average(20));

  Serial.println("Before setting up the scale 02:");
  Serial.print("read: \t\t");
  Serial.println(scale2.read());

  Serial.print("read average 02: \t\t");
  Serial.println(scale2.read_average(20));

  scale1.set_scale(2280.f);                     
  scale1.tare();
  scale2.set_scale(2280.f);                     
  scale2.tare();

  Serial.print("read average 01: \t\t");
  Serial.println(scale1.read_average(20));

  Serial.print("get units 01: \t\t");
  Serial.println(scale1.get_units(5), 1);

  Serial.print("read average 02: \t\t");
  Serial.println(scale2.read_average(20));

  Serial.print("get units 02: \t\t");
  Serial.println(scale2.get_units(5), 1);

  Serial.println("HX711 Calibration Done"); /
  
  /***** MEMS Sensor Initialization *****/
  Wire.begin(SDA_1, SCL_1);
  Wire1.begin(SDA_2, SCL_2);

  bool status1 = mpu1.begin(0x68, &I2Cone);
  if (!status1) {
    Serial.println("Could not find a valid MPU_1 sensor, check wiring!");
  }
  
  bool status2 = mpu2.begin(0x68, &I2Ctwo);
  if (!status2) {
    Serial.println("Could not find a valid MPU_2 sensor, check wiring!");
  }

  /***** Get Module Detail from Sensors *****/
  if (status1) {
    mpu1_accel = mpu1.getAccelerometerSensor();
    mpu1_accel->printSensorDetails();
    mpu1_gyro = mpu1.getGyroSensor();
    mpu1_gyro->printSensorDetails();
  }

  if (status2) {
    mpu2_accel = mpu2.getAccelerometerSensor();
    mpu2_accel->printSensorDetails();
    mpu2_gyro = mpu2.getGyroSensor();
    mpu2_gyro->printSensorDetails();
  }
  /***** BLE-Stack Initialization *****/
  BLEDevice::init("ESP32_Shaun");
  
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(           // Definition contatinig Properties of the Characteristic
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
                    
  pCharacteristic->addDescriptor(new BLE2902());
  
  pService->start();                                              // Starting the BLE Service

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();                                  // Turning on the BLE Radio for Transmission
  Serial.println("Waiting for a client connection to notify..."); // Waiting until a BLE Slave / Client connects to the ESP32

}

void loop() {
  if (deviceConnected) {
        sens_data();
        pCharacteristic->setValue(payload);
        pCharacteristic->notify();
        delay(3); 
    }   
    
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); 
        pServer->startAdvertising(); 
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    
    if (deviceConnected && !oldDeviceConnected) {
    
        oldDeviceConnected = deviceConnected;
    }

}
