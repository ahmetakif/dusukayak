#include<Wire.h>
const int MPU2=0x69,MPU1=0x68;
int16_t AcX1,AcY1,AcZ1,Tmp1,GyX1,GyY1,GyZ1;
int16_t AcX2,AcY2,AcZ2,Tmp2,GyX2,GyY2,GyZ2;

//CONTRIBUTIONS BY AHMET AKIF KAYA
int limitangle = 60;
int limitside = 1; //1 being back, 0 being front
int minVal=265;
int maxVal=402;
double x1; 
double y1; 
double z1;
double x2; 
double y2; 
double z2;
const int ledpin = 13;
int ledstate = 0;
int warn1 = 0;
int warn2 = 0;
 
//-------------------------------------------------\setup loop\------------------------------------------------------------ 
 void setup(){ 
      Wire.begin(); 
      Wire.beginTransmission(MPU1);
      Wire.write(0x6B);// PWR_MGMT_1 register 
      Wire.write(0); // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);Wire.begin(); 
      Wire.beginTransmission(MPU2);
      Wire.write(0x6B);// PWR_MGMT_1 register 
      Wire.write(0); // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      Serial.begin(9600); 
      
      //CONTRIBUTIONS BY AHMET AKIF KAYA
      pinMode(ledpin,OUTPUT);
      digitalWrite(ledpin,LOW);
     } 
     
//---------------------------------------------------\void loop\------------------------------------------------------------
 void loop(){
   
      //get values for first mpu having address of 0x68   
      GetMpuValue1(MPU1);
      Serial.print("  ");
      Serial.print("|||");
      
      //get values for second mpu having address of 0x69
      GetMpuValue2(MPU2);
      Serial.println("");

      //CONTRIBUTIONS BY AHMET AKIF KAYA
      ledstate = diffcalc(z1,z2);
      digitalWrite(ledpin,ledstate);
    }
 
//----------------------------------------------\user defined functions\-------------------------------------------------- 
      
 
 void GetMpuValue1(const int MPU){ 
  
      Wire.beginTransmission(MPU); 
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) 
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 14, true); // request a total of 14 registers 
      AcX1=Wire.read()<<8| Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) 
      AcY1=Wire.read()<<8|  Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ1=Wire.read()<<8| Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L) 
      Tmp1=Wire.read()<<8| Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) 
      GyX1=Wire.read()<<8| Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L) 
      GyY1=Wire.read()<<8| Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L) 
      GyZ1=Wire.read()<<8| Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) 
      Serial.print("AcX = ");
      Serial.print(AcX1);
      Serial.print(" | AcY = "); 
      Serial.print(AcY1);
      Serial.print(" | AcZ = ");
      Serial.print(AcZ1); 
      Serial.print(" | GyX = ");
      Serial.print(GyX1); 
      Serial.print(" | GyY = "); 
      Serial.print(GyY1);
      Serial.print(" | GyZ = ");
      Serial.println(GyZ1); 

      //CONTRIBUTIONS BY AHMET AKIF KAYA
      int xAng1 = map(AcX1,minVal,maxVal,-90,90); 
      int yAng1 = map(AcY1,minVal,maxVal,-90,90); 
      int zAng1 = map(AcZ1,minVal,maxVal,-90,90);
      x1 = RAD_TO_DEG * (atan2(-yAng1, -zAng1)+PI); 
      y1 = RAD_TO_DEG * (atan2(-xAng1, -zAng1)+PI); 
      z1 = RAD_TO_DEG * (atan2(-yAng1, -xAng1)+PI);
      Serial.print(" | z1 = ");
      Serial.println(z1); 
     }
     
     
 void GetMpuValue2(const int MPU){ 
   
      Wire.beginTransmission(MPU); 
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) 
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 14, true); // request a total of 14 registers 
      AcX2=Wire.read()<<8| Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) 
      AcY2=Wire.read()<<8|  Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ2=Wire.read()<<8| Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L) 
      Tmp2=Wire.read()<<8| Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) 
      GyX2=Wire.read()<<8| Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L) 
      GyY2=Wire.read()<<8| Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L) 
      GyZ2=Wire.read()<<8| Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) 
      Serial.print("AcX = ");
      Serial.print(AcX2);
      Serial.print(" | AcY = "); 
      Serial.print(AcY2);
      Serial.print(" | AcZ = ");
      Serial.print(AcZ2);  
      Serial.print(" | GyX = ");
      Serial.print(GyX2); 
      Serial.print(" | GyY = "); 
      Serial.print(GyY2);
      Serial.print(" | GyZ = ");
      Serial.println(GyZ2); 

      //CONTRIBUTIONS BY AHMET AKIF KAYA
      int xAng2 = map(AcX2,minVal,maxVal,-90,90); 
      int yAng2 = map(AcY2,minVal,maxVal,-90,90); 
      int zAng2 = map(AcZ2,minVal,maxVal,-90,90);
      x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2)+PI); 
      y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2)+PI); 
      z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2)+PI);
      Serial.print(" | z2 = ");
      Serial.println(z2); 
     }

//CONTRIBUTIONS BY AHMET AKIF KAYA
  int diffcalc(int angle1,int angle2){
    int diff = z2 - z1;
    Serial.print(" | fark = ");
    Serial.println(diff);
    if (diff > limitangle){
      warn1 = 1;
      warn2 = 0;
    }
    else if (diff < -limitangle){
      warn1 = 0;
      warn2 = 1;
    }
    else{
      warn1 = 0;
      warn2 = 0;
    }
    if (limitside == 0){
      return warn1;
    }
    else{
      return warn2;
    }
  }
