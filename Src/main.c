#include "includes.h"
extern CircBuf_t * rx_buf;

int main(void){
	
	/* Enable the clock gate to ALL the Port module */
	PORT_ENABLE_CLK(A);		
	PORT_ENABLE_CLK(B);
	PORT_ENABLE_CLK(C);
	PORT_ENABLE_CLK(D);
	PORT_ENABLE_CLK(E);
	
	/* Initialize uart0 */
	uart0_Init(9600,0,0,8,1);
	
	/* Enable system interrupt */
	__enable_irq();

	while (1);
	
}
