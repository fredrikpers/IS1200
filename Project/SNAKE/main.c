#include <pic32mx.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

char display[128][32];
char textbuffer[4][16];
int snake_array[500][2];

//Taken from the "Hello-display"
static const uint8_t const font[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 94, 0, 0, 0, 0,
    0, 0, 4, 3, 4, 3, 0, 0,
    0, 36, 126, 36, 36, 126, 36, 0,
    0, 36, 74, 255, 82, 36, 0, 0,
    0, 70, 38, 16, 8, 100, 98, 0,
    0, 52, 74, 74, 52, 32, 80, 0,
    0, 0, 0, 4, 3, 0, 0, 0,
    0, 0, 0, 126, 129, 0, 0, 0,
    0, 0, 0, 129, 126, 0, 0, 0,
    0, 42, 28, 62, 28, 42, 0, 0,
    0, 8, 8, 62, 8, 8, 0, 0,
    0, 0, 0, 128, 96, 0, 0, 0,
    0, 8, 8, 8, 8, 8, 0, 0,
    0, 0, 0, 0, 96, 0, 0, 0,
    0, 64, 32, 16, 8, 4, 2, 0,
    0, 62, 65, 73, 65, 62, 0, 0,
    0, 0, 66, 127, 64, 0, 0, 0,
    0, 0, 98, 81, 73, 70, 0, 0,
    0, 0, 34, 73, 73, 54, 0, 0,
    0, 0, 14, 8, 127, 8, 0, 0,
    0, 0, 35, 69, 69, 57, 0, 0,
    0, 0, 62, 73, 73, 50, 0, 0,
    0, 0, 1, 97, 25, 7, 0, 0,
    0, 0, 54, 73, 73, 54, 0, 0,
    0, 0, 6, 9, 9, 126, 0, 0,
    0, 0, 0, 102, 0, 0, 0, 0,
    0, 0, 128, 102, 0, 0, 0, 0,
    0, 0, 8, 20, 34, 65, 0, 0,
    0, 0, 20, 20, 20, 20, 0, 0,
    0, 0, 65, 34, 20, 8, 0, 0,
    0, 2, 1, 81, 9, 6, 0, 0,
    0, 28, 34, 89, 89, 82, 12, 0,
    0, 0, 126, 9, 9, 126, 0, 0,
    0, 0, 127, 73, 73, 54, 0, 0,
    0, 0, 62, 65, 65, 34, 0, 0,
    0, 0, 127, 65, 65, 62, 0, 0,
    0, 0, 127, 73, 73, 65, 0, 0,
    0, 0, 127, 9, 9, 1, 0, 0,
    0, 0, 62, 65, 81, 50, 0, 0,
    0, 0, 127, 8, 8, 127, 0, 0,
    0, 0, 65, 127, 65, 0, 0, 0,
    0, 0, 32, 64, 64, 63, 0, 0,
    0, 0, 127, 8, 20, 99, 0, 0,
    0, 0, 127, 64, 64, 64, 0, 0,
    0, 127, 2, 4, 2, 127, 0, 0,
    0, 127, 6, 8, 48, 127, 0, 0,
    0, 0, 62, 65, 65, 62, 0, 0,
    0, 0, 127, 9, 9, 6, 0, 0,
    0, 0, 62, 65, 97, 126, 64, 0,
    0, 0, 127, 9, 9, 118, 0, 0,
    0, 0, 38, 73, 73, 50, 0, 0,
    0, 1, 1, 127, 1, 1, 0, 0,
    0, 0, 63, 64, 64, 63, 0, 0,
    0, 31, 32, 64, 32, 31, 0, 0,
    0, 63, 64, 48, 64, 63, 0, 0,
    0, 0, 119, 8, 8, 119, 0, 0,
    0, 3, 4, 120, 4, 3, 0, 0,
    0, 0, 113, 73, 73, 71, 0, 0,
    0, 0, 127, 65, 65, 0, 0, 0,
    0, 2, 4, 8, 16, 32, 64, 0,
    0, 0, 0, 65, 65, 127, 0, 0,
    0, 4, 2, 1, 2, 4, 0, 0,
    0, 64, 64, 64, 64, 64, 64, 0,
    0, 0, 1, 2, 4, 0, 0, 0,
    0, 0, 48, 72, 40, 120, 0, 0,
    0, 0, 127, 72, 72, 48, 0, 0,
    0, 0, 48, 72, 72, 0, 0, 0,
    0, 0, 48, 72, 72, 127, 0, 0,
    0, 0, 48, 88, 88, 16, 0, 0,
    0, 0, 126, 9, 1, 2, 0, 0,
    0, 0, 80, 152, 152, 112, 0, 0,
    0, 0, 127, 8, 8, 112, 0, 0,
    0, 0, 0, 122, 0, 0, 0, 0,
    0, 0, 64, 128, 128, 122, 0, 0,
    0, 0, 127, 16, 40, 72, 0, 0,
    0, 0, 0, 127, 0, 0, 0, 0,
    0, 120, 8, 16, 8, 112, 0, 0,
    0, 0, 120, 8, 8, 112, 0, 0,
    0, 0, 48, 72, 72, 48, 0, 0,
    0, 0, 248, 40, 40, 16, 0, 0,
    0, 0, 16, 40, 40, 248, 0, 0,
    0, 0, 112, 8, 8, 16, 0, 0,
    0, 0, 72, 84, 84, 36, 0, 0,
    0, 0, 8, 60, 72, 32, 0, 0,
    0, 0, 56, 64, 32, 120, 0, 0,
    0, 0, 56, 64, 56, 0, 0, 0,
    0, 56, 64, 32, 64, 56, 0, 0,
    0, 0, 72, 48, 48, 72, 0, 0,
    0, 0, 24, 160, 160, 120, 0, 0,
    0, 0, 100, 84, 84, 76, 0, 0,
    0, 0, 8, 28, 34, 65, 0, 0,
    0, 0, 0, 126, 0, 0, 0, 0,
    0, 0, 65, 34, 28, 8, 0, 0,
    0, 0, 4, 2, 4, 2, 0, 0,
    0, 120, 68, 66, 68, 120, 0, 0,
    
};

