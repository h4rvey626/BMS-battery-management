#include "BQ76930.h"
#include "i2c.h"
#include "usart.h"
#include "math.h"
#include "string.h"
#include <stdio.h>
#include "cmsis_os.h"


/*****************************

******************************/
/***********************
fuction:void get_offset(void)
************************/
int ADC_offset,GAIN;
float ADC_GAIN = 0;





void Get_offset(void)
{
 unsigned char gain[2];
 
 I2C_ReadByte(BQ76940_ADDR, ADCGAIN1, &gain[0]);//ADC_GAIN1
 I2C_ReadByte(BQ76940_ADDR, ADCGAIN2, &gain[1]);//ADC_GAIN2
 ADC_GAIN = ((gain[0]&0x0c)<<1)+((gain[1]&0xe0)>>5);//12uV
 I2C_ReadByte(BQ76940_ADDR, ADCOFFSET, &ADC_offset);//45mV
 GAIN = 365+ADC_GAIN;//GAIN=377uV
}
/****************************************
fuction: void Get_Battery1(void)
description:��ȡ��1�ŵ����ص�ѹ
Parameters:batterylval[0],battery1val[1];
******************************************/
void BQ76930_config(void)
{
	  WAKE_ALL_DEVICE(); 
    BQ_1_2_config();
	  Get_offset();
    OV_UV_1_2_PROTECT();
    OCD_SCD_PROTECT();  
	  //Open_DSG_CHG();  
	 
	  BQ76940_WriteByte_CRC(SYS_STAT,0xFF);  
}

void BQ_1_2_config(void)
{

    BQ_1_config();//
}

void OV_UV_1_2_PROTECT(void)
{
    OV_UV_1_PROTECT();
}

void WAKE_ALL_DEVICE(void)
{
	  MCU_WAKE_BQ_ONOFF(1);
	  HAL_Delay(100);
		MCU_WAKE_BQ_ONOFF(0);

}


void SHIP_ALL_DEVICE(void)
{
	BQ76940_WriteByte_CRC(SYS_STAT,0xFF);
}



 int Batteryval[50]={0};

/* 保护状态标志 (volatile: 多任务读写) */
volatile unsigned char OV_FLAG = 0;
volatile unsigned char UV_FLAG = 0;
volatile unsigned char OC_FLAG = 0;
volatile unsigned char Temp_up_flag = 0;

/* 温度报警标志 (volatile: 多任务读写) */
volatile int OT_Alarm_flag = 0;
volatile int UT_Alarm_flag = 0;
unsigned char shang[50]={0xAA,0x01};
unsigned char shang1[50]={0xAA,0x02};
unsigned char shang2[50]={0xAA,0x03};
unsigned char shang3[50]={0xAA,0x04};

unsigned char can_buf1[8]={0xAA,0x01};
unsigned char can_buf2[8]={0xAA,0x02};
unsigned char can_buf3[8]={0xAA,0x03};
unsigned char can_buf4[8]={0xAA,0x04};
unsigned char can_buf5[8]={0xAA,0x05};
unsigned char can_buf6[8]={0xAA,0x06};
unsigned char can_buf7[8]={0xAA,0x07};


 void Get_Battery1(void)
{
  unsigned char readbattbuf[2];
  unsigned char battery1val[2];
  short batteryval1;
	
  I2C_ReadByte(BQ76940_ADDR, 0x0c, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x0d, &readbattbuf[0]);
 
  batteryval1= readbattbuf[1];
  batteryval1= (batteryval1 << 8) |readbattbuf[0];
  batteryval1=((batteryval1*GAIN)/1000)+ADC_offset;//�����ѹ���㹫ʽ����1��
  Batteryval[0]=batteryval1;
	
  battery1val[1]=(char)(batteryval1 >> 8);
  battery1val[0]=(char)(batteryval1 & 0x00FF);
	
	shang[2]=battery1val[1];
	shang[3]=battery1val[0];
	
	shang1[2]=battery1val[1];
	shang1[3]=battery1val[0];
	
	can_buf1[2]=battery1val[1];
	can_buf1[3]=battery1val[0];

}


