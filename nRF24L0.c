/*nRF24L01.c*/

#include "nRF24L0.h"
#include "../spi_lib/spi.h"
#include <xc.h>




char nRF24_send_CMD(char cmd, char* txdata, char txbytecount, char* rxdata, char rxbytecount)
{
	char nRF24_status;
	char txi=0;
	char rxi=0;
	char txbytes=txbytecount;   //txbytecount in the function
	char rxbytes=rxbytecount;
	
	SSPBUF = cmd;
	SPIselectnRF24;
	SSPEN=1;;
	while (!BF){}
        BF=0;
	nRF24_status=SSPBUF;  //during the command transmission the status register is received. 
	while (txbytes || rxbytes)
	{
		if (txbytes)
		{
			SSPBUF=*(txdata+txi);
			txbytes--;
		}		
		else
			SSPBUF=0;
		while (!BF){}
		if(rxbytes)
		{
			*(rxdata+rxi)=SSPBUF;
			rxbytes--;
		}
		BF=0;
	}
	SSPEN=0;
	SPIdeselectnRF24;
	return (nRF24_status);
}

void nRF24_init()
{
	nRF24CSNdir=0; //output
	nRF24CEdir=0;	
	
	SPIdeselectnRF24;
	disableRF24;
	spi_init();
	nRF24_PD_mode();
}


char nRF24_read_reg(char reg, char * data_reg, char bytes_length)
{
    char data;
    char tmp = (R_REGISTER | reg);
    data = nRF24_send_CMD(tmp, 0, 0,data_reg,bytes_length);//CMD: 000A AAAA, where A is the address from  the register
    
    return (data);
}

char nRF24_write_reg(char reg, char * data_reg, char bytes_length)
{
	return nRF24_send_CMD(W_REGISTER|reg, data_reg,bytes_length,0,0);   //CMD: 001A AAAA, where A is the address from  the register
}

char nRF24_set_bit(char reg,char bitpos)  //return the status from the register.
{
	char data;
	nRF24_read_reg(reg,&data,1);
	if TESBIT(data,bitpos)
		return (data);
	
	SETBIT(data,bitpos);
	nRF24_write_reg(reg,&data,1);
	if (DBG)			//if debug then request the register we have changed
	{
		data=0;
		nRF24_read_reg(reg,&data, 1);
	}
	return (data);
}

char nRF24_res_bit(char reg,char bitpos)  //return the status from the register.
{
	char data;
        char result;

	result = nRF24_read_reg(reg,&data,1);
	//if !(TESBIT(data,bitpos)) 
        if (data&(1<<bitpos))
		return (data);
	
	RESBIT(data,bitpos);
	nRF24_write_reg(reg,&data,1);
	if (DBG)			//if debug then request the register we have changed
	{
		data=0;
		nRF24_read_reg(reg,&data,1);
	}
	return (data);
}

char nRF24_PD_mode()    //Power Down mode
{
	char data;
	char status;
	
	status=nRF24_read_reg(nRF24_CONFIG_REG,&data,1);
	if (TESBIT(data,PWR_UP))								//check bits to avoid rewrite
	{
		RESBIT(data,PWR_UP);    
		nRF24_write_reg(nRF24_CONFIG_REG, &data, 1 );
	}
	disableRF24;
	return data;
}	


char nRF24_RX_mode()		//Reception mode
{
    char data;
    char status;
	
	status=nRF24_read_reg(nRF24_CONFIG_REG,&data,1);	
	if (!(TESBIT(data,PWR_UP) && TESBIT(data,PRIM_RX)))		//check bits to avoid rewrite
	{
		SETBIT(data,PWR_UP); 
		SETBIT(data,PRIM_RX);
		nRF24_write_reg(nRF24_CONFIG_REG, &data, 1 );
	}
	enableRF24;
	return (data);
}


