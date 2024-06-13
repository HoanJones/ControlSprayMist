/*
 * DHT11.c
 *
 * Created: 1/8/2024 1:01:42 PM
 *  Author: TRAN THI BICH HOAN
 */ 
#include <avr/io.h>
#include "Delay.h"
#include "LCD.h"
#include "DHT11.h"

#define DHT_PIN 1
#define LED_PIN 2
#define DHT_DDR DDRA
#define DHT_OUTPUT PORTA
#define DHT_INPUT PINA

int x, y, z, t, c;
int a = 0, b = 0;

#define FRE 8


/**************************************************************************

H?m INIT() l? h?m kh?ng c? tham s? v? kh?ng tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? kh?i t?o tr?ng th?i c?c PORT c?a vi ?i?u khi?n. Nh? b?t k? h?m t? x?y d?ng n?o kh?c, ng??i l?p tr?nh c? th? ??i t?n, chia nh?, g?p to, ho?c thay ??i c?c l?nh trong h?m n?y theo d?ng ? ri?ng khi l?p tr?nh.

**************************************************************************/

void INIT()

{

	// Kh?i t?o tr?ng th?i Output cho c?c ch?n n?i t?i c?c LED ??n

	DDRD |= 0xFF;

	

	// Kh?i t?o tr?ng th?i logic 1 cho c?c ch?n n?i t?i c?c LED ??n

	PORTD |= 0xFF;

	

	// Kh?i t?o tr?ng th?i Output cho c?c ch?n n?i t?i LED 7 thanh

	DDRC |= 0xFF;

	

	// Kh?i t?o tr?ng th?i logic 1 cho c?c ch?n n?i t?i LED 7 thanh

	PORTC |= 0xFF;

}

/**************************************************************************

H?m PORT() l? h?m kh?ng c? tham s? v? kh?ng tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? ?i?u khi?n tr?ng th?i logic 0/1 c?a c?c ch?n trong c?c PORT c?a vi ?i?u khi?n. Trong m?ch Kit n?y, tr?ng th?i logic l?m c?c ??n LED s?ng/t?t theo quy t?c: 0 ? LED s?ng, 1 ? LED t?t.

**************************************************************************/

void PORT()

{

	/* Khai b?o c?c bi?n s? d?ng t?i trong h?m n?y */

	

	// Bi?n led_shift ?? ?i?u khi?n c?c LED ??n

	// Gi? tr? ??u l? 255 = 0xFF = 0b11111111 -> t?t c? c?c LED ??u t?t

	unsigned char led_shift = 255;

	

	// Bi?n ??m cho LED 7 thanh, gi? tr? ??u l? 0

	unsigned char led_7_count = 0;

	

	// V?ng for gi?p c?c LED s?ng/t?t theo quy lu?t l?p ?i l?p l?i

	for(;;)

	{

		/* ?o?n m? ?i?u khi?n c?c LED ??n */

		

		// C?c LED s?ng/t?t theo 8 bit c?a bi?n led_shift

		PORTD = led_shift;

		

		// Thay ??i bi?n led_shift

		if(led_shift != 0)			// N?u led_shift kh?c 0

		led_shift = led_shift << 1;	// D?ch tr?i 1 bit

		else

		led_shift = 255;			// Tr? l?i gi? tr? 255

		

		/* ?o?n m? ?i?u khi?n LED 7 thanh */

		

		// Xu?t gi? tr? ??m ra LED 7 thanh

		LED7_OUT(led_7_count);

		

		// ??o tr?ng th?i PC3 ?? nh?p nh?y d?u ch?m tr?n LED 7 thanh

		PORTC ^= (1<<PC3);

		

		// T?ng d?n gi? tr? ??m

		led_7_count = led_7_count + 1;

		

		// Khi v??t qu? 9, gi? tr? ??m ???c reset v? 0

		if(led_7_count > 9)

		led_7_count = 0;

		

		// H?m tr? kho?ng 0.5 s = 500 ms

		DELAY_MS(500);

	}

}

