

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF        // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#define _XTAL_FREQ 500000


#include<xc.h>
#include "key_scan.h"
#include"LCD.h"
//#include"Alarm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>


#define one_minute 115
#define one_hours 6921//actual is 6290.52
#define one_second 2
#define MIN_DUTY_CYCLE 4
#define MAX_DUTY_CYCLE 18

// Functions Declaration

void random_init();


void initialize1();
void initialize2();
void UART_Initialize();
void UART_Write(unsigned char data);


int cnt = 0,pos = 0;
unsigned int Timer_t = 49910;






volatile int timer_count_hrs,timer_count_min;
//***************************************MATH STATE*******************************************
volatile int answer,operandA,operandB,input_ans,temp_input=0,operation;

volatile int x;
time_t toc;

 //*******************************************Button******************************************
volatile int KEY, power_switch,state;
bool enter=false;

int main()
{
    
    TRISC=0x00;
    PORTC=0x00;
   // PIE1bits.TMR1IE = 1;
    TRISD = 0x00;

    //*******************************************Button******************************************
    power_switch = 0;
    TRISB = 0x0f; // RB0-RB3 as input (R1-R4), RB4-RB7 as output (C1-C4)
    PORTB = 0x00;
    TRISDbits.RD1=1;
    INTCON2bits.RBPU = 0; // enable weak pull-up
    OSCCONbits.IRCF=0b011;

    //******************************************END***********************************************
    
    while(RD1==1);
    
    power_switch ^= 1;
    __delay_ms(50);
    
    initializeLCD();
    clearLCD();
    
    //************************************************MAIN STATE**************************************

    // First time display
     while(!enter){
        KEY = keyread();
        __delay_ms(30);
        if(KEY>=0){
            if(KEY == 14){
                state = enter = 0;
                resetAlarm();
                continue;
            }
            if(KEY == 15)
                enter = true;
            
            if(state == 0){
                hours = minute= 0x00;
                state++;
            }
            else if(state == 1 && KEY < 10 && enter == 0){
                hours = KEY;
                setAlarm(hours,0);
                state++;
            }
            else if(state == 2 && KEY < 6 && enter == 0){
                minute = KEY;
                setAlarm(minute,1);
                state++;
                minute*=10;
            }

            else if(state == 3 && KEY < 10 && enter == 0){
                int temp;
                temp= KEY;
                setAlarm(temp,2);
                minute+=temp;
                state++;
            }
        }
     }
    __delay_ms(1000);

    //************************************************COUNT DOWN STATE**************************************
    // After user set time then start count down
    countDown(hours, minute);
 
    //************************************************MATH STATE**************************************
    // After count down = 0
    // ring the alarm and show math state
    random_init();
    mathState(operandA,operandB,operation);
   // initialize1();
    LATC=0;
    RC0=1;
    temp_input=0;
    input_ans=0;
    enter=false;
    while(input_ans!=answer){
      KEY = keyread();
      __delay_ms(300);
      if(enter){
          input_ans=0;
          temp_input=0;
          wrongAnswer();
          mathState(operandA,operandB,operation);
          enter=false;
          continue;
      }
      else if(KEY>=0){
            if(KEY == 14){
                input_ans=0;
                temp_input=0;
                enter=false;
                mathState(operandA,operandB,operation);
                continue;
            }
            else if(KEY == 15){
                input_ans=temp_input;
                enter=true;
            }
            
            else if(KEY<10){
                if(temp_input==0){
                    temp_input=KEY;
                }
                else {
                    if(temp_input<999){
                    temp_input=(temp_input*10)+KEY;
                    }
                }
                printNumLCD(KEY,1);
            }
      }
    // Stop the alarm if the user input the correct answer
    }
    RC0=0;
    rightAnswer();  
    while(true);
    return 0;
}


void random_init(){
    srand(TMR1);
    operation =rand()%3;
    
    switch(operation){
        case 0:
            //Addition
        case 1:
            //Subtraction
            //Produce both operand randomly (1 to 1000))
            operandA=rand()%99+1;
            operandB=rand()%99+1;
            if(operation==0)
                answer=operandA+operandB;
            else if(operation==1){
                if(operandB>operandA){
                    //avoiding answer being negative
                    int temp=operandB;
                    operandB=operandA;
                    operandA=operandB;
                }
                answer=operandA-operandB;
            }
            break;
            
        case 2:
            //Multiplication
            //Produce both operand randomly (1 to 12))
            operandA=rand()%12+1;
            operandB=rand()%12+1;
            answer=operandA*operandB;
            break;
    }
    
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}


void UART_Initialize() {
    
    // TODO
    /*portC pin6/7 is mult with usart setting their tris*/    
    
    TRISCbits.TRISC6=1;      
    TRISCbits.TRISC7=1;            
    
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */
    
    
    // choose the async mode
    TXSTAbits.SYNC=0;//asynchronous transmittion   
    
    
    // Setting baud rate
    // using the Baud rate table in data sheet
  
    /*
     * Baud rate
     * 1200=4Mhz/(64(x+1))
     * (64(x+1))=4MHz/1200
     * x=51.083=51

*/
    BAUDCONbits.BRG16=0;//Using 8 bit only      
    TXSTAbits.BRGH=0;
    SPBRG=51;   
    
    // Serial enable
    
    RCSTAbits.SPEN=1;              //open serial port
    PIR1bits.TXIF=1;                //TXREG is empty initially
    PIR1bits.RCIF=0;
    TXSTAbits.TXEN=1;             //Enable Tx
    RCSTAbits.CREN=1;             //Enable Rx
   
    
    // setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 0;              //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 0;              //Setting Rc as low priority interrupt
    }





