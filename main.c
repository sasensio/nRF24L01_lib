/* 
 * File:   main.c
 * Author: samuel
 *
 * Created on July 15, 2014, 6:31 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "spi.h"
#include "nRF24L0.h"
#include "16f88_lib.h"



#pragma config WDTE=OFF,  FOSC=XT,  CP = OFF, PWRTE = OFF, CCPMX = RB0,  DEBUG = OFF,  LVP = OFF,  BOREN = OFF,  MCLRE = ON, FCMEN = OFF & IESO = OFF

char TX_addr[5];
char RX_addr_P0[5];
char RX_addr_P1[5];
/* 
char RX_addr_P2;
char RX_addr_P3;
char RX_addr_P4;
char RX_addr_P5; 
*/

char RXTX_status;	/*
					bit 0:	1= data pending to read
					bit 1: 	1= last TX was successful
					bit	2:	1= TX error
					*/


void interrupt int_rutine(void)
{
	char data;
	if (INT0IF)
	{
		data=nRF24_send_CMD(NOP_CMD, 0, 0, 0, 0);		//get status regiter
		if TESBIT(data,RX_DR)
		{
			//data available to read
			RXTX_status |= 0x1;		//enable bit 0
			SETBIT(data,RX_DR); // clear interruption
		}
		if TESBIT(data,TX_DS)
		{
			RXTX_status |= 0x2;			//enable bit 1
			//data sent succesfully
			SETBIT(data,TX_DS); // clear interruption
		}
		if TESBIT(data,MAX_RT)
		{
			RXTX_status |= 0x4;		//enable bit 2
			//data was not sent
			SETBIT(data,MAX_RT); // clear interruption
		}
	
		nRF24_write_reg(nRF24_STATUS_REG,&data,1);	
	}
} 


int main(int argc, char** argv) {

    while(1)
    {
        spi_init();
        nRF24_init();
        RB3=1;
        wait_ms(1);
        RB3=0;
        nRF24_PD_mode();                //Power Down mode
        RB3=1;
        wait_ms(2);
        RB3=0;
        nRF24_RX_mode();		//Reception mode
        RB3=1;
        wait_ms(3);
        RB3=0;
        nRF24_TX_mode();
		
		
		
	/*	Manage interruptions
		SLEEP();
		NOP; 
		if (RXTX_status)
			manage_interruption();
	*/		
    }

    return (EXIT_SUCCESS);
}

void manage_interruption()
{
	//thing to do when an interruption is trigered

}

