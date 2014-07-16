/*nRF24L01.h*/
#define DBG 0



// CSN , chip select for SPI
#define nRF24CSNpin             RB3
#define nRF24CSNdir             TRISB3
#define SPIselectnRF24	 	nRF24CSNpin=0
#define SPIdeselectnRF24	nRF24CSNpin=1

#define nRF24irqpin		RB0

// CE for enable and disable TX and RX modes
#define nRF24CEpin		RB5		
#define nRF24CEdir              TRISB5
#define enableRF24		nRF24CEpin=1
#define disableRF24		nRF24CEpin=0






#define SETBIT(D,B) (D=D|(1<<B))
#define RESBIT(D,B) (D=D&(~(1<<B)))
#define TESBIT(D,B) (D&(1<<B))  //return the actual value from the bit


//****** Enhanced ShockBurst options *****

//address
#define AW_DEF 0x03  	// 2 bits, Address field width  3 (01) , 4 (10) or 5 (11) bytes
#define ADDR_SPACE 5   //number of bytes for the addresses 

// SETUP_RETR_REG  Automatic retransmission 
#define ARD_DEF 0x1 	//4 bits 250uS + value*250us   	resetValue:0000
#define ARC_DEF 0x3		//4 bits value					resetValue:0011

// RF_CH
#define RF_CH_DEF 0x02	//7 bits 2400 + RF_CH  MHz		resetValue: 0000010 (2402 MHz)

// RF_SETUP
#define PLL_LOCK_DEF 0 		//Force PLL lock signal (only 4 test)
#define RF_DR_DEF 0x1		//Speed transmission 0:1Mbps ; -> 1:2Mbps
#define RF_PWR_DEF 0x3		//Output power   def: 0dbm		

//CRC settings 
#define EN_CRC_DEF	1;
#define CRCO_DEF	1;		//CRC: 0: 1 byte,  1:2 bytes

// Address setup


#define  RX_ADDR 0xAAAAAAAAA0
#define  TX_ADDR 0xAAAAAAAAA1


	//Static payload




//Registers with bits
	//It is defined the lower position of the bit(s)

//CONFIG
#define nRF24_CONFIG_REG        0x00
	//bits
	#define PWR_UP      0x02
	#define PRIM_RX     0x01


#define nRF24_EN_AA_REG		0x01
	#define ENAA_P0 0x00
#define nRF24_EN_RXADDR_REG	0x02

#define nRF24_SETUP_AW_REG	0x03
	#define AW          0x00	//2 bits
#define nRF24_SETUP_RETR_REG	0x04
	#define ARD         0x4		//bits 7:4
	#define ARC         0x0		//bits 3:0
#define nRF24_RF_CH_REG		0x05
	#define RF_CH       0x0		//bits 6:0
#define nRF24_RF_SETUP_REG	0x06
	#define PLL_LOCK    0x4		//bit 4
	#define RF_DR       0x3		//bit 3
	#define RF_PWR      0x1		//bits 2:1
	#define LNA_HCURR	0x0	//bit 0
#define nRF24_STATUS_REG	0x07
	#define RX_DR       0x6		//bit 6  Data RX interrupt: data available in RX FIFO
	#define TX_DS       0x5		//bit 5	 Data sent TX FIFO interrupt. If AUTO_ACK: ack received
	#define MAX_RT      0x4		//bit 4	 Maximum number of TX retransmits interrupt
	#define RX_P_NO     0x1		//bits 3:1 Data pipe number for the payload available for reading from RX_FIFO
	#define TX_FULL     0x0		//bit 0  TX FIFO full flag.
	
#define nRF24_OBSERVE_TX_REG	0x08
#define nRF24_CD_REG		0x09

#define nRF24_RX_ADDR_P0_REG	0x0A
#define nRF24_RX_ADDR_P1_REG	0x0B
#define nRF24_RX_ADDR_P2_REG	0x0C
#define nRF24_RX_ADDR_P3_REG	0x0D
#define nRF24_RX_ADDR_P4_REG	0x0E
#define nRF24_RX_ADDR_P5_REG	0x0F
#define nRF24_TX_ADDR_REG		0x10

#define nRF24_RX_PW_P0_REG	0x11
#define nRF24_RX_PW_P1_REG	0x12
#define nRF24_RX_PW_P2_REG	0x13

//...


#define nRF24_FIFO_STATUS_REG   0x17
#define nRF24_DYNPD_REG 	0x1C
#define nRF24_FEATURE_REG 	0x1D
	#define EN_DPL      0x02	 //Enables Dynamic Payload Length
	#define EN_ACK_PAY  0x01	 //Enables Payload with ACK
	#define EN_DYN_ACK  0x00	 //Enables the W_TX_PAYLOAD_NOACK command
	

//commands definition
#define R_REGISTER 		0x00
#define W_REGISTER 		0x20
#define R_RX_PAYLOAD    0x61
#define W_TX_PAYLOAD    0xA0
#define FLUSH_TX 		0xE1
#define FLUSH_RX 		0xE2
#defien REUSE_TX_PL		0xE3
#define ACTIVATE		0x50		//follow by 0x73 activate:R_RX_PL_WID, W_ACK_PAYLOAD, W_TX_PAYLOAD_NOACK
#define R_RX_PL_WID		0x60
#define W_ACK_PAYLOAD	0x
#define W_TX_PAYLOAD_NOACK 0xB0		//send data, no payload
#define NOP_CMD			0xFF  //for read status


void delay_ms(char time);		
char nRF24_send_CMD(char cmd, char* txdata, char txbytecount, char* rxdata, char rxbytecount);
//always return the status register, the value to send or receive is exchange with pointers.
void nRF24_init();
char nRF24_read_reg(char reg, char * data_reg, char bytes_length);
char nRF24_write_reg(char reg, char * data_reg, char bytes_length);
char nRF24_set_bit(char reg,char bitpos);
char nRF24_res_bit(char reg,char bitpos);

/*
  MODE		PWR_UP	PRIM_RX   	CE
PowerDown	   0		-		-
	RX		   1		1		1
	TX		   1		0 		1
StandBy I 	   1	 	-		0
StandBy II	   1		0		1	// TXFIFO should be empty, if not RX mode.
*/
char nRF24_PD_mode();		//Power Down mode
char nRF24_RX_mode();		//Reception mode
char nRF24_TX_mode();
char nRF24_SBI_mode();		//Standby I mode
char nRF24_SBII_mode();		//StandbyII mode. if data in TX FIFO it will be sent 

char nRF24_setupEShockBurst(char);
char nRF24_setup_EShockBurst(char addressEspaceSize, char ard, char arc, char);
char nRF24_setup_RF(char rf_ch, char pll_lock, char rf_dr, char rf_pwr, char lna_hcurr);
char nRF24_setup_rx_data_pipe(unsigned char pipeID, char * address, char sizePL, char autoACK);
//sizePL=0 means automatic payload size