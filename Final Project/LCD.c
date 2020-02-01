#include"LCD.h"

void rightAnswer()
{
    clearLCD();
    setCursorLCD(1,5);
    printStringLCD("Correct!");
    setCursorLCD(2,5);
    printStringLCD("(.^oo^.)");
    __delay_ms(2000);
}
void wrongAnswer()
{
    clearLCD();
    setCursorLCD(1,3);
    printStringLCD("Wrong answer");
    setCursorLCD(2,6);
    printStringLCD("(T__T)");
    __delay_ms(2000);
}

void resetAlarm()
{
    setCursorLCD(1,6);
    printStringLCD("Alarms");
    setCursorLCD(2,4);
    printStringLCD("-hrs --min");
}

void setAlarm(int num, int index)
{
    if (index == 0)
    {
        setCursorLCD(2,4);
        printNumLCD(num, 1);
    }
    else if (index == 1)
    {
        setCursorLCD(2,9);
        printNumLCD(num, 1);
    }
    else if (index == 2)
    {
        printNumLCD(num, 1);
    }
    else
    {
        clearLCD();
        setCursorLCD(1,6);
        printStringLCD("ERROR!");
    }
}

void mathState(int num1, int num2, int operator)
{

    // Display math state
    clearLCD();
    setCursorLCD(1,4);
    printStringLCD("Math State");

    setCursorLCD(2,2);
    printNumLCD(num1, 2);
    printCharLCD(' ');

    if (operator == 0)
    {
        printCharLCD('+');

    }
    else if (operator == 1)
    {
        printCharLCD('-');
    }
    else if (operator == 2)
    {
        printCharLCD('X');
    }
    else
    {
        clearLCD();
        setCursorLCD(1,6);
        printStringLCD("ERROR!");
        return;
    }

    printCharLCD(' ');
    printNumLCD(num2, 2);
    printStringLCD(" = ");
}


void countDown(int hour, int min)
{
    total_instruction_cycle = (hour*60*60) + (min*60);
    int sec = 0;
    OSCCONbits.IRCF=0b011;

    // Display time left
    clearLCD();
    setCursorLCD(1,4);
    printStringLCD("Time Left:");
    setCursorLCD(2,1);
    printStringLCD("0hrs 00min 00sec");

    while(total_instruction_cycle > 0)
    {
        // Display hour min sec
        setCursorLCD(2,1);
        printNumLCD(hour, 1);
        setCursorLCD(2,6);
        printNumLCD(min, 2);
        setCursorLCD(2,12);
        printNumLCD(sec, 2);

        if (min == 0 && sec == 0)
        {
            min = 59;
            sec = 59;
            hour--;
        }
        else if (sec == 0)
        {
            sec = 59;
            min--;
        }
        else
        {
            sec--;
        }

        total_instruction_cycle--;

        // Delay for 1 sec
         __delay_ms(990);
    }
}


void initializeLCD()
{
    setBitLCD(0x00);
    for(int i=1065244; i<=0; i--)  NOP();  
    commandLCD(0x03);
      __delay_ms(5);
    commandLCD(0x03);
      __delay_ms(11);
    commandLCD(0x03); 
    commandLCD(0x02);      //02H is used for Return home -> Clears the RAM and initializes the LCD
    commandLCD(0x02);      //02H is used for Return home -> Clears the RAM and initializes the LCD
    commandLCD(0x08);      //Select Row 1
    commandLCD(0x00);      //Clear Row 1 Display
    commandLCD(0x0C);      //Select Row 2
    commandLCD(0x00);      //Clear Row 2 Display
    commandLCD(0x06);      //Increment cursor (shift cursor to right)
}

//Based on the Hex value Set the Bits of the Data Lines
void setBitLCD(char data_bit) 
{
    if(data_bit& 1) 
        D4 = 1;
    else
        D4 = 0;

    if(data_bit& 2)
        D5 = 1;
    else
        D5 = 0;

    if(data_bit& 4)
        D6 = 1;
    else
        D6 = 0;

    if(data_bit& 8) 
        D7 = 1;
    else
        D7 = 0;
}

void commandLCD(char a)
{
    RS = 0;         //Command mode           
    setBitLCD(a);   //Incoming Hex value
    EN  = 1;         
    __delay_ms(4);
    EN  = 0;         
}

void clearLCD()
{
    commandLCD(0); //Clear the LCD (0x00)
    commandLCD(1); //Move the cursor to first position (0x01)
}

void setCursorLCD(char a, char b)
{
    char tem, lower, upper;
    if(a == 1)
    {
        tem = 0x80 + b - 1;  //80H is used to move the cursor
        lower = tem & 0x0F;  //Lower 8-bits (0)
        upper = tem>>4;      //Upper 8-bits (8)
        commandLCD(upper);   //Set Row
        commandLCD(lower);   //Set Column
    }
    else if(a == 2)
    {
        tem = 0xC0 + b - 1;
        lower = tem & 0x0F;  //Lower 8-bits (0)
        upper = tem>>4;      //Upper 8-bits (C)
        commandLCD(upper);   //Set Row
        commandLCD(lower);   //Set Column
    }
}

void printCharLCD(char data)  //Send 8-bits through 4-bit mode
{
    char lowerBit,upperBit;
    lowerBit = data&0x0F;
    upperBit = data&0xF0;
    RS = 1;                    //Data mode
    setBitLCD(upperBit>>4);    //Send upper half by shifting by 4
    EN = 1;
    EN = 0;
    setBitLCD(lowerBit);       //Send Lower half
    EN = 1;
    EN = 0;
}

void printStringLCD(char *a)
{
    for(int i = 0; a[i] != '\0'; i++)
    {
       printCharLCD(a[i]);  
    }
}

void printNumLCD(int n, int digit)   
{
    int tem = 0;
    int divisor = 1;
    for (int i = 1; i < digit; ++i)
    {
        divisor *= 10;
    }

    for (int i = 0; i < digit; ++i)
    {
        tem = n/divisor;
        printCharLCD(tem+'0');
        n -= (tem*divisor);
        divisor /= 10; 
    }
}