/****************************************
fuction: void Get_Battery2(void)
description:��ȡ��2�ŵ����ص�ѹ
Parameters:battery2val[0],battery2val[1];
******************************************/
void Get_Battery2(void)
{
 
  unsigned char readbattbuf[2];
  int batteryval2;
  unsigned char battery2val[2];
	
  I2C_ReadByte(BQ76940_ADDR, 0x0e, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x0f, &readbattbuf[0]);
 
  batteryval2= readbattbuf[1];
  batteryval2= (batteryval2 << 8) |readbattbuf[0];
   
  batteryval2=((batteryval2*GAIN)/1000)+ADC_offset;
	Batteryval[1]=batteryval2;
	
  battery2val[1]=(char)(batteryval2 >> 8);
  battery2val[0]=(char)(batteryval2 & 0x00FF);
	
	shang[4]=battery2val[1];
	shang[5]=battery2val[0];
	shang1[4]=battery2val[1];
	shang1[5]=battery2val[0];
	
	can_buf1[4]=battery2val[1];
	can_buf1[5]=battery2val[0];

}
/****************************************
fuction: void Get_Battery3(void)
description:��ȡ��3�ŵ����ص�ѹ
Parameters:battery3val[0],battery3val[1];
******************************************/
void Get_Battery3(void)
{
 
  unsigned char readbattbuf[2];
  int batteryval3 ;
  unsigned char battery3val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x10, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x11, &readbattbuf[0]);

  batteryval3= readbattbuf[1];
  batteryval3= (batteryval3 << 8) |readbattbuf[0];
  batteryval3=((batteryval3*GAIN)/1000)+ADC_offset;
  battery3val[1]=(char)(batteryval3 >> 8);
  battery3val[0]=(char)(batteryval3 >> 0);
	Batteryval[2]=batteryval3;
	
	shang[6]=battery3val[1];
	shang[7]=battery3val[0];
	
	
	shang1[6]=battery3val[1];
	shang1[7]=battery3val[0];
	
	can_buf1[6]=battery3val[1];
	can_buf1[7]=battery3val[0];
}
/****************************************
fuction: void Get_Battery4(void)
description:获取电池4的电压值
Parameters:battery4val[0],battery4val[1];
******************************************/
void Get_Battery4(void)
{
  unsigned char readbattbuf[2];
  int batteryval4 ;
  unsigned char battery4val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x12, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x13, &readbattbuf[0]);
 
  batteryval4= readbattbuf[1];
  batteryval4= (batteryval4 << 8) |readbattbuf[0];
  batteryval4=((batteryval4*GAIN)/1000)+ADC_offset;//????????,?4?
  battery4val[1]=(char)(batteryval4 >> 8);
  battery4val[0]=(char)(batteryval4 >> 0);
	Batteryval[3]=batteryval4;
	
	shang[8]=battery4val[1];
	shang[9]=battery4val[0];

  shang1[8]=battery4val[1];
	shang1[9]=battery4val[0];
	
	can_buf2[2]=battery4val[1];
	can_buf2[3]=battery4val[0];

}
/****************************************
fuction: void Get_Battery5(void)
description:获取电池5的电压值
Parameters:battery5val[0],battery5val[1];
******************************************/
void Get_Battery5(void)
{
 unsigned char readbattbuf[2];
  short batteryval5 ;
 unsigned char battery5val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x14, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x15, &readbattbuf[0]);
 
  batteryval5= readbattbuf[1];
  batteryval5= (batteryval5 << 8) |readbattbuf[0];
  batteryval5=((batteryval5*GAIN)/1000)+ADC_offset;//????????,?5?
	Batteryval[4]=batteryval5;
  battery5val[1]=(char)(batteryval5 >> 8);
  battery5val[0]=(char)(batteryval5 & 0x00FF);
	

	shang[10]=battery5val[1];
	shang[11]=battery5val[0];
	
	shang1[10]=battery5val[1];
	shang1[11]=battery5val[0];
	
	can_buf2[4]=battery5val[1];
	can_buf2[5]=battery5val[0];

}
/****************************************
fuction: void Get_Battery6(void)
description:获取电池6的电压值
Parameters:battery6val[0],battery6val[1];
******************************************/
void Get_Battery6(void)
{
 
  unsigned char readbattbuf[2];
  short batteryval6 ;
 unsigned char battery6val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x16, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x17, &readbattbuf[0]);
 
  batteryval6= readbattbuf[1];
  batteryval6= (batteryval6 << 8) |readbattbuf[0];
  batteryval6=((batteryval6*GAIN)/1000)+ADC_offset;//????????,?6?
	Batteryval[5]=batteryval6;
  battery6val[1]=(char)(batteryval6 >> 8);
  battery6val[0]=(char)(batteryval6 & 0x00FF);
	
	shang[12]=battery6val[1];
	shang[13]=battery6val[0];

  shang1[12]=battery6val[1];
	shang1[13]=battery6val[0];
	
	can_buf2[6]=battery6val[1];
	can_buf2[7]=battery6val[0];

}
/****************************************
fuction: void Get_Battery7(void)
description:获取电池7的电压值
Parameters:battery7val[0],battery7val[1];
******************************************/
void Get_Battery7(void)
{
  unsigned char readbattbuf[2];
  short batteryval7 ;
  unsigned char battery7val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x18, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x19, &readbattbuf[0]);
 
  batteryval7= readbattbuf[1];
  batteryval7= (batteryval7 << 8) |readbattbuf[0];
  batteryval7=((batteryval7*GAIN)/1000)+ADC_offset;//????????,?7?
	Batteryval[6]=batteryval7;
  battery7val[1]=(char)(batteryval7 >> 8);
  battery7val[0]=(char)(batteryval7 & 0x00FF);
	
	shang[14]=battery7val[1];
	shang[15]=battery7val[0];
	
	shang1[14]=battery7val[1];
	shang1[15]=battery7val[0];
	
	can_buf3[2]=battery7val[1];
	can_buf3[3]=battery7val[0];

}
/****************************************
fuction: void Get_Battery8(void)
description:获取电池8的电压值
Parameters:battery8val[0],battery8val[1];
******************************************/
void Get_Battery8(void)
{
  unsigned char readbattbuf[2];
  int batteryval8 ;
  unsigned char battery8val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x1a, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x1b, &readbattbuf[0]);
 
  batteryval8= readbattbuf[1];
  batteryval8= (batteryval8 << 8) |readbattbuf[0];
  batteryval8=((batteryval8*GAIN)/1000)+ADC_offset;//????????,?8?
  battery8val[1]=(char)(batteryval8 >> 8);
  battery8val[0]=(char)(batteryval8 >> 0);
	Batteryval[7]=batteryval8;
	
	shang[16]=battery8val[1];
	shang[17]=battery8val[0];

  shang1[16]=battery8val[1];
	shang1[17]=battery8val[0];
	
	can_buf3[4]=battery8val[1];
	can_buf3[5]=battery8val[0];

}

