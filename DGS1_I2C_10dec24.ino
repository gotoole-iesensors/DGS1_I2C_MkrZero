#include "Wire.h"

#define Timed_Initial_Zero  
#define Timed_Initial_Zero_Period 3600	//Zero set after 3600s
#define Continous_Output_Period   60		//Continuous output interval of 60s

//#define I2C_DEV_ADDR 0x31 //H2
#define I2C_DEV_ADDR 0x32 //CO
//#define I2C_DEV_ADDR 0x33 //EtOH
//#define I2C_DEV_ADDR 0x34 //H2S
//#define I2C_DEV_ADDR 0x35 //O3
//#define I2C_DEV_ADDR 0x36 //Cl
//#define I2C_DEV_ADDR 0x37 //NO2
//#define I2C_DEV_ADDR 0x38 //SO2
//#define I2C_DEV_ADDR 0x39 //C2H4
//#define I2C_DEV_ADDR 0x3A //NO
//#define I2C_DEV_ADDR 0x3B //IAQ
//#define I2C_DEV_ADDR 0x3C //HCHO
//#define I2C_DEV_ADDR 0x3D //RESP

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  delay(3000);

  Serial.println("\r\r\r\r\r\r\r\r");
  Serial.println("*****************************************************");
  Serial.print("Continous output set at: ");
  Serial.print(Continous_Output_Period);
  Serial.println("s");
#ifdef Timed_Initial_Zero
  Serial.print("Timed initial zero set at: ");
  Serial.print(Timed_Initial_Zero_Period);
  Serial.println("s");
#endif
  Serial.println("*****************************************************");
  Serial.println("Serial, PPB, Tx100, Hx100, ADC_Raw, T_Raw, H_Raw"); 
}