/**************************************************************************

H?m LED7_OUT() l? h?m c? tham s? num nh?ng kh?ng tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? ?i?u khi?n LED 7 thanh ch? th? gi? tr? c?a num (0-9) b?ng c?ch s?ng/t?t c?c ?o?n LED m?t c?ch ph? h?p. H?m LED7_OUT() kh?ng l?m thay ??i tr?ng th?i s?ng/t?t c?a d?u ch?m tr?n LED 7 thanh. Trong m?ch Kit n?y, tr?ng th?i logic l?m c?c thanh LED s?ng/t?t nh? sau: 0 ? thanh LED s?ng, 1 ? thanh LED t?t.

**************************************************************************/

void LED7_OUT(unsigned char num)

{

	// Khai b?o bi?n temp l?u tr?ng th?i c?a PORTC

	unsigned char temp = PORTC;

	

	// C?c ch?n vi ?i?u khi?n ?ng v?i c?c thanh LED

	// a - PC5				PC5

	// b - PC4			PC6		PC4

	// c - PC2			PC6		PC4

	// d - PC1				PC7

	// e - PC0			PC0		PC2

	// f - PC6			PC0		PC2

	// g - PC7				PC1		PC3

	// dot - PC3

	

	// T?t c?c ?o?n LED hi?n ?ang s?ng tr??c khi s?ng c?c ?o?n LED m?i

	temp &= 0B00001000;

	

	// G?n m?c logic cho 8 bit c?a bi?n temp ?ng v?i gi? tr? c?a bi?n num

	switch(num)

	{

		case 0: temp |= 0B10000000; break;

		case 1: temp |= 0B11100011; break;

		case 2: temp |= 0B01000100; break;

		case 3: temp |= 0B01000001; break;

		case 4: temp |= 0B00100011; break;

		case 5: temp |= 0B00010001; break;

		case 6: temp |= 0B00010000; break;

		case 7: temp |= 0B11000011; break;

		case 8: temp |= 0B00000000; break;

		case 9: temp |= 0B00000001; break;

	}

	

	// Xu?t gi? tr? logic m?i ra PORTC ?? l?m s?ng LED 7 thanh

	PORTC = temp;

}

/**************************************************************************

H?m DELAY_MS() l? h?m c? tham s? mili_count nh?ng kh?ng tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? t?o ra kho?ng th?i gian tr? (th?i gian ch?) t?nh b?ng mili gi?y. Vi?c tr? ???c th?c hi?n b?ng c?c v?ng l?p r?ng. V?ng l?p r?ng (c? th? l? v?ng for) tuy kh?ng th?c hi?n c?ng vi?c g? nh?ng v?n l?m CPU ti?u t?n m?t kho?ng th?i gian nh?t ??nh cho vi?c kh?i t?o v? k?t th?c. Nhi?u v?ng for li?n ti?p s? t?o m?t kho?ng tr? ??ng k?.

**************************************************************************/

void DELAY_MS(unsigned int mili_count)

{

	// Khai b?o hai bi?n ch?y cho hai v?ng for

	unsigned int i,j;

	

	// Xung nh?p c?a h? th?ng c?ng cao, s? v?ng l?p c?ng t?ng

	mili_count = mili_count * FRE;

	

	// C?c v?ng for g?y tr?

	for(i = 0; i < mili_count; i++)

	for(j = 0; j < 53; j++);

}

/**************************************************************************

H?m PB_2_LED() l? h?m kh?ng c? tham s? v? kh?ng tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? ?i?u khi?n c?c LED theo ph?m ?n v?i quy t?c m? t? t?i M?c 3.3.1.

**************************************************************************/

void PB_2_LED()

{

	// V?ng for gi?p vi?c qu?t ph?m ?n ???c l?p ?i l?p l?i

	for(;;)

	{

		// G?i h?m qu?t ph?m, l?u k?t qu? ph?m ?n v?o bi?n push_button

		int push_button = PB_CHECK();

		

		// Hi?n s? th? t? ph?m ?n ra LED 7 thanh

		LED7_OUT(push_button);

		

		// ?i?u khi?n h?ng LED ??n

		switch (push_button)

		{

			// N?u push_button = 1, s?ng 2 LED ngo?i c?ng b?n tr?i

			case 1: PORTD = 0b11111100; break;

			

			// N?u push_button = 2, ?

			case 2: PORTD = 0b11110011; break;

			case 3: PORTD = 0b11001111; break;

			case 4: PORTD = 0b00111111; break;

			

			// push_button = 0, t?t t?t c? c?c LED

			default: PORTD = 0xFF;

		}

	}

}

