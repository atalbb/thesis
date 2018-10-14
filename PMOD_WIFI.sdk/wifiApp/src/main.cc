/************************************************************************/
/*                                                                      */
/*       TCPEchoServer                                                  */
/*                                                                      */
/*       A DEWFcK TCP Server application to           			        */
/*       demonstrate how to use the TcpServer Class.                    */
/*       This can be used in conjuction  with TCPEchoClient             */
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
#include <stdio.h>
#include "PmodWIFI.h"
#include "xil_cache.h"
#include "led_ip.h"
#include "sevenSegment.h"
#ifdef __MICROBLAZE__
#define PMODWIFI_VEC_ID XPAR_INTC_0_PMODWIFI_0_VEC_ID
#else
#define PMODWIFI_VEC_ID XPAR_FABRIC_PMODWIFI_0_WF_INTERRUPT_INTR
#endif

/************************************************************************/
/*                                                                      */
/*              SET THESE VALUES FOR YOUR NETWORK                       */
/*                                                                      */
/************************************************************************/
const u8 test[1024]={"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<script>\r\n"
		"    var count = 1;\r\n"
		"    function setColor(btn, color) {\r\n"
		"        var property = document.getElementById(btn);\r\n"
		"        if (count == 0) {\r\n"
		"            property.style.backgroundColor = \"#FFFFFF\"\r\n"
		"            count = 1;\r\n"
		"        }\r\n"
		"        else {\r\n"
		"            property.style.backgroundColor = \"#7FFF00\"\r\n"
		"            count = 0;\r\n"
		"        }\r\n"
		"    }\r\n"
		"</script>\r\n"
		"</head>"
		"<body>"
		"<input type=\"button\" id=\"button\" value = \"button\" style= \"color:white\" onclick=\"setColor('button', '#101010')\";/>"
		"</body>"
		"<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js\"></script>"
		//"<script src=\"js/scripts.js\"></script>"
		"</body>"
		"</html>"};
const u8 http_header[100] = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n\r\n";

 u8 homepage[] = "<!DOCTYPE html>"
