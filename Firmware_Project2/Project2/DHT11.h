/*
 * DHT11.h
 *
 * Created: 1/8/2024 1:00:53 PM
 *  Author: TRAN THI BICH HOAN
 */ 


#ifndef DHT11_H_
#define DHT11_H_

void INIT();

void PORT();

void LED7_OUT(unsigned char num);

void DELAY_MS(unsigned int mili_count);

void PB_2_LED();

unsigned char PB_CHECK();

void DHT_GPIO_Write(uint8_t state);
int DHT_GPIO_Read();
void DHT11_Init();
int DHT11_Read();
void DHT11_Read_Data(float *humi, float *temp);



#endif /* DHT11_H_ */