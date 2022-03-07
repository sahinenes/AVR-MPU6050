/*
 * MPU6050Lib.cpp
 *
 * Created: 20.12.2021 18:34:59
 * Author : Enes
 */ 


#include "mpu6050.h"
#include "atmega8xx_usart.h"


#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536

#define RAD_TO_DEG 57.295779513082320876798154814105
#define dt 0.01							// 10 ms sample rate!
#define PI 3.1415926535897932384626433832795

float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;


char buffer[20], float_[10];
float Xa,Ya,Za,t;
float Xg=0,Yg=0,Zg=0;

long map(long x, long in_min, long in_max, long out_min, long out_max) ;

int minVal=265;
int maxVal=402;

double x;
double y;
double z;


int main(void)
{
	
		

		//init uart
		USART_t usart;
		usart.clockSpeed=1000000;
		usart.USARTbaudRate=2400;
		usart.USARTsettings.characterSize=USART_CHARACTER_8;
		usart.USARTsettings.clockPolarity=USART_CLK_PLRTY_RSNG;
		usart.USARTsettings.operation=USART_OPERATION_ASYNC;
		usart.USARTsettings.parityMode=USART_PARITY_DISABLED;
		usart.USARTsettings.stop=USART_STOP_2;
		usartInit(&usart);
		
		
		
		
		FILE usart0_str = FDEV_SETUP_STREAM(usartTransmit, NULL, _FDEV_SETUP_WRITE);
		stdout=&usart0_str;
		
		

		I2C_Init();		/* Initialize I2C */
		Gyro_Init();		/* Initialize Gyro */

		//init interrupt
		sei();
		
		printf("setup");

    /* Replace with your application code */
    while (1) 
    {
		
				Read_RawValue();

				int xAng = map(Acc_x,minVal,maxVal,-90,90);
				int yAng = map(Acc_y,minVal,maxVal,-90,90);
				int zAng = map(Acc_z,minVal,maxVal,-90,90);

				x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
				y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
				z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
				
				
				/* Take values in buffer to send all parameters over USART */
				dtostrf( x, 3, 2, float_ );
				sprintf(buffer," x = %s g\t",float_);
				printf(buffer);

				dtostrf( y, 3, 2, float_ );
				sprintf(buffer,"y = %s g\t",float_);
				printf(buffer);

				dtostrf( z, 3, 2, float_ );
				sprintf(buffer," z = %s g\t",float_);
				printf(buffer);
				
				printf("\n");

    }
}


void Read_RawValue()
{
	MPU_Start_Loc();									/* Read Gyro values */
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Temperature = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}