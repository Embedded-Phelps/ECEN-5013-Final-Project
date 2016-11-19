#include "includes.h"

int main(void){
	int32_t temp;
	/* Enable the clock gate to ALL the Port module */
	PORT_ENABLE_CLK(A);		
	PORT_ENABLE_CLK(B);
	PORT_ENABLE_CLK(C);
	PORT_ENABLE_CLK(D);
	PORT_ENABLE_CLK(E);
	
	/* Enable system interrupt */
	__enable_irq();
	app_ADCInit();

	while (1){
		temp = read_OnChipTemperature();
		temp = temp;
	}
}
