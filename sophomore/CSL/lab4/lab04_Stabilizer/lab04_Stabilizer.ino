/* 
 *  This project is based on "DMP without interrupt pin" by batata004
 *  see: https://forum.arduino.cc/t/good-news-dmp-from-mpu6050-can-be-used-without-interrupt-pin/393797
*/ 
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

// ================================================================
// ===                          PID                             ===
// ================================================================
float error;
float prev_error;
float kp = 0; //10
float ki = 0; //0.005
float kd = 0; //400
int dt = 10;
float setPoint = 0;
float P, I, D, PID;
float time;

// ================================================================
// ===                     DC Motor Control                     ===
// ================================================================
#define enA 9
#define in1 6
#define in2 7
int speed = 150;


void setup() {
  time = millis();
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  Wire.begin();
  TWBR = 24;
  mpu.initialize();
  mpu.dmpInitialize();
  // set the offsets here
  mpu.setXAccelOffset(475);
  mpu.setYAccelOffset(3296);
  mpu.setZAccelOffset(1409);
  mpu.setXGyroOffset(99);
  mpu.setYGyroOffset(-3);
  mpu.setZGyroOffset(47);
  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
  fifoCount = mpu.getFIFOCount();

  Serial.begin(115200);

}

void loop() {
  while (fifoCount < packetSize) {
    //Serial.println("HHHHHHHHHHHHHHHHHHHHHHHHHHHH");
    fifoCount = mpu.getFIFOCount();
  }
  
  if (fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  }
  else {
    if (fifoCount % packetSize != 0) {
      mpu.resetFIFO();
      fifoCount = 0;
    }
    else {
      while (fifoCount >= packetSize) {
        //Serial.println("crashed");
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
      }

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      Serial.print("ypr\t");
      Serial.print(ypr[0]*180/PI);
      Serial.print("\t");
      Serial.print(ypr[1]*180/PI);
      Serial.print("\t");
      Serial.print(ypr[2]*180/PI);
      //Serial.println();
      
      float currentTime = millis();
      if (currentTime > time + dt) {
        time = currentTime;
        
        error = (ypr[1] * 180 / M_PI) - setPoint;
        P = kp * error;
        I += ki * error;
//        I = (abs(error) < someThreshold) ? I + ki * error : 0;  <- Or you can set a threshold to only consider small erorrs
        D = kd * ((error - prev_error) / dt);
        PID = constrain(P + I + D, -90, 90);
        if (isnan(PID)) PID = 0;

        prev_error = error;
        speed = abs(PID);

        Serial.print("\t");
        Serial.print(PID);
        

        // check the direction
        if (PID < 0) {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
        } else {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
        }
        
        // send PWM signal
        analogWrite(enA, speed);
      }
      Serial.println();
    }
  }

}