/**************************************************************************

H?m PB_CHECK() l? h?m kh?ng c? tham s? v? c? tr? l?i gi? tr?, do ng??i l?p tr?nh t? x?y d?ng ?? nh?n di?n ph?m ?ang ???c ?n. Gi? tr? tr? l?i c?a h?m ch?nh l? th? t? c?a ph?m. Khi ???c ?n, ph?m s? k?t n?i ch?n t??ng ?ng c?a V?K t?i GND (m?c logic 0). Khi nh? ph?m, ch?n t??ng ?ng c?a V?K ???c treo l?n m?c logic 1 nh? c?c tr? k?o c? s?n. L?u ?: ph?m 1 ???c n?i t?i PB0, ph?m 2 ???c n?i t?i PB1,... ph?m 4 ???c n?i t?i PB3.

**************************************************************************/

unsigned char PB_CHECK()

{

	// Ki?m tra tr?ng th?i logic c?a 4 ch?n PB0-3. N?u kh?c 1111

	if((PINB & 0x0F) != 0x0F)

	{

		// Ki?m tra PB0. N?u l? m?c logic 0, h?m k?t th?c v? = 1

		if(!(PINB & (1<<PB0)))

		return 1;

		

		// Ki?m tra PB1. N?u l? m?c logic 0, h?m k?t th?c v? = 2

		if(!(PINB & (1<<PB1)))

		return 2;

		

		// Ki?m tra PB2. N?u l? m?c logic 0, h?m k?t th?c v? = 3

		if(!(PINB & (1<<PB2)))

		return 3;

		

		// Ki?m tra PB3. N?u l? m?c logic 0, h?m k?t th?c v? = 4

		if(!(PINB & (1<<PB3)))

		return 4;

	}

	

	// N?u kh?ng c? ph?m n?o ???c ?n, h?m k?t th?c v? = 0

	return 0;

}

void DHT_GPIO_Write(uint8_t state)
{
	DHT_DDR |= (1 << DHT_PIN);
	if (state == 1)
	{
		DHT_OUTPUT |= (1 << DHT_PIN);
	}
	else DHT_OUTPUT &= ~(1 << DHT_PIN);
}
int DHT_GPIO_Read()
{
	DHT_DDR &= ~(1 << DHT_PIN);
	return 1&(DHT_INPUT >> DHT_PIN);
}


void DHT11_Init()
{
	DHT_GPIO_Write(1);
	DHT_GPIO_Write(0);
	delay_ms(20);
	DHT_GPIO_Write(1);
	while(DHT_GPIO_Read() == 1);
	while(DHT_GPIO_Read() == 0);
	while(DHT_GPIO_Read() == 1);
	// start transmit data
}


int DHT11_Read()
{
	for (int i = 0; i < 8; i++)
	{
		while(DHT_GPIO_Read() == 0);
		delay_30us();
		if(DHT_GPIO_Read() == 1)  // muc high
		{
			a = (a << 1) | 0x01;
		}
		else a = (a << 1);
		while(DHT_GPIO_Read() == 1);
	}
	b = a;
	a = 0x00;
	return b;
}


void Led_control (uint8_t state){
	DDRA |= (1 << LED_PIN);
	if (state == 1){
		PORTA |= (1 << LED_PIN);
	}
	else {
		PORTA &= ~(1<<LED_PIN);
	}
}

void DHT11_Read_Data(float *humi, float *temp)
{
	DHT11_Init();
	x = DHT11_Read();
	y = DHT11_Read();
	z = DHT11_Read();
	t = DHT11_Read();
	c = DHT11_Read();
	*humi = x + (float)y/10.0;
	*temp = z + (float)t/10.0;
	if (*humi >= 60){
		Led_control(1);
		delay_ms(1000);

	}
	else{
		Led_control(0);
	}
	delay_ms(100);

	Led_control(0);
}