/****************************************
fuction: void Get_Battery9(void)
description:获取电池9的电压值
Parameters:battery9val[0],battery9val[1];
******************************************/
void Get_Battery9(void)
{
  unsigned char readbattbuf[2];
  short batteryval9 ;
  unsigned char battery9val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x1c, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x1d, &readbattbuf[0]);
 
  batteryval9= readbattbuf[1];
  batteryval9= (batteryval9 << 8) |readbattbuf[0];
  batteryval9=((batteryval9*GAIN)/1000)+ADC_offset;//????????,?8?
  battery9val[1]=(char)(batteryval9 >> 8);
  battery9val[0]=(char)(batteryval9 & 0x00FF);
	Batteryval[8]=batteryval9;
	
	shang[18]=battery9val[1];
	shang[19]=battery9val[0];

  shang1[18]=battery9val[1];
	shang1[19]=battery9val[0];
	
	can_buf3[6]=battery9val[1];
	can_buf3[7]=battery9val[0];

}
/****************************************
fuction: void Get_Battery10(void)
description:获取电池10的电压值
Parameters:battery10val[0],battery10val[1];
******************************************/
void Get_Battery10(void)
{
  unsigned char readbattbuf[2];
  short batteryval10 ;
  unsigned char battery10val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x1e, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x1f, &readbattbuf[0]);
 
  batteryval10= readbattbuf[1];
  batteryval10= (batteryval10 << 8) |readbattbuf[0];
  batteryval10=((batteryval10*GAIN)/1000)+ADC_offset;//????????,?8?
	Batteryval[9]=batteryval10;
  battery10val[1]=(char)(batteryval10 >> 8);
  battery10val[0]=(char)(batteryval10 & 0x00FF);
	
	shang[20]=battery10val[1];
	shang[21]=battery10val[0];

  shang2[2]=battery10val[1];
	shang2[3]=battery10val[0];
	can_buf4[2]=battery10val[1];
	can_buf4[3]=battery10val[0];


}

