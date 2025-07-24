#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

// Create servo objects
Servo thumbServo, indexServo, middleServo, ringServo, pinkyServo, wristServo;

// Servo pin assignments
const int thumbPin  = 3;
const int indexPin  = 5;
const int middlePin = 6;
const int ringPin   = 9;
const int pinkyPin  = 10;
const int wristPin  = 11;

// MPU6050 instance
MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("❌ MPU6050 connection failed!");
    while (1);
  }

  // Attach all servos
  thumbServo.attach(thumbPin);
  indexServo.attach(indexPin);
  middleServo.attach(middlePin);
  ringServo.attach(ringPin);
  pinkyServo.attach(pinkyPin);
  wristServo.attach(wristPin);

  Serial.println("✅ Ready to receive angles...");
}

void loop() {
  // Read IMU roll value (optional for debug)
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float roll = gx / 131.0;  // Just for debug purposes
  Serial.print("IMU Roll (deg/sec): ");
  Serial.println(roll);

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int angles[6];

    // Read 6 comma-separated integers
    if (sscanf(data.c_str(), "%d,%d,%d,%d,%d,%d", &angles[0], &angles[1], &angles[2],
                                                    &angles[3], &angles[4], &angles[5]) == 6) {
      // Apply angles to servos
      thumbServo.write(angles[0]);
      indexServo.write(angles[1]);
      middleServo.write(angles[2]);
      ringServo.write(angles[3]);
      pinkyServo.write(angles[4]);
      wristServo.write(angles[5]);

      // Debug
      Serial.print("✅ Received: ");
      Serial.println(data);
    } else {
      Serial.println("❌ Invalid data received");
    }
  }

  delay(10); // small delay for stability
}
