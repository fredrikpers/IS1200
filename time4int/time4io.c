#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"


int getsw(void)
{

int SW = (PORTD >> 8) & 0x000f; // Tar ut värdet på PORTD

return SW;
    
}
int getbtns(void)
{
 
    int BTN = (PORTD >> 5) & 0x0007;
    
    return BTN;
    
    
}
