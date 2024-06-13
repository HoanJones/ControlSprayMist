/*
 * Project2.cpp
 *
 * Created: 1/8/2024 12:52:31 PM
 * Author : TRAN THI BICH HOAN
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "LCD.h"
#include <stdio.h>
#include <avr/sfr_defs.h>

#define DHT11_PIN 0 //chan tín hi?u vào t? DHT11 t?i PB0 c?a VXL
#define PA0_CONTROL PA0 //Chan PA0 dùng ?? ?i?u khi?n xu?t ?i?n áp ra 5V
#define DO_AM 80 //Nguong do am cho phep phun suong
#define NHIET_DO 25 //nguong nhiet do phun suong
#define DB DDRB
#define DC DDRC
#define DD DDRD
#define PB PORTB
#define PC PORTC

uint8_t c = 0, I_RH, D_RH, I_Temp, D_Temp, CheckSum;

void Request()				/* VDK gui xung yeu cau */
{
	DB |= (1<<DHT11_PIN);
	PB &= ~(1<<DHT11_PIN);	/* cho chan PB o muc logic thap */
	_delay_ms(20);			/* cho xung thap dai 20ms */
	PB |= (1<<DHT11_PIN);	/* keo chan PB len muc logic cao*/
}

void Response()				/* nhan phan hoi tu DHT11 */
{
	DB &= ~(1<<DHT11_PIN);
	while(PINB & (1<<DHT11_PIN));        /* xung cao */
	while((PINB & (1<<DHT11_PIN)) == 0);   /* xung thap dai 54us */
	while(PINB & (1<<DHT11_PIN));        /* xung cao dai 80us */
}

uint8_t Receive_data()			/* ham nhan du lieu tu DHT11 */
{
	for (int q = 0; q < 8; q++)
	{
		while((PINB & (1<<DHT11_PIN)) == 0);  /* cho doan tin hieu xung thap */
		_delay_us(30);
		if(PINB & (1<<DHT11_PIN))/* neu xung cao dai hon 30us */
		{
			c = (c<<1)|(0x01);	/* thi gia tri logic tra ve co gia tri 1 */
		}
		else			/* neu khong gia tri logic tra ve la 0 */
		{
			c = (c<<1);
		}
		while(PINB & (1<<DHT11_PIN));
	}
	return c;
}

//Ham dieu khien ?i?n áp ra ? chân PD0 khi ?? ?m d??i ng??ng
void Control_Output(float do_am, float temp) {
	if (do_am < DO_AM | temp > NHIET_DO) {
		PORTA |= (1 << PA0_CONTROL); // ??t chân PA0 lên m?c cao (5V)
	} 
	else {
		PORTA &= ~(1 << PA0_CONTROL); // ??t chân PA0 xu?ng m?c th?p (0V)
	}
}

int main(void)
{
	//Khoi tao trang thai
	DDRD |= 0xFF;
	PORTD |= 0xAA;
	DDRC |= 0xFF;
	PORTC |= 0x00;
	DDRA |= (1 << PA0_CONTROL); // Ch?n chân PD0 làm ??u ra
	PORTA |= 0x00;
	
	//Khai bao ?âu ra ?i?u khi?n
	//DDRD |= (1 << PD0_CONTROL); // Ch?n chân PD0 làm ??u ra
	//khai bao bien
	
	//Bat LCD
	
	LCD_Init();
	char nhietdo[5];
	char doam[5];
	
	LCD_Gotoxy(0, 0);
	LCD_Puts("Pham Van Hoan");
	LCD_Gotoxy(1, 1);
	LCD_Puts("MSSV: 20203427");
	_delay_ms(3000);
	LCD_Clear();
	_delay_ms(100);
	
	while (1)
	{
		Request();		/* gui xung yeu cau */
		Response();		/* nhan phan hoi */
		I_RH = Receive_data();	/* luu 8 bit dau tien vao I_RH */
		D_RH = Receive_data();	/* luu 8 bit tiep theo vao D_RH */
		I_Temp = Receive_data();	/* luu 8 bit tiep theo vao I_Temp */
		D_Temp = Receive_data();	/* luu 8 bit tiep theo vao D_Temp */
		CheckSum = Receive_data();/* luu 8 bit tiep theo vao CheckSum */

		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			LCD_Gotoxy(0, 0);
			LCD_Puts("Error!!!");
		}
		else
		{
			LCD_Gotoxy(0, 0);
			LCD_Puts("Nhiet do: ");
			sprintf(nhietdo,"%d\337C", I_Temp);
			_delay_ms(500);
			LCD_Gotoxy(10, 0);
			LCD_Puts(nhietdo);
			
			LCD_Gotoxy(0, 1);
			LCD_Puts("Do am: ");
			sprintf(doam,"%d ", I_RH);
			_delay_ms(100);
			LCD_Gotoxy(7, 1);
			LCD_Puts(doam);
			LCD_Gotoxy(10, 1);
			LCD_Puts("%");
			_delay_ms(500);
			
			//?i?u khi?n ?i?n áp ra chân PD0
			Control_Output(I_RH, I_Temp);
		}
		_delay_ms(10);
	}

}


