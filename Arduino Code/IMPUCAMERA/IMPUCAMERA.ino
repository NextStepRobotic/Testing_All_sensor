#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;

// Servo declarations
Servo thumbServo, indexServo, middleServo, ringServo, pinkyServo, wrist;

// Servo pin assignments
const int thumbPin  = 3;
const int indexPin  = 5;
const int middlePin = 6;
const int ringPin   = 9;
const int pinkyPin  = 10;
const int wristServoPin = 11;

// Forearm twist rotation
float twistTotal = 0; // Accumulated twist angle

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
  wrist.attach(wristServoPin);

  Serial.println("✅ MPU6050 & Servos Ready!");
}

void loop() {
  // === Forearm twist from MPU6050 ===
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float twistSpeed = gx / 131.0; // deg/sec (forearm twist speed)
  twistTotal += twistSpeed * 0.01; // integrate over time (~10ms loop)
  twistTotal = constrain(twistTotal, 0, 20); // Adjusted range to 0 to 20°

  // Apply custom mapping formula:
  // 20° → 180
  // 10° → 90
  // 5° → 45
  // 4° → 36
  int wristAngle = twistTotal * 9.0; // scale factor based on 20° → 180
  wristAngle = constrain(wristAngle, 0, 180);
  wrist.write(wristAngle);

  // === Finger control from Python Serial ===
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // Read serial data
    int angles[5];  // Array for 5 finger servo angles

    if (sscanf(data.c_str(), "%d,%d,%d,%d,%d", &angles[0], &angles[1], &angles[2], &angles[3], &angles[4]) == 5) {
      // Reverse angles before writing (optional)
      thumbServo.write(180 - angles[0]);
      indexServo.write(180 - angles[1]);
      middleServo.write(180 - angles[2]);
      ringServo.write(180 - angles[3]);
      pinkyServo.write(180 - angles[4]);

      // Debug
      Serial.print("Received Fingers (Reversed): ");
      Serial.print(180 - angles[0]); Serial.print(", ");
      Serial.print(180 - angles[1]); Serial.print(", ");
      Serial.print(180 - angles[2]); Serial.print(", ");
      Serial.print(180 - angles[3]); Serial.print(", ");
      Serial.println(180 - angles[4]);
    } else {
      Serial.println("Invalid Data Received!");
    }
  }

  // Debug wrist
  Serial.print("Twist°: "); Serial.print(twistTotal);
  Serial.print(" | Wrist Servo°: "); Serial.println(wristAngle);

  delay(10);
}