/****************************************
fuction: void Get_Battery11(void)
description:获取电池11的电压值
Parameters:battery11val[0],battery11val[1];
******************************************/
void Get_Battery11(void)
{
  unsigned char readbattbuf[2];
  short batteryval11 ;
  unsigned char battery11val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x20, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x21, &readbattbuf[0]);
 
  batteryval11= readbattbuf[1];
  batteryval11= (batteryval11 << 8) |readbattbuf[0];
  batteryval11=((batteryval11*GAIN)/1000)+ADC_offset;//????????,?8?
	Batteryval[10]=batteryval11;
  battery11val[1]=(char)(batteryval11 >> 8);
  battery11val[0]=(char)(batteryval11 & 0x00FF);
	
	shang[22]=battery11val[1];
	shang[23]=battery11val[0];

  shang2[4]=battery11val[1];
	shang2[5]=battery11val[0];
	
	can_buf4[4]=battery11val[1];
	can_buf4[5]=battery11val[0];

}
/****************************************
fuction: void Get_Battery12(void)
description:获取电池12的电压值
Parameters:battery12val[0],battery12val[1];
******************************************/
void Get_Battery12(void)
{
  unsigned char readbattbuf[2];
  short batteryval12 ;
  unsigned char battery12val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x22, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x23, &readbattbuf[0]);
 
  batteryval12= readbattbuf[1];
  batteryval12= (batteryval12 << 8) |readbattbuf[0];
  batteryval12=((batteryval12*GAIN)/1000)+ADC_offset;//????????,?8?
	Batteryval[11]=batteryval12;
  battery12val[1]=(char)(batteryval12 >> 8);
  battery12val[0]=(char)(batteryval12 & 0x00FF);
	
	shang[24]=battery12val[1];
	shang[25]=battery12val[0];

  shang2[6]=battery12val[1];
	shang2[7]=battery12val[0];
	
	can_buf4[6]=battery12val[1];
	can_buf4[7]=battery12val[0];

}
/****************************************
fuction: void Get_Battery13(void)
description:获取电池13的电压值
Parameters:battery13val[0],battery13val[1];
******************************************/
void Get_Battery13(void)
{
  unsigned char readbattbuf[2];
  short batteryval13 ;
  unsigned char battery13val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x24, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x25, &readbattbuf[0]);
 
  batteryval13= readbattbuf[1];
  batteryval13= (batteryval13 << 8) |readbattbuf[0];
  batteryval13=((batteryval13*GAIN)/1000)+ADC_offset;//????????,?8?
	Batteryval[12]=batteryval13;
  battery13val[1]=(char)(batteryval13 >> 8);
  battery13val[0]=(char)(batteryval13 & 0x00FF);
	
	shang[26]=battery13val[1];
	shang[27]=battery13val[0];

  shang2[8]=battery13val[1];
	shang2[9]=battery13val[0];
	
	can_buf5[2]=battery13val[1];
	can_buf5[3]=battery13val[0];

}
/****************************************
fuction: void Get_Battery14(void)
description:获取电池14的电压值
Parameters:battery14val[0],battery14val[1];
******************************************/
void Get_Battery14(void)
{
  unsigned char readbattbuf[2];
  short batteryval14 ;
  unsigned char battery14val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x26, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x27, &readbattbuf[0]);
 
  batteryval14= readbattbuf[1];
  batteryval14= (batteryval14 << 8) |readbattbuf[0];
  batteryval14=((batteryval14*GAIN)/1000)+ADC_offset;//????????,?8?
	Batteryval[13]=batteryval14;
  battery14val[1]=(char)(batteryval14 >> 8);
  battery14val[0]=(char)(batteryval14 & 0x00FF);
	
	shang[28]=battery14val[1];
	shang[29]=battery14val[0];

  shang2[10]=battery14val[1];
	shang2[11]=battery14val[0];
	
	can_buf5[4]=battery14val[1];
	can_buf5[5]=battery14val[0];

}
/****************************************
fuction: void Get_Battery15(void)
description:获取电池15的电压值
Parameters:battery15val[0],battery15val[1];
******************************************/
void Get_Battery15(void)
{
  unsigned char readbattbuf[2];
  short batteryval15 ;
  unsigned char battery15val[2];
  I2C_ReadByte(BQ76940_ADDR, 0x28, &readbattbuf[1]);
  I2C_ReadByte(BQ76940_ADDR, 0x29, &readbattbuf[0]);
 
  batteryval15= readbattbuf[1];
  batteryval15= (batteryval15 << 8) |readbattbuf[0];
  batteryval15=((batteryval15*GAIN)/1000)+ADC_offset;
	Batteryval[14]=batteryval15;
  battery15val[1]=(char)(batteryval15 >> 8);
  battery15val[0]=(char)(batteryval15 & 0x00FF);
	
	shang[30]=battery15val[1];
	shang[31]=battery15val[0];

  shang2[12]=battery15val[1];
	shang2[13]=battery15val[0];
	
	can_buf5[6]=battery15val[1];
	can_buf5[7]=battery15val[0];


}



