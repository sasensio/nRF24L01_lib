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


int main(int argc, char** argv) {

    while(1)
    {
        spi_init();
        nRF24_init();
        wait_ms(1);
        nRF24_PD_mode();		//Power Down mode
        wait_ms(2);
        nRF24_RX_mode();		//Reception mode
        wait_ms(3);
        nRF24_TX_mode();
    }

    return (EXIT_SUCCESS);
}