int timeoutcount;
int random_counter;
char distance = 'r';
int fruit_x = 80;
int fruit_y = 20;
int snake_x = 20;
int snake_y = 20;
int snake_length = 10;
int rand_i;
int rand_j;
volatile int* portE = (volatile int*) 0xbf886110; // initierar pekare portE

//Taken from the "Hello-display"
void delay(int cyc)
{
	int i;
	for(i = cyc; i > 0; i--);
}
//Taken from the "Hello-display"
uint8_t spi_send_recv(uint8_t data)
{
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}
//Taken from the "hello-display"
void display_update2() {
    int i, j, k;
    int c;
    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);
        
        spi_send_recv(0x0);
        spi_send_recv(0x10);
        
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
        for(j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if(c & 0x80)
                continue;
            
            for(k = 0; k < 8; k++)
                spi_send_recv(font[c*8 + k]);
        }
    }
}
//Taken from the "Hello-display"
void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4)
        return;
    if(!s)
        return;
    
    for(i = 0; i < 16; i++)
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}
//Taken from Labb3
int getbtns(void)
{
    int BTN = (PORTD >> 5) & 0x0005; // Läser av värdet på bitarna 5-7.
   
    return BTN;
}
//Taken from the "Hello-display"
void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}
//Taken from the "Hello-display"
void display_update()
{
    int y, j, k;
    int value;
    for(y = 0; y < 4; y++) {
        
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(y);
        
        spi_send_recv(0x0);
        spi_send_recv(0x10);
        
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
        for(j = 0; j < 128; j++)
        {
            value = (display[j][y*8]);
            
            for(k = 1; k < 8; k++)
            value |= (display[j][y*8 +k]) << k;
            
            spi_send_recv(value);
        }
    }
}
void set_pixel(int x, int y) {

    display[x][y] = 1;
}
void remove_pixel(int x, int y ){
    
    display[x][y] = 0;
}
void reset_snake()
{
    int i;
    for(i = 0; i < snake_length; i++)
       {
           snake_array[i][0] = 0;
           snake_array[i][1] = 0;
       }
}
void spelplan()
{
    int i,j,k;
    
    for(j = 0; j < 128; j++)
    {
        for(k = 0; k < 32; k++)
            remove_pixel(j,k);
    }
    
    for(i = 0; i < 128; i++)
    {
        set_pixel(i,0);
        set_pixel(i,31);
    }
    
    for(i = 0; i < 31; i++)
    {
        set_pixel(0,i);
        set_pixel(127,i);
    }
}
void print_snake()
{
    int i,x,y;
    for(i = 0; i < snake_length; i++)
    {
        x = snake_array[i][0];
        y = snake_array[i][1];
        set_pixel(x,y);
    }
}
void start_body()
{
    int i,j;
    int value = 20;
    for(i = 0; i < snake_length; i++)
    {
        snake_array[i][0] = value;
        snake_array[i][1] = 20;
        value--;
    }
    print_snake();
}
void set_fruit()
{
    set_pixel(fruit_x, fruit_y);
    set_pixel(fruit_x-1,fruit_y);
    set_pixel(fruit_x,fruit_y-1);
    set_pixel(fruit_x-1,fruit_y-1);
}

