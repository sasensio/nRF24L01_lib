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
    }

    return (EXIT_SUCCESS);
}

