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

volatile int* portE = (volatile int*) 0xbf886110; // initierar pekare portE

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{

  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    
    volatile int* trise = (volatile int*) 0xbf886100; // intierar en pekare trise
    
    *trise = *trise & 0xff00; // Maskar trise
    
    *portE = 0; // Sätter portE till 0
    
    TRISD = TRISD & 0x0fe0; //Tar fram bitarna 11 till 5 för switcharna och knapparna.
      
}

/* This function is called repetitively from the main program */
void labwork( void )
{

        int sw = getsw(); // Hämtar info från time4io
        int btn = getbtns(); //Hämtar info från time4io
    
    if(btn == 1 || btn == 3 || btn == 5 || btn == 7) //Button 2
    {
    
        mytime = mytime & 0xff0f; //Rätt position i mytime
        mytime = sw << 4 | mytime; //Lägger värdet på sw på rätt position i mytime och ändrar inte det som redan står
    }
    
    if(btn == 2 || btn == 3 || btn == 6 || btn == 7) //Button 3
    {
        mytime = mytime & 0xf0ff; //Rätt position i mytime
        mytime = sw << 8 | mytime; //Lägger värdet på sw på rätt position i mytime och ändrar inte det som redan står
    }
    
    if(btn == 4 || btn == 5 || btn == 6 || btn == 7) //Button 4
    {
        mytime = mytime & 0x0fff; //Rätt position i mytime
        mytime = sw << 12 | mytime; //Lägger värdet på sw på rätt position i mytime och ändrar inte det som redan står
    }
        
  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
    *portE +=1; // lägger till +1 för att gå framåt
  display_image(96, icon);
}