"<html>"
"<body bgcolor=\"#0065A4\"><center>"
"<p><font size=\"8\">"
"<b>WELCOME TO THESIS DEMO</b><br>"
"</font>"
"</p>"
"<p>"
"<font size=\"5\">"
"<b><ins>LED CONFIGURATION</ins></b>"
"<form action=\"\" method=\"get\">"
"<font size=\"3\">"
"<b>LED1:</b>"
"<input type=\"radio\" name=\"Led1\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led1\" value=\"Off\" checked> Off<br>"
"<b>LED2:</b>"
"<input type=\"radio\" name=\"Led2\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led2\" value=\"Off\" checked> Off<br>"
"<b>LED3:</b>"
"<input type=\"radio\" name=\"Led3\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led3\" value=\"Off\" checked> Off<br>"
"<b>LED4:</b>"
"<input type=\"radio\" name=\"Led4\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led4\" value=\"Off\" checked> Off<br>"
"<b>LED5:</b>"
"<input type=\"radio\" name=\"Led5\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led5\" value=\"Off\" checked> Off<br>"
"<b>LED6:</b>"
"<input type=\"radio\" name=\"Led6\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led6\" value=\"Off\" checked> Off<br>"
"<b>LED7:</b>"
"<input type=\"radio\" name=\"Led7\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led7\" value=\"Off\" checked> Off<br>"
"<b>LED8:</b>"
"<input type=\"radio\" name=\"Led8\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led8\" value=\"Off\" checked> Off<br>"
"<b>LED9:</b>"
"<input type=\"radio\" name=\"Led9\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led9\" value=\"Off\" checked> Off<br>"
"<b>LED10:</b>"
"<input type=\"radio\" name=\"Led10\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led10\" value=\"Off\" checked> Off<br>"
"<b>LED11:</b>"
"<input type=\"radio\" name=\"Led11\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led11\" value=\"Off\" checked> Off<br>"
"<b>LED12:</b>"
"<input type=\"radio\" name=\"Led12\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led12\" value=\"Off\" checked> Off<br>"
"<b>LED13:</b>"
"<input type=\"radio\" name=\"Led13\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led13\" value=\"Off\" checked> Off<br>"
"<b>LED14:</b>"
"<input type=\"radio\" name=\"Led14\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led14\" value=\"Off\" checked> Off<br>"
"<b>LED15:</b>"
"<input type=\"radio\" name=\"Led15\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led15\" value=\"Off\" checked> Off<br>"
"<b>LED16:</b>"
"<input type=\"radio\" name=\"Led16\" value=\"On\" > On"
"<input type=\"radio\" name=\"Led16\" value=\"Off\" checked> Off<br><br>"
"<font size=\"5\">"
"<b><ins>SEVEN SEGMENT DISPLAY CONFIGURATION</ins></b><br>"
"<font size=\"3\">"
"<b>Hexadecimal Value(0-F):</b> <input type=\"text\" name=\"digit\" value = \"0\" maxlength=\"1\"><br>"
"<input type=\"submit\">"
"</form>"
"</font>"
"</p>"
"</center> </body>"
"</html>";
 u8 aes_html_page[2000]={"<html>\r\n"
		 "<head>\r\n"
		 "    <script src=\"https://cdn.rawgit.com/chrisveness/crypto/4e93a4d/aes.js\"></script>\r\n"
		 "    <script src=\"https://cdn.rawgit.com/chrisveness/crypto/4e93a4d/aes-ctr.js\"></script>\r\n"
		 "    <script>\r\n"
		 "        'use strict';\r\n"
		 "        function byteArrayToHexStr(b) { \r\n"
		 "          var s = '';\r\n"
		 "          for (var i=0; i<b.length; i++) s += b[i]<0x10 ? '0'+b[i].toString(16)+' ' : b[i].toString(16)+' ';\r\n"
		 "          return s;\r\n"
		 "        }\r\n"
		 "        String.prototype.toCodes = function() {\r\n"
		 "          if (this.length == 0) return '';\r\n"
		 "          var arr = this.split('');\r\n"
		 "          for (a in arr) arr[a] = arr[a].charCodeAt(0);\r\n"
		 "          return arr.join(':');\r\n"
		 "        }\r\n"
		 "    </script>\r\n"
		 "</head>\r\n"
		 "<body>\r\n"
		 "    <button onClick=\"alert(byteArrayToHexStr(Aes.cipher([0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff], Aes.keyExpansion([0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f]))))\"> 256-bit Test Vector </button><br>\r\n"
		 "</body>\r\n"
		 "</html>\r\n"
};
char gLedStatusStr[16][5]={"Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off","Off"};
char gDigitStr[2];
uint32_t gLedBitMap={0};
uint8_t gSevenSegDigit=0;
void extract_web_page_info(char *rx_buff){
    int i = 0,j = 0;
    char *ret;
    ret = strstr(rx_buff,"Led1");
    if(ret==0){
    	return;
    }
    ret = strstr(rx_buff,"=");
    if(ret==0){
    	return;
    }
    ret++;
    for(i=0;i<16;i++){
        while(*ret!='&'){
            gLedStatusStr[i][j++] = *ret;
            ret++;
        }
        gLedStatusStr[i][j]='\0';
        j=0;
        ret = strstr(ret,"=");
        ret++;
    }
    while(*ret!=' '){
           gDigitStr[j++] = *ret;
           ret++;
    }
   gDigitStr[j] = '\0';
   j=0;
	for(i=0;i<16;i++){
		xil_printf("LED%d:%s\r\n",i+1,gLedStatusStr[i]);
		if(strcmp((char*)gLedStatusStr[i],"On")==0){
			gLedBitMap |= (1<<i);
		}else{
			gLedBitMap &= ~(1<<i);
		}

	}
	xil_printf("Digit = %s\r\n",gDigitStr);
	if(strcmp(gDigitStr,"0")==0){gSevenSegDigit=0;};
	if(strcmp(gDigitStr,"1")==0){gSevenSegDigit=1;};
	if(strcmp(gDigitStr,"2")==0){gSevenSegDigit=2;};
	if(strcmp(gDigitStr,"3")==0){gSevenSegDigit=3;};
	if(strcmp(gDigitStr,"4")==0){gSevenSegDigit=4;};
	if(strcmp(gDigitStr,"5")==0){gSevenSegDigit=5;};
	if(strcmp(gDigitStr,"6")==0){gSevenSegDigit=6;};
	if(strcmp(gDigitStr,"7")==0){gSevenSegDigit=7;};
	if(strcmp(gDigitStr,"8")==0){gSevenSegDigit=8;};
	if(strcmp(gDigitStr,"9")==0){gSevenSegDigit=9;};
	if((strcmp(gDigitStr,"A")==0)||(strcmp(gDigitStr,"a")==0)){gSevenSegDigit=10;};
	if((strcmp(gDigitStr,"B")==0)||(strcmp(gDigitStr,"b")==0)){gSevenSegDigit=11;};
	if((strcmp(gDigitStr,"C")==0)||(strcmp(gDigitStr,"c")==0)){gSevenSegDigit=12;};
	if((strcmp(gDigitStr,"D")==0)||(strcmp(gDigitStr,"d")==0)){gSevenSegDigit=13;};
	if((strcmp(gDigitStr,"E")==0)||(strcmp(gDigitStr,"e")==0)){gSevenSegDigit=14;};
	if((strcmp(gDigitStr,"F")==0)||(strcmp(gDigitStr,"f")==0)){gSevenSegDigit=15;};
}
uint32_t update_webpage(){
	char ledOn[16][10];
	char ledOff[16][10];
	uint8_t i = 0;

	//if(ledStatus[0]){strcpy(ledOn[0],"checked");strcpy(ledOff[0],"\0");}
	//if(ledStatus[0]){strcpy(ledOn[0],"checked");strcpy(ledOff[0],"\0");}
	for(i=0;i<16;i++){
		if(strcmp(gLedStatusStr[i],"On")==0){
			strcpy(ledOn[i],"checked");
			strcpy(ledOff[i],"");
		}else{
			strcpy(ledOn[i],"");
			strcpy(ledOff[i],"checked");
		}
	}
	sprintf((char*)homepage,"<!DOCTYPE html>"
	"<html>"
	"<body bgcolor=\"#0065A4\"><center>"
	"<p><font size=\"8\">"
	"<b>WELCOME TO THESIS DEMO</b><br>"
	"</font>"
	"</p>"
	"<p>"
	"<font size=\"5\">"
	"<b><ins>LED CONFIGURATION</ins></b>"
	"<form action=\"\" method=\"get\">"
	"<font size=\"3\">"
	"<b>LED1:</b>"
	"<input type=\"radio\" name=\"Led1\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led1\" value=\"Off\" %s> Off<br>"
	"<b>LED2:</b>"
	"<input type=\"radio\" name=\"Led2\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led2\" value=\"Off\" %s> Off<br>"
	"<b>LED3:</b>"
	"<input type=\"radio\" name=\"Led3\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led3\" value=\"Off\" %s> Off<br>"
	"<b>LED4:</b>"
	"<input type=\"radio\" name=\"Led4\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led4\" value=\"Off\" %s> Off<br>"
	"<b>LED5:</b>"
	"<input type=\"radio\" name=\"Led5\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led5\" value=\"Off\" %s> Off<br>"
	"<b>LED6:</b>"
	"<input type=\"radio\" name=\"Led6\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led6\" value=\"Off\" %s> Off<br>"
	"<b>LED7:</b>"
	"<input type=\"radio\" name=\"Led7\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led7\" value=\"Off\" %s> Off<br>"
	"<b>LED8:</b>"
	"<input type=\"radio\" name=\"Led8\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led8\" value=\"Off\" %s> Off<br>"
	"<b>LED9:</b>"
	"<input type=\"radio\" name=\"Led9\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led9\" value=\"Off\" %s> Off<br>"
	"<b>LED10:</b>"
	"<input type=\"radio\" name=\"Led10\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led10\" value=\"Off\" %s> Off<br>"
	"<b>LED11:</b>"
	"<input type=\"radio\" name=\"Led11\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led11\" value=\"Off\" %s> Off<br>"
	"<b>LED12:</b>"
	"<input type=\"radio\" name=\"Led12\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led12\" value=\"Off\" %s> Off<br>"
	"<b>LED13:</b>"
	"<input type=\"radio\" name=\"Led13\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led13\" value=\"Off\" %s> Off<br>"
	"<b>LED14:</b>"
	"<input type=\"radio\" name=\"Led14\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led14\" value=\"Off\" %s> Off<br>"
	"<b>LED15:</b>"
	"<input type=\"radio\" name=\"Led15\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led15\" value=\"Off\" %s> Off<br>"
	"<b>LED16:</b>"
	"<input type=\"radio\" name=\"Led16\" value=\"On\" %s> On"
	"<input type=\"radio\" name=\"Led16\" value=\"Off\" %s> Off<br><br>"
	"<font size=\"5\">"
	"<b><ins>SEVEN SEGMENT DISPLAY CONFIGURATION</ins></b><br>"
	"<font size=\"3\">"
	"<b>Hexadecimal Value(0-F):</b> <input type=\"text\" name=\"digit\" value = \"%x\" maxlength=\"1\"><br>"
	"<input type=\"submit\">"
	"</form>"
	"</font>"
	"</p>"
	"</center> </body>"
	"</html>",
	ledOn[0],ledOff[0],ledOn[1],ledOff[1],ledOn[2],ledOff[2],ledOn[3],ledOff[3],ledOn[4],ledOff[4],
	ledOn[5],ledOff[5],ledOn[6],ledOff[6],ledOn[7],ledOff[7],ledOn[8],ledOff[8],ledOn[9],ledOff[9],
	ledOn[10],ledOff[10],ledOn[11],ledOff[11],ledOn[12],ledOff[12],ledOn[13],ledOff[13],ledOn[14],ledOff[14],ledOn[15],ledOff[15],gSevenSegDigit);
}

