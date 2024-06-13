/*
 * LCD.h
 *
 * Created: 1/8/2024 1:04:00 PM
 *  Author: TRAN THI BICH HOAN
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 8000000UL
#define DATA PORTC  //output data for LCD (D0 - D7)
#define RS PD6 //chan RS cua LCD
#define RW PD5 //chan RW cua LCD
#define EN PD7 //chan EN cua LCD
#define LCD_D4 PC4           //Truyen du lieu qua 4 chan
#define LCD_D5 PC5
#define LCD_D6 PC6
#define LCD_D7 PC7
#define CONTROL PORTD

/*khai bao nguyen mau*/
void LCD_Enable(void);
void LCD_Send4Bit(unsigned char Data);
void LCD_SendCommand(unsigned char command);
void LCD_Clear();
void LCD_Init();
void LCD_Gotoxy(int x, int y);
void LCD_PutChar(unsigned char Data);
void LCD_Puts(char *s);
void num_dsp( float number);
/*Bo sung chuc nang cho ham*/
void LCD_Enable(void)
{
	CONTROL |= (1 << EN);  //EN = 1;
	_delay_us(7);
	CONTROL &= ~(1 << EN);  //EN=0;
	_delay_us(50);
}

void LCD_Send4Bit(unsigned char Data)
{
	//LCD_D4=Data & 0x01;
	if (Data & 0x01)
	{
		DATA |= (1 << LCD_D4);
	}
	else DATA &= ~(1 << LCD_D4);
	
	//LCD_D5=(Data>>1)&1;
	if ((Data >> 1) & 1)
	{
		DATA |= (1 << LCD_D5);
	}
	else DATA &= ~(1 << LCD_D5);
	//LCD_D6=(Data>>2)&1;
	if ((Data >> 2) & 1)
	{
		DATA |= (1 << LCD_D6);
	}
	else DATA &= ~(1 << LCD_D6);
	//LCD_D7=(Data>>3)&1;
	if ((Data >> 3) & 1)
	{
		DATA |= (1 << LCD_D7);
	}
	else DATA &= ~(1 << LCD_D7);
}
//ham gui ki tu len LCD
void LCD_SendCommand(unsigned char command)
{
	LCD_Send4Bit(command >> 4);
	LCD_Enable();
	LCD_Send4Bit(command);
	LCD_Enable();
}
void LCD_Clear()
{
	LCD_SendCommand(0x01);
	_delay_ms(2);
}
void LCD_Init()
{
	LCD_Send4Bit(0x00);
	_delay_ms(20);
	CONTROL &= ~(1 << RS);//LCD_RS=0;
	CONTROL &= ~(1 << RW);//LCD_RW=0;
	LCD_Send4Bit(0x03);
	LCD_Enable();
	_delay_ms(5);
	LCD_Enable();
	_delay_us(100);
	LCD_Enable();
	LCD_Send4Bit(0x02);
	LCD_Enable();
	LCD_SendCommand(0x28 );
	LCD_SendCommand(0x0C);
	LCD_SendCommand(0x06);
	LCD_SendCommand(0x01);
	LCD_SendCommand(0x02);
	_delay_ms(2);
}
void LCD_Gotoxy(int x, int y)
{
	unsigned char address;
	if(!y) address = (0x80 + x);
	else address = (0xC0 + x);
	_delay_ms(1);
	LCD_SendCommand(address);
	_delay_us(50);

}
void LCD_PutChar(unsigned char Data)
{
	CONTROL |= (1 << RS);//LCD_RS=1;
	LCD_SendCommand(Data);
	CONTROL &= ~(1 << RS);//LCD_RS=0 ;
}
void LCD_Puts(char *s)
{
	while (*s)
	{
		LCD_PutChar(*s);
		s++;
	}
}
//Ham in ra mot so nguyen tren
void LCD_PutInt(int value) {
	//char buffer[16];  // ?? l?n ?? ch?a m?t s? nguyên
	//sprintf(buffer,"%d", value);  // Chuy?n ??i s? nguyên thành chu?i
	//LCD_Puts(buffer);  // Hi?n th? chu?i lên màn hình LCD
}

#endif /* LCD_H_ */