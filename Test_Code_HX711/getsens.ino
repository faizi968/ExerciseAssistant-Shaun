void sens_data() {

  //mpu_data();
  weight_data();

  
  mydata = //"aX1"+String(aX1)+"aY1"+String(aY1)+"aZ1"+String(aZ1)+"gX1"+String(gX1)+"gY1"+String(gY1)+"gZ1"+String(gZ1)+
           //"aX2"+String(aX2)+"aY2"+String(aY2)+"aZ2"+String(aZ2)+"gX2"+String(gX2)+"gY2"+String(gY2)+"gZ2"+String(gZ2);
           "W1"+String(w1)+"W2"+String(w2);
  mydata.toCharArray(payload, 80);

  delay(500);

  
}

void mpu_data() {
  
  sensors_event_t accel1;
  sensors_event_t gyro1;
  sensors_event_t accel2;
  sensors_event_t gyro2;
    
  mpu1_accel->getEvent(&accel1);
  mpu1_gyro->getEvent(&gyro1);
  
  mpu2_accel->getEvent(&accel2);
  mpu2_gyro->getEvent(&gyro2);

  aX1 = accel1.acceleration.x;
  aY1 = accel1.acceleration.y;
  aZ1 = accel1.acceleration.z;
  gX1 = gyro1.gyro.x;
  gY1 = gyro1.gyro.y;
  gZ1 = gyro1.gyro.z;

  aX2 = accel2.acceleration.x;
  aY2 = accel2.acceleration.y;
  aZ2 = accel2.acceleration.z;
  gX2 = gyro2.gyro.x;
  gY2 = gyro2.gyro.y;
  gZ2 = gyro2.gyro.z;  
}

void weight_data() {
  
  w1 = scale1.get_units(10);
  Serial.print("Average 01:\t");
  Serial.println(w1);

  w2 = scale2.get_units(10);
  Serial.print("Average 02:\t");
  Serial.println(w2);

  scale1.power_down(); 
  scale2.power_down();             
  delay(1000);
  scale1.power_up();
  scale2.power_up();
  
}