IPv4 ipServer = {0,0,0,0};//{0,0,0,0} for DHCP

unsigned short portServer = 80;//DEIPcK::iPersonalPorts44 + 300;     // port 44300

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
    DEWFcK::WEP40KEY keySet = {    0xBE, 0xC9, 0x58, 0x06, 0x97,     // Key 0
                                    0x00, 0x00, 0x00, 0x00, 0x00,     // Key 1
                                    0x00, 0x00, 0x00, 0x00, 0x00,     // Key 2
                                    0x00, 0x00, 0x00, 0x00, 0x00 };   // Key 3
    #define WiFiConnectMacro() deIPcK.wfConnect(szSsid, keySet, iWEPKey, &status)

#elif defined(USE_WEP104)

    const int iWEPKey = 0;
    DEWFcK::WEP104KEY keySet = {   0x3E, 0xCD, 0x30, 0xB2, 0x55, 0x2D, 0x3C, 0x50, 0x52, 0x71, 0xE8, 0x83, 0x91,   // Key 0
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
    LISTEN,
    ISLISTENING,
    WAITISLISTENING,
    AVAILABLECLIENT,
    ACCEPTCLIENT,
    READ,
    WRITE,
    CLOSE,
    EXIT,
    DONE
} STATE;

STATE state = CONNECT;