/****************************
更新所有电池的电压值，并计算总电压值
 *****************************/

void Get_Update_ALL_Data(void)
{
	int i,Sum_val;
//  for(i=0;i<15;i++)
//	{
	   Sum_val= Batteryval[0]+Batteryval[1]+Batteryval[4]+Batteryval[5]+Batteryval[6]+Batteryval[9]+Batteryval[10]+Batteryval[11]+Batteryval[14];
	//}
	
	Batteryval[15] = Sum_val;
	
	shang[32]=(char)(Batteryval[15] >> 8);
  shang[33]=(char)(Batteryval[15] &0XFF);
	
  shang2[14]=(char)shang[32];
  shang2[15]=(char)shang[33];
	
	can_buf6[2]=(char)shang[32];
	can_buf6[3]=(char)shang[33];

}

int SOC;
void Get_SOC(void) 	//获取SOC值，并更新SOC值到Batteryval[16]中
{
  /* 用平均单体电压查表估算 SOC (9串电池) */
  int avg_mv = Batteryval[15] / 9;  // 平均单体电压 mV

  if      (avg_mv >= 4150) { SOC = 100; }
  else if (avg_mv >= 4100) { SOC = 95;  }
  else if (avg_mv >= 4050) { SOC = 90;  }
  else if (avg_mv >= 4000) { SOC = 85;  }
  else if (avg_mv >= 3950) { SOC = 80;  }
  else if (avg_mv >= 3900) { SOC = 75;  }
  else if (avg_mv >= 3850) { SOC = 70;  }
  else if (avg_mv >= 3800) { SOC = 65;  }
  else if (avg_mv >= 3750) { SOC = 60;  }
  else if (avg_mv >= 3700) { SOC = 55;  }
  else if (avg_mv >= 3650) { SOC = 50;  }
  else if (avg_mv >= 3600) { SOC = 45;  }
  else if (avg_mv >= 3550) { SOC = 40;  }
  else if (avg_mv >= 3500) { SOC = 35;  }
  else if (avg_mv >= 3450) { SOC = 30;  }
  else if (avg_mv >= 3400) { SOC = 25;  }
  else if (avg_mv >= 3350) { SOC = 20;  }
  else if (avg_mv >= 3300) { SOC = 15;  }
  else if (avg_mv >= 3200) { SOC = 10;  }
  else if (avg_mv >= 3100) { SOC = 5;   }
  else if (avg_mv >= 3000) { SOC = 2;   }
  else                      { SOC = 0;   }

  Batteryval[16] = SOC;
	
  shang[34]=(char)(Batteryval[16] >> 8);
  shang[35]=(char)(Batteryval[16] &0XFF);

	
	shang2[16]=(char)shang[34];
  shang2[17]=(char)shang[35];
	
	can_buf6[4]=(char)shang[34];
	can_buf6[5]=(char)shang[35];

}




/////////////////////////////////////////////////////////////////////////////////////////////////////////

void OV_UV_1_PROTECT(void) //设置过压和欠压保护
{
  unsigned char OVTrip_Val,UVTrip_Val;
  float t = 0.377;
  OVTrip_Val = (unsigned char)((((unsigned int)((OVPThreshold - ADC_offset)/t + 0.5 ) )>>4) &0xFF);
  UVTrip_Val = (unsigned char)((((unsigned int)((UVPThreshold - ADC_offset)/t + 0.5 ) )>>4) &0xFF);
  BQ76940_WriteByte_CRC(OV_TRIP,OVTrip_Val);
  BQ76940_WriteByte_CRC(UV_TRIP,UVTrip_Val);
 }

 

void OCD_SCD_PROTECT(void)  //设置过流和短路保护
{

BQ76940_WriteByte_CRC(PROTECT1,0xFF);//	
BQ76940_WriteByte_CRC(PROTECT2,0xFF);//

}


