#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"


int getsw(void)
{

int SW = (PORTD >> 8) & 0x000f; //Bit 11 till 8 läser av värdet på switcharna.
    

return SW;
    
}
int getbtns(void)
{
 
    int BTN = (PORTD >> 5) & 0x0007; // Läser av värdet på bitarna 5-7.
    
    return BTN;
    
    
}
