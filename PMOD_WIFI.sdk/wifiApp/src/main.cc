/************************************************************************/
/*                                                                      */
/*      TCPEchoClient                                                   */
/*                                                                      */
/*        A DEIPcK TCP Client application to                    		*/
/*        demonstrate how to use the TcpClient Class.                   */
/*        This can be used in conjunction with TCPEchoServer            */
/*                                                                      */
/************************************************************************/
/*       Author:        Keith Vogel                                     */
/*       Copyright 2014, Digilent Inc.                                  */
/************************************************************************/
/*
*
* Copyright (c) 2013-2014, Digilent <www.digilentinc.com>
* Contact Digilent for the latest version.
*
* This program is free software; distributed under the terms of
* BSD 3-clause license ("Revised BSD License", "New BSD License", or "Modified BSD License")
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this
*        list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names
*        of its contributors may be used to endorse or promote products derived
*        from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*       5/14/2014 (KeithV): Created                                    */
/*		 8/09/2016 (TommyK): Modified to use Microblaze/Zynq			*/
/*                                                                      */
/************************************************************************/

#include <string.h>
#include "PmodWIFI.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xgpio.h"
#include "led_ip.h"
#include "sevenSegment.h"
#include "xstatus.h"
#include "xuartlite.h"
#include "xil_printf.h"
#include "xbram.h"
#include "atal_AES_LUT_41clks.h"
#ifdef __MICROBLAZE__
#define PMODWIFI_VEC_ID XPAR_INTC_0_PMODWIFI_0_VEC_ID
#else
#define PMODWIFI_VEC_ID XPAR_FABRIC_PMODWIFI_0_WF_INTERRUPT_INTR
#endif

#define NO_OF_KEY_WORDS			120
#define NO_OF_DATA_WORDS		4

uint32_t *key_addr_start = (uint32_t*)0xC2000000;
uint32_t *data_readaddr_start = (uint32_t*)0xC0000000;    // datastore
//uint32_t *data_writeaddr_start = (uint32_t*)(0xC0000004 + 4 * NO_OF_DATA_WORDS);    // datastore
uint32_t *data_writeaddr_start = (uint32_t*)0xC0000004;    // datastore
uint32_t key[NO_OF_KEY_WORDS]={0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
							   0x10111213, 0x14151617, 0x18191a1b, 0x1c1d1e1f,
							   0xa573c29f, 0xa176c498, 0xa97fce93, 0xa572c09c,
							   0x1651a8cd, 0x0244beda, 0x1a5da4c1, 0x0640bade,
							   0xae87dff0, 0x0ff11b68, 0xa68ed5fb, 0x03fc1567,
							   0x6de1f148, 0x6fa54f92, 0x75f8eb53, 0x73b8518d,
							   0xc656827f, 0xc9a79917, 0x6f294cec, 0x6cd5598b,
							   0x3de23a75, 0x524775e7, 0x27bf9eb4, 0x5407cf39,
							   0x0bdc905f, 0xc27b0948, 0xad5245a4, 0xc1871c2f,
							   0x45f5a660, 0x17b2d387, 0x300d4d33, 0x640a820a,
							   0x7ccff71c, 0xbeb4fe54, 0x13e6bbf0, 0xd261a7df,
							   0xf01afafe, 0xe7a82979, 0xd7a5644a, 0xb3afe640,
							   0x2541fe71, 0x9bf50025, 0x8813bbd5, 0x5a721c0a,
							   0x4e5a6699, 0xa9f24fe0, 0x7e572baa, 0xcdf8cdea,
							   0x24fc79cc, 0xbf0979e9, 0x371ac23c, 0x6d68de36,
							   0x24fc79cc, 0xbf0979e9, 0x371ac23c, 0x6d68de36,
							   0x34f1d1ff, 0xbfceaa2f, 0xfce9e25f, 0x2558016e,
							   0x5e1648eb, 0x384c350a, 0x7571b746, 0xdc80e684,
							   0xc8a30580, 0x8b3f7bd0, 0x43274870, 0xd9b1e331,
							   0xb5708e13, 0x665a7de1, 0x4d3d824c, 0xa9f151c2,
							   0x74da7ba3, 0x439c7e50, 0xc81833a0, 0x9a96ab41,
							   0x3ca69715, 0xd32af3f2, 0x2b67ffad, 0xe4ccd38e,
							   0xf85fc4f3, 0x374605f3, 0x8b844df0, 0x528e98e1,
							   0xde69409a, 0xef8c64e7, 0xf84d0c5f, 0xcfab2c23,
							   0xaed55816, 0xcf19c100, 0xbcc24803, 0xd90ad511,
							   0x15c668bd, 0x31e5247d, 0x17c168b8, 0x37e6207c,
							   0x7fd7850f, 0x61cc9916, 0x73db8903, 0x65c89d12,
							   0x2a2840c9, 0x24234cc0, 0x26244cc5, 0x202748c4,
							   0x1a1f181d, 0x1e1b1c19, 0x12171015, 0x16131411,
							   0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};