/****************************************
fuction: void BQ_1_config(void)
description:BQ76930初始配置函数，配置BQ76930的寄存器以满足系统需求
Parameters: None
******************************************/
unsigned char BQ769_INITAdd[12]={SYS_STAT,CELLBAL1,CELLBAL2,CELLBAL3,SYS_CTRL1,SYS_CTRL2,PROTECT1,PROTECT2,PROTECT3,OV_TRIP,UV_TRIP,CC_CFG};
unsigned char BQ769_INITdata[12]={0xFF,     0x00,    0x00,     0x00,     0x18,    0x43,      0x00,     0x00,    0x00,    0x00,   0x00,  0x19};
void BQ_1_config(void)
{
   char i;
   for(i=0;i<12;i++)
    {
			HAL_Delay(10);
			BQ76940_WriteByte_CRC(BQ769_INITAdd[i],BQ769_INITdata[i]);
    }
}


void Battery1_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL1,0X01);
}

void Battery2_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL1,0X02);
}

void Battery5_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL1,0X10);
}

void Battery6_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL2,0X01);
}

void Battery7_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL2,0X02);
}

void Battery10_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL2,0X10);
}

void Battery11_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL3,0X01);
}
void Battery12_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL3,0X02);
}
void Battery15_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL3,0X10);
}

void Stop_Balance(void)
{
   BQ76940_WriteByte_CRC(CELLBAL1, 0x00);
   BQ76940_WriteByte_CRC(CELLBAL2, 0x00);
   BQ76940_WriteByte_CRC(CELLBAL3, 0x00);
}

/****************************************
fuction: void BQ_1_config(void)
description:BQ76930初始配置函数，配置BQ76930的寄存器以满足系统需求
Parameters: None
******************************************/
void BQ_2_config(void)
{
   char i;
   for(i=0;i<11;i++)
    {
			HAL_Delay(50);
    }
}
/****************************************
fuction:void SHIP_1_BQ769(void)
description:BQ76930进入睡眠模式
Parameters: None
******************************************/
void SHIP_1_BQ769(void)
{
  BQ76940_WriteByte_CRC(SYS_CTRL1,0x19); //01
  HAL_Delay(20);
  BQ76940_WriteByte_CRC(SYS_CTRL1,0x1a); //10
 }

 


/****************************************
fuction:void SHIP_2_BQ769(void)
description:BQ76930唤醒函数，唤醒BQ76930进入正常工作模式
Parameters: None
******************************************/
void WAKE_1_BQ769(void)
{
   MCU_WAKE_BQ_ONOFF(1);
	 HAL_Delay(50);
	 MCU_WAKE_BQ_ONOFF(0);
	 //NVIC_SystemReset();                       //����
 }

 
 /****************************************
fuction: void Get_BQCurrent(void)
description:BQ76940获取电流值的函数，读取BQ76940的电流寄存器，并将电流值转换为mA单位，更新到Batteryval[17]中
Parameters: None
******************************************/

 void Get_BQ_Current(void)
 {	 
   unsigned char readCurrentbuf[2];
	 unsigned int    Currentbufval;
	 float  Currentval;
   unsigned char Currentbuf[1];
	 
	I2C_ReadByte(BQ76940_ADDR, CC_HI_BYTE, &readCurrentbuf[1]);
   I2C_ReadByte(BQ76940_ADDR, CC_LO_BYTE, &readCurrentbuf[0]);
   Currentbufval = readCurrentbuf[1];
   Currentbufval = (Currentbufval << 8)|readCurrentbuf[0];
	 if(  Currentbufval <= 0x7D00 )
	 {	 
	 Currentval = (Currentbufval*2.11); 
   Batteryval[17]=Currentval;
	 shang[36]=Batteryval[17]>>8;
	 shang[37]=Batteryval[17]&0X00FF;
	 shang2[18]=shang[36];
	 shang2[19]=shang[37];
		 
	can_buf6[6]=(char)shang[36];
	can_buf6[7]=(char)shang[37];

	 
	 }
   else
	 {		  
		 Currentval = ((0XFFFF - Currentbufval )*2.11);
		 Batteryval[17]=Currentval;
    shang[36]=Batteryval[17]>>8;
	  shang[37]=Batteryval[17]&0X00FF;
    shang2[18]=shang[36];
	  shang2[19]=shang[37];		 
  can_buf6[6]=(char)shang[36];
	can_buf6[7]=(char)shang[37];
	 
	 }		 	 
 }
 /****************************************
fuction: void Get_BQ_1_Temp(void)
description:BQ76930获取温度值的函数，读取BQ76930的温度寄存器，并将温度值转换为摄氏度单位，更新到Batteryval[18]中
Parameters: None
******************************************/

 float  Tempval_1,Tempval_2,Tempval_3,Tempval_4;
 void Get_BQ1_1_Temp(void)
 {
	 float Rt=0;
   float Rp=10000;
   float T2=273.15+25;
   float Bx=3380;
   float Ka=273.15;
   unsigned char readTempbuf[2];
	 int  TempRes;
   unsigned char Tempbuf[1];
	 I2C_ReadByte(BQ76940_ADDR, TS1_HI_BYTE, &readTempbuf[1]);
   I2C_ReadByte(BQ76940_ADDR, TS1_LO_BYTE, &readTempbuf[0]);
   TempRes = (readTempbuf[1] << 8 ) | readTempbuf[0];
	 TempRes = (10000*(TempRes*382/1000))/(3300-(TempRes*382/1000)); 
   Tempval_1 = 1/(1/T2+(log(TempRes/Rp))/Bx)- Ka + 0.5;
	 Batteryval[18] = Tempval_1;
   	 	 
 }


 void Get_BQ1_2_Temp(void) //获取温度值的函数，读取BQ76930的温度寄存器，并将温度值转换为摄氏度单位，更新到Batteryval[18]中
 {
   float Rt=0;
   float Rp=10000;
   float T2=273.15+25;
   float Bx=3380;
   float Ka=273.15;
   unsigned char readTempbuf[2];
	 int  TempRes;
   unsigned char Tempbuf[1];
	 I2C_ReadByte(BQ76940_ADDR, TS1_HI_BYTE, &readTempbuf[1]);
   I2C_ReadByte(BQ76940_ADDR, TS1_LO_BYTE, &readTempbuf[0]);
   TempRes = (readTempbuf[1] << 8 ) | readTempbuf[0];
	 TempRes = (10000*(TempRes*382/1000))/(3300-(TempRes*382/1000)); 
   Tempval_2 = 1/(1/T2+(log(TempRes/Rp))/Bx)- Ka + 0.5;
	 Batteryval[18] = Tempval_2;
   shang[38]=Batteryval[18];	 	 
	 shang3[2]=shang[38];	 
	can_buf7[2]=(char)shang[38];

 }
  
 unsigned char BMS_sta,DSG_STA,CHG_STA,DSG_STA_FLAG,CHG_STA_FLAG;	
