// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
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

#include <xc.h>
#define _XTAL_FREQ 500000
#define MIN_DUTY_CYCLE 4
#define MAX_DUTY_CYCLE 18

int cnt = 0;
int pos = 0;
unsigned int time = 49910;

void mydelay(unsigned int);
void initialize();
void adc_init(void);
void ringBell();
void initializeUART();

int proceed=0;
void main(void) {
    adc_init();

    OSCCONbits.IRCF2 = 0; //use 500kHz oscillator
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
      
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
   
    //__delay_ms(10000);
    while(proceed == 0){
        ADCON0bits.GO = 1; //processing ADC
    }
    
        TRISCbits.TRISC2 = 0; //use RC2 port as output (CCP1)

       //TMR2ON = 1, T2CKPS<1:0> = 11 (1:16 prescaler)
    T2CON = 0x07;
    TMR2 = 0;
     PR2 = 0; //timer 2 is compared to PR2=16 at each cycle //period = 1ms
    CCPR1L = 0; //duty cycle = 0~1 ms
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;
    
    CCP1CONbits.CCP1M3 = 1; //set CCP to PWM mode
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;
    
    //for TMR1
    T1CONbits.RD16 = 1; //use 16 bits
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0; //Clock source = internal clock
    
    
    PIE1bits.TMR1IE = 1;
//    IPR1bits.TMR1IP = 1;
    IPR1bits.TMR1IP = 1;
    PIR1bits.TMR1IF = 0;
    cnt = 0;
    
    while(1){
        ADCON0bits.GO = 1; //processing ADC
    }
    
    while(1);
}

void __interrupt (high_priority) my_isr_routine (void)
{
    if(PIR1bits.ADIF==1){
        PIR1bits.ADIF = 0;
        if(ADRES>1000){
            proceed=1;
     }
        if(ADRES<100){
            proceed=0;
            if(proceed == 0)
            {
                TMR2ON = 0;
                PR2 = 0;
                CCPR1L = 0;
            }
    }
    }
    if(TMR1IF) 
    {
        TMR1IF = 0;
        cnt++;
        if(cnt == 4)
        {
            cnt = 0;
            
            int temp=0;
            temp = time % 3;
            if(temp)
            {
                PR2 = 15625/500 - 1;
                CCPR1L = 0.5*PR2;
            }
            else
            {
                PR2 = 0;
                CCPR1L = 0;
            }
        }
        TMR1 = time; 
        if(time < 65515)
            time += 19;
        if(time == 65528)
        {
            PR2 = 15625/500 - 1;
            CCPR1L = 0.5*PR2;
            __delay_ms(5000);
            TMR2ON = 0;
            PR2 = 0;
            CCPR1L = 0;
            TRISCbits.RC2 = 1;
            IPR1bits.TMR1IP = 0;
            initialize();
            CCPR1L = MAX_DUTY_CYCLE;
            TMR1 = 63000;
        }
    }
    //*********************************************TERMINATE***********************************************

}

void __interrupt(low_priority) low_isr()
{
    if(PIR1bits.ADIF==1){
        PIR1bits.ADIF = 0;
        if(ADRES>1000){
            proceed=1;
     }
        if(ADRES<100){
            proceed=0;
            if(proceed == 0)
            {
                TMR2ON = 0;
                PR2 = 0;
                CCPR1L = 0;
            }
    }
    }
    // Clear interrupt flag bit
    TMR1IF = 0;   
    
    // Clear DC1B<1:0>
    CCP2CONbits.DC2B = 0;
    
    // From 90 degree to -90 degree
    while(1)
    {
        while(CCP2CONbits.DC2B < 3)
        {
            CCP2CONbits.DC2B++;
        }
        
        CCP2CONbits.DC2B = 0;
        CCPR2L--;
        
        if(CCPR2L == MIN_DUTY_CYCLE)
        {
            break;
        }
    }
    
//    mydelay(80);
    __delay_ms(500);
    
    // From -90 degree to 90 degree
    while(1)
    {
        while(CCP2CONbits.DC2B < 3)
        {
            CCP2CONbits.DC2B++;
        }
        
        CCP2CONbits.DC2B = 0;
        CCPR2L++;
        
        if(CCPR2L == MAX_DUTY_CYCLE)
        {
            break;
        }
    }
    
//    mydelay(60);
    __delay_ms(500);
    
    // Clear interrupt flag bit
    TMR1IF = 0;     
    
    TMR1 = 63000;
    
    if(PORTAbits.RA1 == 1)
    {
        TMR2ON = 0;
        PR2 = 0;
        CCPR1L = 0;
    }
}