uint32_t data[32]={	 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff,
					 0xffeeddcc, 0xbbaa9988, 0x77665544, 0x33221100,
					 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef,
					 0xfedcba98, 0x76543210, 0xfedcba98, 0x76543210,
					 0xa1b2c3d4, 0xf55f4d3c, 0x3b1a3223, 0x17718998,
					 0x1fa793fe, 0xb3784793, 0x091a6e4d, 0x78acdafa,
					 0x7af1da9c, 0x46739018, 0x3265481d, 0x3ac9bfde,
					 0x87fe567a, 0x8ee04778, 0x328745ea, 0xc7aa9039};
uint32_t build_aes_block(uint8_t *d, uint32_t len){
	uint32_t i = 0,j=0,k=0;
	uint32_t noOfBlocks = len/16;
	xil_printf("AES: noOfBlocks = %d\r\n",noOfBlocks);
	//for(i=0;i<noOfBlocks;i++){
		while(k<(16*noOfBlocks)){
			data[j++] = (d[k]<<24)|(d[k+1]<<16)|(d[k+2]<<8)|(d[k+3]<<0);
			k += 4;
		}
		//k=0;
	//}
	//xil_printf("AES::: noOfWords = %d\r\n",j);
	return j;
}
uint32_t covert_aes_datablock_to_byte(uint8_t *d,uint32_t *encData, uint32_t noOfWords){
	uint32_t i =0,j=0;
	for(i=0;i<noOfWords;i++){
		d[j++] = (encData[i] & 0xff000000) >> 24;
		d[j++] = (encData[i] & 0x00ff0000) >> 16;
		d[j++] = (encData[i] & 0x0000ff00) >> 8;
		d[j++] = encData[i] & 0x000000ff;
	}
	return j;
}
void delay(){
	uint32_t i = 0;
	for(i=0;i<1000;i++);
}
void loadPlainTextInBram(uint32_t * plainText, uint32_t noOfWords){
	uint32_t i = 0;
 	for(i=0;i<noOfWords;i++){
 		*(data_readaddr_start+i) = plainText[i];
 	}
}
void loadFromBramToBuf(uint32_t * bramAddr, uint32_t *buf, uint32_t noOfWords){
	uint32_t i = 0;
	for(i=0;i<noOfWords;i++){
		buf[i] = *(bramAddr+i);
	}
}
void displayBram(uint32_t *startAddress,uint32_t noOfWords){
   uint32_t i = 0;
   for(i=0;i<noOfWords;i++){
	   xil_printf("location: %x, Value: %08x\n\r",startAddress+i,*(startAddress+i));
   }
}
void loadExpandedKeysInBram(uint32_t * expandedKeys, uint32_t noOfWords){
	uint32_t i = 0;
 	for(i=0;i<noOfWords;i++){
 		*(key_addr_start+i) = expandedKeys[i];
 	}
}
void startEncryption(uint32_t noOfWords){
	  uint32_t check = 0;
	  check = ATAL_AES_LUT_41CLKS_mReadReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG1_OFFSET);
	  //xil_printf("\n\rCheck = %d\n\r",check);
	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, 0);
	  delay();
	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, ((noOfWords<<2)|(1<<0)));
	  delay();
	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, ((1<<11)|(noOfWords<<2)|(1<<0))); //b11:Reset, b[10:2]:Length(inWords),b[1:0]:Enc/Dec
	  while(check != 7)
	  {
		check = ATAL_AES_LUT_41CLKS_mReadReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG1_OFFSET);
	    //xil_printf("\n\rCheck = %d\n\r",check);
	  }
}
void startDecryption(uint32_t noOfWords){
	  uint32_t check = 0;
	  check = ATAL_AES_LUT_41CLKS_mReadReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG1_OFFSET);
	  //xil_printf("\n\rCheck = %d\n\r",check);

	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, 0);
	  delay();
	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, ((noOfWords<<2)|(2<<0)));
	  delay();
	  ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, ((1<<11)|(noOfWords<<2)|(2<<0))); // Length = 4, Start = 1
	  //ATAL_AES_LUT_41CLKS_mWriteReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG0_OFFSET, 0x10); // Length = 4, Start = 0
	  while(check != 7)
	  {
		check = ATAL_AES_LUT_41CLKS_mReadReg(0x44a60000, ATAL_AES_LUT_41CLKS_S_AXI_SLV_REG1_OFFSET);
	    //xil_printf("\n\rCheck = %d\n\r",check);
	  }
}
void aestest(){
	   uint32_t tempBuff[NO_OF_DATA_WORDS];
    xil_printf("-- My Algorithm --\r\n");

	   xil_printf("-- Loading Keys --\r\n");
	   loadExpandedKeysInBram(key,NO_OF_KEY_WORDS);
	   //xil_printf("***** Keys *****\n\r");  // Printing loaded keys
	   //displayBram(key_addr_start,NO_OF_KEY_WORDS);

	   loadPlainTextInBram(data,NO_OF_DATA_WORDS);
	   xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
	   displayBram(data_readaddr_start,NO_OF_DATA_WORDS);

	   startEncryption(NO_OF_DATA_WORDS);
	   xil_printf("***** Encrypted Data *****\n\r");  // Printing loaded keys
	   displayBram(data_writeaddr_start + NO_OF_DATA_WORDS,NO_OF_DATA_WORDS);

	   loadFromBramToBuf(data_writeaddr_start + NO_OF_DATA_WORDS,tempBuff,NO_OF_DATA_WORDS);
	   loadPlainTextInBram(tempBuff,NO_OF_DATA_WORDS);
	   xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
	   displayBram(data_readaddr_start,NO_OF_DATA_WORDS);

	   startDecryption(NO_OF_DATA_WORDS);
	   xil_printf("***** Decrypted Data *****\n\r");  // Printing loaded keys
	   displayBram(data_writeaddr_start + NO_OF_DATA_WORDS,NO_OF_DATA_WORDS);

}
uint32_t addAesPadding(uint8_t * data, uint32_t len){
	uint32_t i = 0;
	uint8_t noOfPaddingBytes = 0;
	if((len%16)==0){
		noOfPaddingBytes = 0;
	}else{
		noOfPaddingBytes = 16 - (len%16);// - (len % 16);
	}
	xil_printf("\r\nnoOfPaddingBytes = %d\r\n",noOfPaddingBytes);
	for(i=0;i<noOfPaddingBytes;i++){
		data[len++] = 0;
	}
	return len;
}