char nRF24_TX_mode()
{
    char data;
    char status;
	
	status=nRF24_read_reg(nRF24_CONFIG_REG,&data,1);
	if (!(TESBIT(data,PWR_UP) && (!TESBIT(data,PRIM_RX))))		//check bits to avoid rewrite
	{
		SETBIT(data,PWR_UP); 
		RESBIT(data,PRIM_RX);
		nRF24_write_reg(nRF24_CONFIG_REG, &data, 1 );	
	}
	enableRF24;
	return data;
}
char nRF24_SBI_mode()		//Standby I mode
{
	char data;
	char status;
	
	status=nRF24_read_reg(nRF24_CONFIG_REG,&data,1);
	if (!(TESBIT(data,PWR_UP)))								//check bits to avoid rewrite
	{
		SETBIT(data,PWR_UP);
		nRF24_write_reg(nRF24_CONFIG_REG, &data, 1 );
	}
	disableRF24;
	return data;
}

char nRF24_SBII_mode()		//StandbyII mode
{
	char data;
	char status;
	
	status=nRF24_read_reg(nRF24_CONFIG_REG,&data,1);
	if (!((TESBIT(data,PWR_UP) && !TESBIT(data,PRIM_RX))))		//check bits to avoid rewrite
	{
		SETBIT(data,PWR_UP);
		RESBIT(data,PRIM_RX);
		nRF24_write_reg(nRF24_CONFIG_REG, &data, 1 );	
	}
	enableRF24;
	return data;
}

char setup_EShockBurst(char addressEspaceSize, char ard, char arc, char)
{
	
	char data=0x00;
	char ret;
	nRF24_write_reg(nRF24_SETUP_AW_REG,&addressEspaceSize,1);
	
	data= ard<<ARD | arc<<ARC;			//prepare SETUP_RETR
	ret= nRF24_write_reg(nRF24_SETUP_RETR_REG,&data,1);
	return ret;
}

char setup_RF(char rf_ch, char pll_lock, char rf_dr, char rf_pwr, char lna_hcurr)
{
	char data=0x00;
	char ret;		//status reg
	nRF24_write_reg(nRF24_RF_CH_REG,&rf_ch,1);
	
	data= pll_lock<<PLL_LOCK | rf_dr<<RF_DR | rf_pwr<<RF_PWR | lna_hcurr<<LNA_HCURR; //prepare RF_SETUP
	ret= nRF24_write_reg(nRF24_RF_SETUP_REG,&data,1);
	return ret;
}

char setup_rx_data_pipe(unsigned char pipeID, char * address, char sizePL, char autoACK)
//sizePL=0 means automatic payload size
//autoack is must be enable for dynamic payload
{


	char ret;
	char data;
	if (pipeID <2)
	{	
		//5 bytes address is required
		ret=nRF24_write_reg(nRF24_RX_ADDR_P0_REG+pipeID, address, AW_DEF);
	}
	else
	{	
		//only 1 bytes address is required
		ret=nRF24_write_reg(nRF24_RX_ADDR_P0_REG+pipeID, address,1);
	}
	
	//Enable data pipe
	nRF24_set_bit(nRF24_EN_RXADDR_REG,pipeID);
	
	//configure Dynamic Payload
	if (!sizePL) //if the size of the pazload is 0 then is set to automatic
	{
		//enable DPL for the pipe
		nRF24_set_bit(nRF24_DYNPD_REG,pipeID);
		//enable EN_DPL
		nRF24_set_bit(nRF24_FEATURE_REG,EN_DPL);
		//enable auto ack for this pipe
		nRF24_set_bit(nRF24_EN_AA_REG,pipeID);
		
        }
	else
	{
		//reset DPL for the pipe
		nRF24_res_bit(nRF24_DYNPD_REG,pipeID);
		//set static payload size
		nRF24_write_reg(nRF24_RX_PW_P0_REG+pipeID, &sizePL, 1);
		if (autoACK)
			nRF24_set_bit(nRF24_EN_AA_REG,pipeID);	//enable auto ack for this pipe
		else
			nRF24_res_bit(nRF24_EN_AA_REG,pipeID);	//disable auto ack for this pipe
	}
	
	
	

        return(ret);
}
