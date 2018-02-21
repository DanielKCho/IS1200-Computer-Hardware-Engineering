#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>

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
#define TEMP_SENSOR_ADDR 0x50

char textbuffer[4][16];
int reset;



/* THREE DIFFERENT WALLS */
const uint8_t const wall[2][8] = {

	{0, 0, 0, 0, 255, 255, 255, 255},
	

};
void user_isr(void){
	IFS(0) &= ~0x80000;
	reset = 1;

}
/* Runner object*/
const uint8_t const runner[] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};




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

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}

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

void display_runner(uint8_t page){
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	spi_send_recv(0x22);			//Set page address
	spi_send_recv(page);			//Set which page

	spi_send_recv(0x21);			//set Column address
	spi_send_recv(0x18 & 0xF);					//Lower 4 bits
	spi_send_recv(0x10 | ((0x18 >> 4) & 0xF)); // upper 4 bits


	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
	int i;
	for(i = 0; i < 11; i++){
		spi_send_recv(runner[i]);
   }
}

void display_obstacles(uint8_t column, int g){
	int i;
	for(i = 0; i < 4; i++){
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	spi_send_recv(0x22); 	//set page address
	spi_send_recv(i);		//set current page
	spi_send_recv(0x21); 	//set column address
	spi_send_recv(column & 0xF);	//set column lower bits
	spi_send_recv(0x10 | ((column >> 4) & 0xF)); 	//set column upper bits
	DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
	int j;
	for(j = 0; j < 2; j++)
		spi_send_recv(wall[g-1][2*i + j]);
	} 
}

void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x21);
		spi_send_recv(0 & 0xF); // column lower 4 bit
		spi_send_recv(0x10 | ((0 >> 4) & 0xF)); // upper 4 bits
		
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




void display_number(int column, int row, char x){
	int j;
		
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22); // = set page
		spi_send_recv(row); // = which page

		spi_send_recv(0x21); // set column address
		spi_send_recv(column & 0xF); // column lower 4 bit
		spi_send_recv(0x10 | ((column >> 4) & 0xF)); // upper 4 bits

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for(j = 0; j < 8; j++)
			spi_send_recv(font[x*8 + j]);
}

void display_score(int column, int row, uint8_t score){

	int n,m;
	if(score >= 100){
		display_number(column, row, '1');
		display_number((column + 8), row, '0');
		display_number((column+16), row, '0');
	}
	else{
	for(n = 1; n *10 < 100; n++)
		if(n*10 > score){
			n--;
			break;
		}
	score -= n*10;
	for(m = 0; m < 10; m++)
		if(m == score)
			break;
	n += 48;
	m += 48;
	if(n != 48){
		display_number(column, row, (char)(n));
	}
	display_number((column+8), row, (char)(m));
	} 		
}

void resetBuffer(void){
	int i, j;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 16; j++)
			textbuffer[i][j] = '\0';
}

int main(void) {
	//Switch
	 IPC(4) = 0x1C000000;
 	 IEC(0) = (0x1 << 19);
 	 enable_interrupt();


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


	display_init();

	//Game variable initialization
	uint8_t page, obstacle1, obstacle2, alive,jump, hold, score;
	int falling, fallingCount, delayCount;
	

	/*
	page - keeps track of which page the runner is on
	obstacle1- keeps track of which column obstacle1 is on
	obstacle2- keeps track of which column obstacle2 is on
	alive- is 1 when runner is alive and 0 when runner is dead
	jump- is 1 when a jump command has been  issued
	score- keeps track of how many obstacles the runner has passed
	hold- used to avoid holding the jump button
	*/
 
	//Starting the game, defining button hold and jumping
	
		while(1){
		obstacle2 = jump = score = hold= 0;
		alive = 1;
		page = 3;
		falling = 0;
		reset = 0;			//Resetting the reset variable so that we can jump into the forloop again.

		while(alive){			
			if(reset){			//Able to reset the game even when our runner is alive. 
				alive = 0;
			}

			for(obstacle1 = 127; obstacle1 >= 0 && !reset; obstacle1--){
				PORTE = score;
				jump = ((PORTD >> 7) & 1);
				display_update();

				if(jump && !hold){
					if(page == 3)
						page = page - 3;
					hold = 1;
				}
				//Falling back to ground-level from mid-air.
				else {
					falling++;
				
				if(falling == fallingCount){
					if(page != 3)
						page++;
					falling = 0;
				}
				}
				if (!jump)
					hold = 0;

				if(score < 7){
					fallingCount = 20;
					delayCount = 200000;
				}else{
					fallingCount = 20;
					delayCount = (100000);
				}

				//Die if you hit an obstacle
				if(obstacle1 < 35 && obstacle1 > 23 && page > 1)
					alive = 0;

				if(alive){

				
				
				if(obstacle1 < 127 && obstacle1 > 1)
					display_obstacles(obstacle1, 1);

				if(obstacle1 == 0){
					obstacle1 = 127;
					
				}

				display_runner(page);
				delay(delayCount);
			

				//Passing by walls, counting score
				if(obstacle1 == 20 && alive)
					score++;
				
				
			}
				if(alive == 0){
					display_string(3, "Your score: ");
						
						display_update();
						display_score(90, 3, score);
						delay(1000000 * 2);
						resetBuffer();

					
					//display_number(100, 2, numbers[0]);
				

				}

			}

		}	

		
		}



		

	return 0;
	
}