void loop() 
{
  unsigned int i;
  
  char byte_received[32];
  char checksum;

  char Serial_Number[13];
  long int PPB;
  int Tx100;
  unsigned int Hx100;
  unsigned int ADC_Raw = 32768;
  unsigned int T_Raw = 24500;
  unsigned int H_Raw = 25500;

  unsigned long int LastMillis;
  bool Serial_Number_MSB_Read_Success;
  bool Serial_Number_LSB_Read_Success;
  bool PPB_Read_Success;
  bool ADC_Read_Success;
  bool Zero_Success;
  bool Zero_Timer_Triggered = false;

  LastMillis = millis();
  while(1)
  {
      Serial_Number_MSB_Read_Success = false;
      while(!Serial_Number_MSB_Read_Success)
      {    
        Wire.beginTransmission(I2C_DEV_ADDR);
        Wire.write('0'); 
        Wire.endTransmission();
        delay(50);
        Wire.requestFrom(I2C_DEV_ADDR, 12, true); //Request 12 bytes
        i = 0;
        while(Wire.available()) 
        {
            byte_received[i] = Wire.read(); //Receive byte      
            i++;
        }
        checksum = 0;
        i = 0;
        while(i < 11)
        {
          checksum = checksum + byte_received[i];
          i++;
        }
        checksum = checksum & 0xFF;
        if((byte_received[0] == '0') && (checksum == byte_received[11]))
        {
          i = 0;
          while(i < 6)
          {
            Serial_Number[i] = byte_received[i + 1];
            i++;
          }
          Serial.print(Serial_Number);
          Serial_Number_MSB_Read_Success = true;
        }
        else
        {
          Serial.print("*");
          delay(250);
        }
      }
      delay(50);
  
      Serial_Number_LSB_Read_Success = false;
      while(!Serial_Number_LSB_Read_Success)
      {                                                                          
        Wire.beginTransmission(I2C_DEV_ADDR);
        Wire.write('1'); 
        Wire.endTransmission();
        delay(50);
        Wire.requestFrom(I2C_DEV_ADDR, 12, true); //Request 12 bytes
        i = 0;
        while(Wire.available()) 
        {
            byte_received[i] = Wire.read(); //Receive byte      
            i++;
        }
        checksum = 0;
        i = 0;
        while(i < 11)
        {
          checksum = checksum + byte_received[i];
          i++;
        }
        checksum = checksum & 0xFF;
        if((byte_received[0] == '1') && (checksum == byte_received[11]))
        {
          i = 0;
          while(i < 6)
          {
            Serial_Number[i] = byte_received[i + 1];
            i++;
          }
          Serial.print(Serial_Number);
          Serial.print(", ");
          Serial_Number_LSB_Read_Success = true;
        }
        else
        {
          Serial.print("*");
          delay(250);
        }
      }
      delay(50);
  
      PPB_Read_Success = false;
      while(!PPB_Read_Success)
      {  
        Wire.beginTransmission(I2C_DEV_ADDR);
        Wire.write('2'); 
        Wire.endTransmission();
        delay(50);
        Wire.requestFrom(I2C_DEV_ADDR, 12, true); //Request 12 bytes
        i = 0;
        while(Wire.available()) 
        {
            byte_received[i] = Wire.read(); //Receive byte      
            i++;
        }
        checksum = 0;
        i = 0;
        while(i < 11)
        {
          checksum = checksum + byte_received[i];
          i++;
        }
        checksum = checksum & 0xFF;   
        if((byte_received[0] == '2') && (checksum == byte_received[11]))
        {
          PPB = (byte_received[1] << 24) | (byte_received[2] << 16) | (byte_received[3] << 8) | (byte_received[4] << 0);
          Tx100 = (byte_received[5] << 8) | (byte_received[6] << 0);
          Hx100 = (byte_received[7] << 8) | (byte_received[8] << 0); 
          Serial.print(PPB); 
          Serial.print(", ");
          Serial.print(Tx100); 
          Serial.print(", ");
          Serial.print(Hx100); 
          Serial.print(", "); 
          PPB_Read_Success = true;  
        }
        else
        {
          Serial.print("*");
          delay(250);
        }
      }
      delay(50);
  
      ADC_Read_Success = false;
      while(!ADC_Read_Success)
      {  
        Wire.beginTransmission(I2C_DEV_ADDR);
        Wire.write('3'); 
        Wire.endTransmission();
        delay(50);
        Wire.requestFrom(I2C_DEV_ADDR, 12, true); //Request 12 bytes
        i = 0;
        while(Wire.available()) 
        {
            byte_received[i] = Wire.read(); //Receive byte      
            i++;
        }
        checksum = 0;
        i = 0;
        while(i < 11)
        {
          checksum = checksum + byte_received[i];
          i++;
        }
        checksum = checksum & 0xFF;  
        if((byte_received[0] == '3') && (checksum == byte_received[11]))
        {
          ADC_Raw = (byte_received[1] << 8) | (byte_received[2] << 0);
          T_Raw = (byte_received[3] << 8) | (byte_received[4] << 0);
          H_Raw = (byte_received[5] << 8) | (byte_received[6] << 0); 
          Serial.print(ADC_Raw); 
          Serial.print(", ");
          Serial.print(T_Raw); 
          Serial.print(", ");
          Serial.println(H_Raw);   
          ADC_Read_Success = true; 
        }
        else
        {
          Serial.print("*");
          delay(250);
        }
      }
      delay(50);
  
#ifdef Timed_Initial_Zero
      if((millis() > (Timed_Initial_Zero_Period * 1000)) && (!Zero_Timer_Triggered))
      {
        Zero_Timer_Triggered = true;

        Serial.print("Zero Requested...");

        Zero_Success = false;
        while(!Zero_Success)
        {  
          Wire.beginTransmission(I2C_DEV_ADDR);
          Wire.write('Z'); //adjust Zero_Timer for a scheduled initial Zero
          Wire.endTransmission();
          delay(50);
          Wire.requestFrom(I2C_DEV_ADDR, 12, true); //Request 12 bytes
          i = 0;
          while(Wire.available()) 
          {
              byte_received[i] = Wire.read(); //Receive byte      
              i++;
          }
          checksum = 0;
          i = 0;
          while(i < 11)
          {
            checksum = checksum + byte_received[i];
            i++;
          }
          checksum = checksum & 0xFF;  
          if((byte_received[0] == 'Z') && (checksum == byte_received[11]))
          {
            Zero_Success = true; 
          }
          else
          {
            Serial.print(".");
            delay(250);
          }
        }
        Serial.println("done");
      }
#endif  

    while((millis() - LastMillis) < (Continous_Output_Period * 1000));  
    LastMillis = millis();
  }
}