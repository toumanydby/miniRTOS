#include "stm32f10x.h"

#define STACK_1_ADDR 0x20001000
#define STACK_2_ADDR 0x20003000
#define XPSR_DEFAULT 0x10000000
#define LR_DEFAULT 0xFFFFFFFD


uint32_t *stack1 = (uint32_t *) STACK_1_ADDR;
uint32_t *stack2 = (uint32_t *) STACK_2_ADDR;

char current_process = 0;
char setup = 0;

void SysTick_Handler(void) {
	if(current_process) {
		stack2 = (uint32_t *) __get_PSP();
		__set_PSP((uint32_t)stack1);
	} else {
		stack1 = (uint32_t *) __get_PSP();
		__set_PSP((uint32_t)stack2);
	}
	
	current_process = !current_process;
	return;
}

void dummy1(void){
	int a = 0;
	while(1){
		a++;
	}
}

void dummy2(void){
	while(1){
		int a = 20;
		a++;
	}
}

void init_stacks(){
	for (int i = 0; i<6; i++) {
		stack1[i] = 0;
		stack2[i] = 0;
	}
	
	stack1[5] = LR_DEFAULT;
	stack2[5] = LR_DEFAULT;
	
	stack1[6] = (uint32_t) dummy1;
	stack2[6] = (uint32_t) dummy2;
	
	stack1[7] = XPSR_DEFAULT;
	stack2[7] = XPSR_DEFAULT;
	
}

int main ( void )
{
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE ;
	SysTick->LOAD = 72000 - 1;
	
	init_stacks();
	
	__set_PSP((uint32_t)stack1);
	
	uint32_t ctrl = __get_CONTROL();
	ctrl |= 3;
	__set_CONTROL(ctrl);
	
	while (1)
	{
	}
}