/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UARTLITE_DEVICE_ID	XPAR_UARTLITE_0_DEVICE_ID

/*
 * The following constant controls the length of the buffers to be sent
 * and received with the UartLite, this constant must be 16 bytes or less since
 * this is a single threaded non-interrupt driven example such that the
 * entire buffer will fit into the transmit and receive FIFOs of the UartLite.
 */
#define TEST_BUFFER_SIZE 64

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int UartLitePolledExample(u16 DeviceId);

/************************** Variable Definitions *****************************/

XUartLite UartLite;		/* Instance of the UartLite Device */

/*
 * The following buffers are used in this example to send and receive data
 * with the UartLite.
 */
u8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */
unsigned int ReceivedCount = 0;

/************************************************************************/
/*                                                                      */
/*              SET THESE VALUES FOR YOUR NETWORK                       */
/*                                                                      */
/************************************************************************/

const char * szIPServer = "192.168.43.7";    // server to connect to
uint16_t portServer = DEIPcK::iPersonalPorts44 + 300;     // port 44300

// Specify the SSID
const char * szSsid = "SM-G950U5F0";

// select 1 for the security you want, or none for no security
#define USE_WPA2_PASSPHRASE
//#define USE_WPA2_KEY
//#define USE_WEP40
//#define USE_WEP104
//#define USE_WF_CONFIG_H

