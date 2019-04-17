#include <stdint.h>
#include <string.h>

float XZero,YZero,ZZero;

typedef struct
{
  int16_t  Begin  ;   // 2  Debut
  uint16_t Cpt ;      // 2  Compteur trame or Code info or error
  float    gyro[3];   // 12 [Y, P, R]    gyro
  float    acc[3];    // 12 [x, y, z]    Acc
  int16_t  End ;      // 2  Fin
} 
_hat;

uint16_t unit;

typedef struct
{
  int16_t Begin  ;   // 2  Debut
  uint16_t Code ;     // 2  Code info
  char     Msg[24];   // 24 Message
  int16_t End ;      // 2  Fin
} 
_msginfo;

_msginfo msginfo;
_hat hat;

int Mag_HardIron_On=0;
int Acc_Calib_On=0;

void PrintCodeSerial(uint16_t code,char Msg[24],bool EOL )
{
  msginfo.Code=code;
  memset(msginfo.Msg,0x00,24);
  strcpy(msginfo.Msg,Msg);
  if (EOL) msginfo.Msg[23]=0x0A;
  // Send HATIRE message to  PC
   Serial.write((byte*)&msginfo,30);
}

//att_t att;
void FT_Setup()
{
  hat.Begin=0xAAAA;
  hat.Cpt=0;
  hat.End=0x5555;
  msginfo.Begin=0xAAAA;
  msginfo.Code=0;
  msginfo.End=0x5555;
  PrintCodeSerial(5000,"HAT BEGIN",true); 
  XZero=TO_DEG(yaw);
  YZero=TO_DEG(pitch);
  ZZero=TO_DEG(roll);
}

void serialEvent()
{
  char Commande;
   Commande = (char)Serial.read();
  switch (Commande) 
  {
  case 'Z':
    // last_z_angle=0;
    XZero=TO_DEG(yaw);
    YZero=TO_DEG(pitch);
    ZZero=TO_DEG(roll);
    //PrintCodeSerial(5000,"Generated",true); 
    break; 
  }
  if (Commande == 'h')
  {
    Mag_HardIron_On=1; 
  }
  if (Commande == 'a')
  {
    Acc_Calib_On=1; 
    for (int i = 0; i < 3; i++) 
    {
      //   accel_min[i]=0;
      // accel_max[i]=0;
    }
  }
  if (Commande == 's') // _s_ynch request
  {
    // Read ID
    //        byte id[2];
    //        id[0] = readChar();
    //        id[1] = readChar();

    // Reply with synch message
    //        Serial.print("#SYNCH");
    //        Serial.write(id, 2);
    //        Serial.println();
    //        Calib_On=1;
  } 
}


void FTData()
{
  if(Serial.available() > 0)  serialEvent();
  hat.gyro[0]=TO_DEG(yaw)-XZero;
  hat.gyro[1]=TO_DEG(pitch)-YZero;
  hat.gyro[2]=TO_DEG(roll)-ZZero;
  Serial.write((byte*)&hat,30);
  hat.Cpt++;  if (hat.Cpt>999) { hat.Cpt=0; };
  delay(10); 


}



