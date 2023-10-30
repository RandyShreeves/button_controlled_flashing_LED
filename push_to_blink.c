// 0. Documentation Section
// This program allows a user to push a button which causes an LED to blink.
// The LED is in the on state when the switch it not pressed.
// The LED blinks at ~5Hz or 100ms intervals while button is pressed.
// Author: Randy Shreeves
// Date: 10/30/2023

// 1. Pre-processor Directives Section
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0xE000E018))

// 2. Global Declarations Section
void port_e_initialization(void);
void sys_tick_initialization(void);
void delay_100ms(void);

// 3. Subroutines Section
int main(void){ 
	// Setup
	unsigned long input_switch;
	port_e_initialization();
	sys_tick_initialization();
	GPIO_PORTE_DATA_R |= 0x02; // LED starts in the on state
	// Loop
  while(1){
		input_switch = GPIO_PORTE_DATA_R&0x01;
		delay_100ms();
		if (input_switch == 0x01){		// if switch is pressed
			GPIO_PORTE_DATA_R ^= 0x02;	// toggle LED
		}
		else{
			GPIO_PORTE_DATA_R |= 0x02;	// else, turn LED on
		}	
  }// end while loop
}// end main

void port_e_initialization(void){
	unsigned long volatile delay; // delay for 3-5 bus cycles for clock to stabilize
	SYSCTL_RCGC2_R |= 0x10;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AMSEL_R &= ~0x03;
  GPIO_PORTE_PCTL_R &= ~0x000000FF;
	GPIO_PORTE_DIR_R |= 0x02;
	GPIO_PORTE_DIR_R &= ~0x01;
	GPIO_PORTE_AFSEL_R &= ~0x03;
	GPIO_PORTE_DEN_R |= 0x03;
} // end portE_initialization

void sys_tick_initialization(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 1599999;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}//end SysTick_initialization

void delay_100ms(void){
	while((NVIC_ST_CTRL_R&0x00010000) == 0) {
		// wait for count flag to be set
  }
}// end delay_1ms()