void remove_fruit()
{
    remove_pixel(fruit_x, fruit_y);
    remove_pixel(fruit_x-1,fruit_y);
    remove_pixel(fruit_x,fruit_y-1);
    remove_pixel(fruit_x-1,fruit_y-1);
}
void reset()
{
    display_string(0, "GAME OVER!!");
    display_string(1, "GAME OVER!!");
    display_string(2, "GAME OVER!!");
    display_string(3, "GAME OVER!!");
    display_update2();
    delay(30000000);

    *portE = 0;
    spelplan();
    reset_snake();
    snake_x = 20;
    snake_y= 20;
    distance = 'r';
    snake_length = 5;
    display_update();
}
void hit_yourself()
{
    int i;
    for(i = 1; i < snake_length; i++)
    {
       if(snake_array[i][0] == snake_x && snake_array[i][1] == snake_y)
           reset();
    }
}
void distances()
{
    switch(distance)
    {
        case 'r':
            snake_x++;
            if(snake_x == 127) //resettar om körs i väggen
                reset();
            break;
            
        case 'l':
            snake_x--;
            if(snake_x == 0) //resettar om körs i väggen
                reset();
            break;
            
        case 'u':
            snake_y--;
            if(snake_y == 0) //resettar om körs i väggen
                reset();
            break;
        
        case 'd':
            snake_y++;
            if(snake_y == 31) //resettar om körs i väggen
                reset();
            break;
    }
}
void update_distance()
{
    int btn = getbtns(); //HÄMTAR STATUS PÅ BUTTONS

   switch(distance)
   {
       case 'r':
           if(btn == 1){ //BUTTON 2
               distance = 'd';
               delay(2000000);
           }
           else if(btn == 4){// BUTTON 4
               distance = 'u';
               delay(2000000);
           }
           break;
       
       case 'l':
           if(btn == 1){ //BUTTON 2
               distance = 'u';
               delay(2000000);
           }
           else if(btn == 4){ // BUTTON 4
               distance = 'd';
               delay(2000000);
           }
           break;
       
       case 'u':
           if(btn == 1){ //BUTTON 2
               distance = 'r';
               delay(2000000);

           }
           else if(btn == 4){ // BUTTON 4
               distance = 'l';
               delay(2000000);
           }
            break;
        
       case 'd':
           if(btn == 1){ //BUTTON 2
               distance = 'l';
               delay(2000000);
           }
               
           else if(btn == 4){ // BUTTON 4
               distance = 'r';
               delay(2000000);
           }
           break;
   }
}
void update_snake()
{
    int i,j;
    int k = snake_length-1;
    for(i = 1; i < snake_length ; i++)
    {
        for(j = 0; j < 2; j++)
        {
            snake_array[k][j] = snake_array[k-1][j];
        }
            k--;
    }
    snake_array[0][0] = snake_x;
    snake_array[0][1] = snake_y;
    print_snake();
}

//Taken from labb3
void labinit()
{
    TRISD = TRISD & 0x00e0; //Tar fram bitarna 7 till 5 för knapparna.
    PR2 = ((80000000/256)/20); // Ger oss 100 ms
    TMR2 = 0; // Resettar timern
    T2CONSET = 0x70; //1:256 sätter timer på prescale 1:256
    T2CONSET = (1 << 15); // Startar timern

    PR3 = ((80000000/256)/100);
    TMR3 = 0;
    T3CONSET = 0x0070; //1:256 sätter timer på prescale 1:256
    T3CONSET = (1 << 15); // Startar timern
    
    volatile int* trise = (volatile int*) 0xbf886100; // intierar en pekare trise
    
    *trise = *trise & 0xff00; // Maskar trise
}

void random_fruit()
{
    if(IFS(0) & 0x1000)
    {
        random_counter++;
        IFSCLR(0) = 0x1000;
    }
     if(random_counter == 1)
     {
         rand_i = TMR3 % 123 + 4;
         rand_j = TMR3 % 26 + 4;
         random_counter = 0;
    }
}
void fruit_eaten()
{
    if(snake_x == fruit_x && snake_y == fruit_y || snake_x == fruit_x-1 && snake_y == fruit_y || snake_x == fruit_x && snake_y == fruit_y-1 || snake_x == fruit_x-1 && snake_y == fruit_y){
        remove_fruit();
        fruit_x = rand_i;
        fruit_y = rand_j;
        snake_length++;
        *portE +=1;

    }
}
//Taken from labb3 but has been changed
void user_isr(void)
{
    if(IFS(0) & 0x100){  // Loop körs när IFS bit 0x100 är 1
        timeoutcount++; // Counter ++
        IFSCLR(0)=0x100; // Sätter IFS bit 0x100 till 0
    }
        if(timeoutcount == 1) // Körs när counter är 1
        {
            spelplan();
            distances();
            update_distance();
            fruit_eaten();
            set_fruit();
            hit_yourself();
            update_snake();
            random_fruit();
            display_update();
            timeoutcount = 0;
        }
}
//Taken from the "Hello-display" but has been changed
int main(void)
{
        
    /* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
    
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;
        
    start_body();
   
    labinit();
    
    display_init();
    
    display_update();
    
        while(1)
        {
            user_isr();
        }
        
	for(;;) ;
    
	return 0;
}