// modify the security key to what you have.
#if defined(USE_WPA2_PASSPHRASE)

    const char * szPassPhrase = "atal1234";
    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, szPassPhrase, &status)

#elif defined(USE_WPA2_KEY)

    WPA2KEY key = { 0x27, 0x2C, 0x89, 0xCC, 0xE9, 0x56, 0x31, 0x1E,
                    0x3B, 0xAD, 0x79, 0xF7, 0x1D, 0xC4, 0xB9, 0x05,
                    0x7A, 0x34, 0x4C, 0x3E, 0xB5, 0xFA, 0x38, 0xC2,
                    0x0F, 0x0A, 0xB0, 0x90, 0xDC, 0x62, 0xAD, 0x58 };
    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, key, &status)

#elif defined(USE_WEP40)

    const int iWEPKey = 0;
    WEP40KEY keySet = { 0xBE, 0xC9, 0x58, 0x06, 0x97,     // Key 0
                        0x00, 0x00, 0x00, 0x00, 0x00,     // Key 1
                        0x00, 0x00, 0x00, 0x00, 0x00,     // Key 2
                        0x00, 0x00, 0x00, 0x00, 0x00 };   // Key 3
    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, keySet, iWEPKey, &status)

#elif defined(USE_WEP104)

    const int iWEPKey = 0;
    WEP104KEY keySet = { 0x3E, 0xCD, 0x30, 0xB2, 0x55, 0x2D, 0x3C, 0x50, 0x52, 0x71, 0xE8, 0x83, 0x91,   // Key 0
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // Key 1
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // Key 2
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // Key 3
    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, keySet, iWEPKey, &status)

#elif defined(USE_WF_CONFIG_H)

    #define WiFiConnectMacro() deIPcK.wfConnect(0, &status)

#else   // no security - OPEN

    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, &status)

#endif





//******************************************************************************************
//******************************************************************************************
//***************************** END OF CONFIGURATION ***************************************
//******************************************************************************************
//******************************************************************************************

    typedef enum
    {
        NONE = 0,
        CONNECT,
        TCPCONNECT,
        WRITE,
        READ,
        CLOSE,
        DONE,
    } STATE;

STATE state = CONNECT;

uint32_t tStart = 0;
uint32_t tempStart = 0;
uint32_t tWait = 5000;

TCPSocket tcpSocket;
u8 rgbRead[1024];

// this is for Print.write to print
byte rgbWrite[] = {'*','W','r','o','t','e',' ','f','r','o','m',' ','p','r','i','n','t','.','w','r','i','t','e','*','\n'};
int cbWrite = sizeof(rgbWrite);

// this is for tcpSocket.writeStream to print
byte rgbWriteStream[] = {'*','W','r','o','t','e',' ','f','r','o','m',' ','t','c','p','C','l','i','e','n','t','.','w','r','i','t','e','S','t','r','e','a','m','*','\n'};
int cbWriteStream = sizeof(rgbWriteStream);

void DemoInitialize();
void DemoRun();

uint8_t uart_init(){
	int Status;
	int Index;

	/*
	 * Initialize the UartLite driver so that it is ready to use.
	 */
	Status = XUartLite_Initialize(&UartLite, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffer[Index] = 0;
	}
	while (1) {
		ReceivedCount += XUartLite_Recv(&UartLite,
					   RecvBuffer + ReceivedCount,
					   TEST_BUFFER_SIZE - ReceivedCount);
		//outbyte(RecvBuffer[ReceivedCount-1]);
//		if ((ReceivedCount == TEST_BUFFER_SIZE-2) || (RecvBuffer[ReceivedCount-1] == 0x0a)) {
//			break;
//		}
//		if (ReceivedCount == TEST_BUFFER_SIZE-2) {
//			RecvBuffer[TEST_BUFFER_SIZE-2] = 0x0d;
//			RecvBuffer[TEST_BUFFER_SIZE-1] = 0x0a;
//			break;
//		}else if(RecvBuffer[ReceivedCount-1] == 0x0a){
//			break;
//		}
		if(RecvBuffer[ReceivedCount-1] == 0x0d){
			ReceivedCount--;
			break;
		}
	}
	return XST_SUCCESS;
}
int main(void)
{
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	//xil_printf("WiFiTCPEchoClient 3.0\r\nConnecting to network...\r\n");
	aestest();
	xil_printf("WiFiTCPEchoClient Program Started!\r\n\r\nAuthenticating Device to the Network...\r\n");
	DemoInitialize();
	DemoRun();
	return 0;
}

