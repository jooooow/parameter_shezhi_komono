#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  

/*void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
*/

void delay_ms(unsigned int t);
void delay_us(unsigned int t);
void Delay_ms(unsigned int t);
void Delay_us(unsigned int t);


#endif