void mydelay(unsigned int n)
{
    for(int i=0; i < n; ++i)
        for(int j=0; j < n; ++j)
            ;
}

void initialize()
{
    // Setting input and output
    TRISCbits.RC1 = 0;          // set PORTC 2nd bit (CCP1) as output (servo motor)
    
    // Clear all interrupt control
    INTCON = 0;
    INTCON2 = 0;
    INTCON3 = 0;
    
    // Enable Global interrupt bit
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    // Setting interrupt caused by INT0
//    INTCON2bits.INTEDG0 = 1;    // Interrupt on rising edge
//    INTCONbits.INT0IF = 0;      // Clear INT0 flag bit
//    INTCONbits.INT0IE = 1;      //Enable external interrupt
    
    // Setting Timer2
    OSCCONbits.IRCF0 = 1;   // Setting Internal 
    OSCCONbits.IRCF1 = 1;   // oscillator frequency
    OSCCONbits.IRCF2 = 0;   // 011: 500kHz
    
    T2CON = 0x06;           // Setting Timer 2 control register
    PR2 = 155;              // Set PR2 to 155
    TMR2 = 0;               // Initialize TMR2
    
    // Setting CCP1CON
    CCP2CON = 0b00001100;
    
        IPR1bits.ADIP=0; //AD high priority

}

void initializeUART()
{
    // Set Baud Rate
    BAUDCONbits.BRG16 = 0 ;     // 8-bit Baud Rate Generator
    TXSTAbits.BRGH = 0;         // Low speed      
    SPBRG = 51;      
    
    // Set Asynchronize Mode
    TXSTAbits.SYNC = 0;     // Choose asynchronize mode
    RCSTAbits.SPEN = 1;     // Enable asynchronous serial port
    
    // Set Transmitter
    TRISCbits.TRISC6 = 1;   // Set TX as input
    PIE1bits.TXIE = 0;      // Disable transmission interrupt
    TXSTAbits.TXEN = 1;     // Enable the transmission
    PIR1bits.TXIF = 1;      // Will be set (=1) when TXREG is empty
    IPR1bits.TXIP = 0;      // Set transmission as low priority interrupt
    
    // Set Receiver
    TRISCbits.TRISC7 = 1;   // Set RX as input
    PIE1bits.RCIE = 1;      // Enable receiver interrupt
    RCSTAbits.CREN = 1;     // Enable the receiver
    PIR1bits.RCIF = 0;      // Will be set (=1) when reception is done
    IPR1bits.RCIP = 1;      // Set receiver as high priority interrupt
}

void adc_init(void){
    ADCON0bits.CHS = 1; //setting input channel = AN1
    ADCON0bits.ADON = 1 ; // enable ADC module
    ADCON0bits.GODONE = 0; // not processing ADC 
    
    ADCON1bits.VCFG1 = 0; //setting vref- = Vss
    ADCON1bits.VCFG0 = 0; //setting vref+ = Vdd
    ADCON1bits.PCFG = 0; //setting A/D port as analog input
    
    ADCON2bits.ACQT = 2; //setting acquisition time  4Tad
    ADCON2bits.ADCS = 4; //conversion time 4 Tosc
    ADCON2bits.ADFM = 1; //setting RIGHT justified
    
    TRISAbits.TRISA1 = 1; //setting RA1/AN1 port as input
    TRISD = 0;  // set port D as output
    PORTD = 0;

    PIR1bits.ADIF = 0;//clear AD flag
    PIE1bits.ADIE = 1; //set AD enable
    INTCONbits.GIE = 1;//set GIE
    IPR1bits.ADIP=1; //AD high priority
    RCONbits.IPEN=1;
}