unsigned tStart = 0;
unsigned tWait = 3000;

TCPServer tcpServer;
#define cTcpClients 10
TCPSocket rgTcpClient[cTcpClients];

TCPSocket * ptcpClient = NULL;

u8 rgbRead[1024];
int cbRead = 0;
int count = 0;

IPSTATUS status;


void DemoInitialize();
void DemoRun();


int main(void)
{
	Xil_ICacheEnable();
	Xil_DCacheEnable();

	xil_printf("TCP Echo Server\r\nConnecting...\r\n");

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
	while (1){
		switch(state)
		    {
		        case CONNECT:
		            if(WiFiConnectMacro())
		            {
		                xil_printf("Connection Created\r\n");
		                deIPcK.begin(ipServer);
		                state = LISTEN;
		            }
		            else if(IsIPStatusAnError(status))
		            {
		            	xil_printf("Unable to make connection, status: 0x%X\r\n", status);
		                state = CLOSE;
		            }
		            break;

		    // say to listen on the port
		    case LISTEN:
		        if(deIPcK.tcpStartListening(portServer, tcpServer))
		        {
		            for(int i = 0; i < cTcpClients; i++)
		            {
		                tcpServer.addSocket(rgTcpClient[i]);
		            }
		        }
		        state = ISLISTENING;
		        break;

		    case ISLISTENING:
		        count = tcpServer.isListening();

		        if(count > 0)
		        {
		        	deIPcK.getMyIP(ipServer);
		        	xil_printf("Server started on %d.%d.%d.%d:%d\r\n",ipServer.u8[0],ipServer.u8[1],ipServer.u8[2],ipServer.u8[3], portServer);
		        	xil_printf("%d sockets listening on port: %d\r\n", count, portServer);
		            state = AVAILABLECLIENT;
		        }
		        else
		        {
		        	xil_printf("%d sockets listening on port: %d\r\n", count, portServer);
		            state = WAITISLISTENING;
		        }
		        break;

		    case WAITISLISTENING:
		        if(tcpServer.isListening() > 0)
		        {
		            state = ISLISTENING;
		        }
		        break;

		    // wait for a connection
		    case AVAILABLECLIENT:
		        if((count = tcpServer.availableClients()) > 0)
		        {
		        	xil_printf("Got %d clients pending\r\n", count);
		            state = ACCEPTCLIENT;
		        }
		        break;

		    // accept the connection
		    case ACCEPTCLIENT:

		        // accept the client
		        if((ptcpClient = tcpServer.acceptClient()) != NULL && ptcpClient->isConnected())
		        {
		        	xil_printf("Got a Connection\r\n");
		            state = READ;
		            tStart = (unsigned) SYSGetMilliSecond();
		        }

		        // this probably won't happen unless the connection is dropped
		        // if it is, just release our socket and go back to listening
		        else
		        {
		            state = CLOSE;
		        }
		        break;

		    // wait fot the read, but if too much time elapses (5 seconds)
		    // we will just close the tcpClient and go back to listening
		    case READ:

		        // see if we got anything to read
		        if((cbRead = ptcpClient->available()) > 0)
		        {
		            int i =0;
		        	cbRead = cbRead < (int) sizeof(rgbRead) ? cbRead : sizeof(rgbRead);
		            cbRead = ptcpClient->readStream(rgbRead, cbRead);

		            xil_printf("Got %d bytes\r\n", cbRead);
		            for(i=0;i<cbRead;i++){
		            	xil_printf("%c",rgbRead[i]);
		            }
		            rgbRead[i] = '\0';
		            xil_printf("\r\n");
		            extract_web_page_info((char*)rgbRead);
		            update_webpage();
	                LED_IP_mWriteReg(0x44a40000,0,gLedBitMap);
	                SEVENSEGMENT_mWriteReg(0x44a50000,0,gSevenSegDigit);
		            state = WRITE;
		        }

		        //If connection was closed by the user
		        else if(!ptcpClient->isConnected())
		        //else if( (((unsigned) SYSGetMilliSecond()) - tStart) > tWait ) // If too much time elapsed between reads, close the connection
		        {
		            state = CLOSE;
		        }
		        break;

		    // echo back the string
		    case WRITE:
		        if(ptcpClient->isConnected())
		        {
		        	//update_webpage();
		        	xil_printf("Writing: \r\n");
		            for(int i=0; i < strlen((const char*)homepage); i++)
		            {
		            	xil_printf("%c",(char) homepage[i]);
		            }
		            xil_printf("\r\n");
		            //http_header
		            //ptcpClient->writeStream(rgbRead, cbRead);
		            //pageLEDonDoorLock
		            //webpage
		            //update_webpage
		            //test
		            //update_webpage();
		            //aes_html_page
		            ptcpClient->writeStream(http_header, strlen((const char*)http_header));
		            //ptcpClient->writeStream(pageLEDonDoorLock, strlen((const char*)pageLEDonDoorLock));
		            //ptcpClient->writeStream(homepage, strlen((const char*)homepage));
		            ptcpClient->writeStream(homepage, strlen((const char*)homepage));
		            //ptcpClient->writeStream(aes_html_page, strlen((const char*)aes_html_page));
		            //ptcpClient->writeStream(test, strlen((const char*)test));
		            state = READ;
		            //state = CLOSE;
		            tStart = (unsigned) SYSGetMilliSecond();
		        }

		        // the connection was closed on us, go back to listening
		        else
		        {
		        	xil_printf("Unable to write back.\r\n");
		            state = CLOSE;
		        }
		        break;

		    // close our tcpClient and go back to listening
		    case CLOSE:
		    	xil_printf("Closing connection\r\n");
		        if (ptcpClient)ptcpClient->close();
		        tcpServer.addSocket(*ptcpClient);
		        xil_printf("\r\n");
		        state = ISLISTENING;
		        break;

		    // something bad happen, just exit out of the program
		    case EXIT:
		        tcpServer.close();
		        xil_printf("Something went wrong, sketch is done.\r\n");
		        state = DONE;
		        break;

		    // do nothing in the loop
		    case DONE:
		    default:
		        break;
		    }

		    // every pass through loop(), keep the stack alive
		    DEIPcK::periodicTasks();
	}
}