void DemoInitialize()
{
	setPmodWifiAddresses(XPAR_PMODWIFI_0_AXI_LITE_SPI_BASEADDR, XPAR_PMODWIFI_0_AXI_LITE_WFGPIO_BASEADDR, XPAR_PMODWIFI_0_AXI_LITE_WFCS_BASEADDR, XPAR_PMODWIFI_0_S_AXI_TIMER_BASEADDR);
	setPmodWifiIntVector(PMODWIFI_VEC_ID);
}

void DemoRun()
{
	IPSTATUS status;
	int cbRead=0;
	uint32_t totalBytes = 0;
	uint32_t totalWords = 0;
	uint32_t tempBuff[32] = {0};
	uint32_t i = 0;
	while (1){
		switch(state)
		    {
		 case CONNECT:
		            if(WiFiConnectMacro())
		            {
		                LED_IP_mWriteReg(0x44a40000,0,0x00ff);
		                SEVENSEGMENT_mWriteReg(0x44a50000,0,1);
		                xil_printf("\r\nDevice Authenticated!\r\n");
		                xil_printf("\r\nDevice connected to the Network!\r\n");
		                xil_printf("\r\nConnecting to TCP Server listening at %s:%d...\r\n",szIPServer,portServer);
		                deIPcK.begin();
		                state = TCPCONNECT;
		            }
		            else if(IsIPStatusAnError(status))
		            {
		                xil_printf("Unable to connect, status: %d\r\n", status);
		                state = CLOSE;
		            }
		            break;

		        case TCPCONNECT:
		            if(deIPcK.tcpConnect(szIPServer, portServer, tcpSocket))
		            {
		                LED_IP_mWriteReg(0x44a40000,0,0xffff);
		                SEVENSEGMENT_mWriteReg(0x44a50000,0,2);
		            	xil_printf("\r\nConnected to TCP server listening at %s:%d!\r\n",szIPServer,portServer);
		                state = WRITE;
		            }
		        break;

		        // write out the strings
		        case WRITE:
		            if(tcpSocket.isEstablished())
		              {
		                //tcpSocket.writeStream(rgbWriteStream, cbWriteStream);
//		            	xil_printf("\r\n*******************************************************\r\n");
		            	xil_printf("\r\nEnter Message(64 chars Max) to be Sent followed by 'Enter':\r\n");
		            	uart_init();
		            	RecvBuffer[ReceivedCount]=0;
		            	xil_printf("\r\n*******************************************************\r\n");
		            	xil_printf("\r\nNo of Bytes:%d\r\n",ReceivedCount);
		            	totalBytes = addAesPadding(RecvBuffer,ReceivedCount);
		            	xil_printf("totalBytes after Padding= %d\r\n",totalBytes);
		            	totalWords = build_aes_block(RecvBuffer,totalBytes);
		            	xil_printf("AES: noOfWords = %d\r\n",totalWords);

		         	   loadPlainTextInBram(data,totalWords);
		         	   xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
		         	   displayBram(data_readaddr_start,totalWords);

		         	   startEncryption(totalWords);
		         	   xil_printf("***** Encrypted Data *****\n\r");  // Printing loaded keys
		         	   displayBram(data_writeaddr_start + totalWords,totalWords);

		        	   loadFromBramToBuf(data_writeaddr_start + totalWords,tempBuff,totalWords);
		        	   totalBytes = covert_aes_datablock_to_byte(RecvBuffer,tempBuff,totalWords);

		            	//xil_printf("\r\nSent Message:%s",RecvBuffer);
		        	   xil_printf("\r\nSent Message:\r\n");
		        	   for(i=0;i<totalBytes;i++){
		            		if((i % 4) == 0){
		            			xil_printf("\r\n");
		            		}
		            		xil_printf("%02x",RecvBuffer[i]);

		            	}
		        	    tcpSocket.writeStream(RecvBuffer, totalBytes);
		            	ReceivedCount = 0;
		                state = READ;
		                tStart = (uint32_t) SYSGetMilliSecond();
	                    //xil_printf("TIMEOUT START:%ld\r\n",tStart);
		                }
		            break;

		            // look for the echo back
		            case READ:
		            	//tempStart = (uint32_t) SYSGetMilliSecond();
		                // see if we got anything to read
		                if((cbRead = tcpSocket.available()) > 0)
		                {
		                    cbRead = cbRead < (int) sizeof(rgbRead) ? cbRead : sizeof(rgbRead);
		                    cbRead = tcpSocket.readStream(rgbRead, cbRead);
		                    rgbRead[cbRead]=0;//Null Terminator
		                    //cbRead = 0;
			                xil_printf("\r\nReceived Message:");
		                    //xil_printf("%s\r\n", rgbRead);
				            xil_printf("\r\n**************************************************\r\n");
				            xil_printf("\r\nNumber of bytes read %d:\r\n", cbRead);
							xil_printf("\r\nReceived Message:");
				            for(int i=0; i < cbRead; i++)
							{
								if((i%4)==0){
									xil_printf("\r\n");
								}
				            	xil_printf("%02x",rgbRead[i]);
							}
							xil_printf("\r\n");
							totalWords = build_aes_block(rgbRead,cbRead);
							xil_printf("AES: noOfWords = %d\r\n",totalWords);
						   loadPlainTextInBram(data,totalWords);
						   xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
						   displayBram(data_readaddr_start,totalWords);
						   startDecryption(totalWords);
						   xil_printf("***** Decrypted Data *****\n\r");  // Printing loaded keys
						   displayBram(data_writeaddr_start + totalWords,totalWords);
						   loadFromBramToBuf(data_writeaddr_start + totalWords,tempBuff,totalWords);
						   totalBytes = covert_aes_datablock_to_byte(RecvBuffer,tempBuff,totalWords);
						   xil_printf("***** RECEIVED DATA *****\n\r");
						   for(i=0;i<totalBytes;i++){
							   xil_printf("%c",RecvBuffer[i]);
						   }
						   xil_printf("\r\n");
			                xil_printf("\r\n*******************************************************\r\n");
		                    if((strncmp((char *)(&rgbRead[2]),"close",5)==0) || (strncmp((char *)(&rgbRead[2]),"CLOSE",5)==0)){
		                    	state = CLOSE;
		                    }else{
		                    	state = WRITE;
		                    }
		                }

		                // give us some time to get everything echo'ed back

//		                else if( (tempStart - tStart) > tWait )
//		                {
//		                    state = WRITE;
//		                    xil_printf("TIMEOUT END:%ld\r\n",(uint32_t) SYSGetMilliSecond());
//
//		                }
		                break;

		        // done, so close up the tcpSocket
		        case CLOSE:
		            tcpSocket.close();
		            xil_printf("\r\nClosing TcpClient!\r\n");
		            state = DONE;
//	            	uart_init();
//	            	RecvBuffer[ReceivedCount]=0;
//                    if((strncmp((char *)RecvBuffer,"open",4)==0) || (strncmp((char *)RecvBuffer,"OPEN",4)==0)){
//                    	state = CONNECT;
//                    	xil_printf("\r\nReconnecting...\r\n");
//                    }else{
//                    	state = DONE;
//                    }
		            break;

		        case DONE:
	                LED_IP_mWriteReg(0x44a40000,0,0x00ff);
	                SEVENSEGMENT_mWriteReg(0x44a50000,0,1);
		        	break;
		        default:
		            break;
		    }

		    // keep the stack alive each pass through the loop()
		    DEIPcK::periodicTasks();
	}
}
