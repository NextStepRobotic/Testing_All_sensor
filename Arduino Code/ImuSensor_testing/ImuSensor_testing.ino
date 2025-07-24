#include<Wire.h>
#include<MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 Ready!");
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw gyro to degrees/sec
  float gx_deg = gx / 131.0;
  float gy_deg = gy / 131.0;
  float gz_deg = gz / 131.0;

  Serial.print("Gyro (deg/sec): ");
  Serial.print("X=");
  Serial.print(gx_deg);
  Serial.print(" | Y=");
  Serial.print(gy_deg);
  Serial.print(" | Z=");
  Serial.println(gz_deg);

  delay(100);
}