void BMS_STA(void) //获取BQ76930的放电和充电状态，并更新到DSG_STA和CHG_STA变量中
	{
   I2C_ReadByte(BQ76940_ADDR, SYS_CTRL2, &BMS_sta);
   DSG_STA = BMS_sta&0x02;
	 CHG_STA = BMS_sta&0x01;
	 shang[39]=DSG_STA;
	 shang[40]=CHG_STA;
		
	can_buf7[3]=(char)shang[39];
	can_buf7[4]=(char)shang[40];

	}
 
 void Only_Open_CHG(void) //仅打开充电MOS管，关闭放电MOS管
	{
		BMS_STA();
		if(DSG_STA!=0)
		{
	    BQ76940_WriteByte_CRC(SYS_CTRL2,0X43);	
		}
		else {BQ76940_WriteByte_CRC(SYS_CTRL2,0X41);}
	}
 void Only_Close_CHG(void) //仅关闭充电MOS管，打开放电MOS管
	{
		BMS_STA();
		if(DSG_STA!=0)
		{
	    BQ76940_WriteByte_CRC(SYS_CTRL2,0X42);	
		}
		else
		{BQ76940_WriteByte_CRC(SYS_CTRL2,0X40);	}
	}
	
	void Only_Open_DSG(void) //仅打开放电MOS管，关闭充电MOS管
	{
		BMS_STA();
		if(CHG_STA!=0)
		{
	    BQ76940_WriteByte_CRC(SYS_CTRL2,0X43);	
		}
		else
		{BQ76940_WriteByte_CRC(SYS_CTRL2,0X42);}	
	}
  void Only_Close_DSG(void) //仅关闭放电MOS管，打开充电MOS管
	{
		BMS_STA();
		if(CHG_STA!=0)
		{
	    BQ76940_WriteByte_CRC(SYS_CTRL2,0X41);	
		}
		else
		{BQ76940_WriteByte_CRC(SYS_CTRL2,0X40);	}
	}
	
	 void Open_DSG_CHG(void) //同时打开放电和充电MOS管
	{
	 BQ76940_WriteByte_CRC(SYS_CTRL2,0X43);
   		
	}
 void Close_DSG_CHG(void) //同时关闭放电和充电MOS管
	{
	 BQ76940_WriteByte_CRC(SYS_CTRL2,0X40);	
	}
 
 
 
 
