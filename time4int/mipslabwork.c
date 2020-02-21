/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

int prime = 1234567;

volatile int* portE = (volatile int*) 0xbf886110; // initierar pekare portE

int timeoutcount = 0; //Initierar counter

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */

void user_isr(void)
{
    if(IFS(0) & 0x100)  // Körs om bit 8 är 1.
    {
        timeoutcount++; //Plussar på counter
        IFSCLR(0)=0x100; // Nollställer IFS

       }
        if(timeoutcount == 10) // Körs var 10 gång
        {
            time2string( textstring, mytime );
            display_string( 3, textstring );
            display_update();
            tick( &mytime );
            timeoutcount=0; // Nollställer counter
        
        }
}

void labinit( void )
{
    
    volatile int* trise = (volatile int*) 0xbf886100; // intierar en pekare trise
    
    *trise = *trise & 0xff00; // Maskar trise
    
    *portE = 0; // Sätter portE till 0
    
    TRISD = TRISD & 0x0fe0; //"nollar allt förutom de bitar vi vill ha 11-5
    
    PR2 = ((80000000/256)/10); // Ger oss 100 ms
    TMR2 = 0; // Resettar timern
    T2CONSET = 0x70; //1:256 sätter timer på prescale 1:256
    T2CONSET = (1 << 15); // Startar timern
    
    IEC(0) = 0x100; //Sätter bit 8 till 1 för att enabla T2IE
    IPC(2) = 0x1C; // Bit 2:4 ska sättas till 1.
    enable_interrupts();
}

void labwork( void )
{
    prime = nextprime( prime );
    display_string( 0, itoaconv( prime ) );
    display_update();
}
