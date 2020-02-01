#ifndef LCD_H
#define LCD_H
#include<stdio.h>
#include <xc.h>

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#define _XTAL_FREQ 500000


void initializeLCD();
void setBitLCD(char data_bit);
void commandLCD(char a);
void clearLCD();
void setCursorLCD(char a, char b);
void printCharLCD(char data);
void printStringLCD(char *a);
void printNumLCD(int n, int digit);
void countDown(int hour, int min);
void mathState(int,int,int);
void resetAlarm();
void setAlarm(int num, int index);
void rightAnswer();
void wrongAnswer();


volatile int hours,minute,second;
volatile unsigned int total_instruction_cycle=0;

#endif