static void UART_SendString(const char *str)
{
	while(*str)
	{
		USART_SendData(USART1, *str++);
	}
}

 void Update_val_usart(void)
{
	char buf[48];
	extern float Tempval_2;
	static const int cell_idx[] = {0, 1, 4, 5, 6, 9, 10, 11, 14};

	UART_SendString("--- BMS Data ---\r\n");

	for(int i = 0; i < 9; i++)
	{
		sprintf(buf, "Cell%d: %dmV\r\n", i + 1, Batteryval[cell_idx[i]]);
		UART_SendString(buf);
	}

	sprintf(buf, "Total: %dmV\r\n", Batteryval[15]);
	UART_SendString(buf);

	sprintf(buf, "SOC: %d%%\r\n", Batteryval[16]);
	UART_SendString(buf);

	int temp_int = (int)Tempval_2;
	int temp_dec = (int)((Tempval_2 - temp_int) * 100);
	if(temp_dec < 0) temp_dec = -temp_dec;
	sprintf(buf, "Temp: %d.%02dC\r\n", temp_int, temp_dec);
	UART_SendString(buf);

	sprintf(buf, "Current: %dmA\r\n", Batteryval[17]);
	UART_SendString(buf);

	sprintf(buf, "DSG: %s, CHG: %s\r\n", DSG_STA ? "ON" : "OFF", CHG_STA ? "ON" : "OFF");
	UART_SendString(buf);
}
/*
 void Update_val_can(void)
{
	Can_Send_Msg(can_buf1,8,0x0001);
	HAL_Delay(2);
	Can_Send_Msg(can_buf2,8,0x0002);
	HAL_Delay(2);
	Can_Send_Msg(can_buf3,8,0x0003);
	HAL_Delay(2);
	Can_Send_Msg(can_buf4,8,0x0004);
	HAL_Delay(2);
	Can_Send_Msg(can_buf5,8,0x0005);
	HAL_Delay(2);
	Can_Send_Msg(can_buf6,8,0x0006);
	HAL_Delay(2);
	Can_Send_Msg(can_buf7,8,0x0007);
	HAL_Delay(2);
} */

 void Get_Update_Data(void)
{	  
    Get_Battery1();
    Get_Battery2();
    //Get_Battery3();
	 // Get_Battery4();
	  Get_Battery5();
	  Get_Battery6();
	  Get_Battery7();
	 // Get_Battery8();
	  //Get_Battery9();
	  Get_Battery10();
	  Get_Battery11();
    Get_Battery12();
    //Get_Battery13();
   // Get_Battery14();
    Get_Battery15(); 	
	Get_Update_ALL_Data();
	Get_SOC();
	Get_BQ1_2_Temp();
    Get_BQ_Current();	
    BMS_STA();
}

/****************************************
fuction:void readbqstate(void)
description:读取BQ76930的状态寄存器，并提取过压、欠压、短路和过流报警标志位，更新到相应的变量中，并根据标志位打印报警信息
Parameters: UV_Alarm_flag�OOV_Alarm_flag
            SCD_Alarm_flag,OCD_Alarm_flag
******************************************/

 int UV_Alarm_flag,OV_Alarm_flag,SCD_Alarm_flag,OCD_Alarm_flag;
void ALERT_1_Recognition(void)
{
    uint8_t sys_stat_1;
    I2C_ReadByte(BQ76940_ADDR, SYS_STAT, &sys_stat_1);
    
    // 直接提取标志位（位操作结果本身就是 0 或 非0）
    UV_Alarm_flag = (sys_stat_1 & 0x08) ? 1 : 0;
    OV_Alarm_flag = (sys_stat_1 & 0x04) ? 1 : 0;
    SCD_Alarm_flag = (sys_stat_1 & 0x02) ? 1 : 0;
    OCD_Alarm_flag = (sys_stat_1 & 0x01) ? 1 : 0;
    
    // 根据标志位打印报警信息
    if (UV_Alarm_flag) printf("pack UV\n");
    if (OV_Alarm_flag) printf("pack OV\n");
    if (SCD_Alarm_flag) printf("pack SCD\n");
    if (OCD_Alarm_flag) printf("pack OCD\n");
}

unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
{
	unsigned char i;
	unsigned char crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= key;
			}
			else
				crc *= 2;

			if((*ptr & i)!=0)
				crc ^= key;
		}
		ptr++;
	}
	return(crc);
}

