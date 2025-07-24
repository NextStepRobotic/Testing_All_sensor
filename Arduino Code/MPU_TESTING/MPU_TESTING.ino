#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize MPU6050
  mpu.initialize();
  
  // सेंसर कनेक्शन चेक करें
  if(mpu.testConnection()) {
    Serial.println("MPU6050 कनेक्शन सफल!");
  } else {
    Serial.println("MPU6050 नहीं मिला! वायरिंग चेक करें");
    while(1);
  }

  // सेटिंग्स कॉन्फिगर करें
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
}

void loop() {
  // रॉ डेटा पढ़ें
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // डेटा प्रिंट करें
  Serial.print("Accel: ");
  Serial.print("X="); Serial.print(ax);
  Serial.print(" Y="); Serial.print(ay);
  Serial.print(" Z="); Serial.print(az);
  
  Serial.print(" | Gyro: ");
  Serial.print("X="); Serial.print(gx);
  Serial.print(" Y="); Serial.print(gy);
  Serial.print(" Z="); Serial.println(gz);

  delay(500);
}
