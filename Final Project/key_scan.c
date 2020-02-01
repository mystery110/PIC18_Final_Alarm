#include "key_scan.h"
#include <xc.h>

int keyread(void){
    int number;
    
    C1=0;
    C2=C3=C4=1;
    if(R1 == 0){
        number = 1; //press 1
        while(R1==0);
        return number;
    }
    if(R2 == 0){
        number = 4; //press 4
        while(R2==0);
        return number;
    }
    if(R3 == 0){
        number = 7; //press 7
        while(R3==0);
        return number;
    }
    if(R4 == 0){
        number = 14; //press *
        while(R4==0);
        return number;
    }
    
    C2=0;
    C1=C3=C4=1;
    if(R1 == 0){
        number = 2;
        while(R1==0);
        return number;
    }
    if(R2 == 0){
        number = 5;
        while(R2==0);
        return number;
    }
    if(R3 == 0){
        number = 8;
        while(R3==0);
        return number;
    }
    if(R4 == 0){
        number = 0;
        while(R4==0);
        return number;
    }
    
    C3=0;
    C1=C2=C4=1;
    if(R1 == 0){
        number = 3;
        while(R1==0);
        return number;
    }
    if(R2 == 0){
        number = 6;
        while(R2==0);
        return number;
    }
    if(R3 == 0){
        number = 9;
        while(R3==0);
        return number;
    }
    if(R4 == 0){
        number = 15;
        while(R4==0);
        return number;
    }
    
    C4=0;
    C1=C2=C3=1;
    if(R1 == 0){
        number = 10;
        while(R1==0);
        return number;
    }
    if(R2 == 0){
        number = 11;
        while(R2==0);
        return number;
    }
    if(R3 == 0){
        number = 12;
        while(R3==0);
        return number;
    }
    if(R4 == 0){
        number = 13;
        while(R4==0);
        return number;
    }
    
    if(R1 == 1 && R2 == 1 && R3 == 1 && R4 == 1)
        return -1;
}
