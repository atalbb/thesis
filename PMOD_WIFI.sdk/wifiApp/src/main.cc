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
#include "atal_AES_LUT_41clks.h"
#ifdef __MICROBLAZE__
#define PMODWIFI_VEC_ID XPAR_INTC_0_PMODWIFI_0_VEC_ID
#else
#define PMODWIFI_VEC_ID XPAR_FABRIC_PMODWIFI_0_WF_INTERRUPT_INTR
#endif

#define NO_OF_KEY_WORDS			120
#define NO_OF_DATA_WORDS		8

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
int str_to_hex(char *str, uint8_t *hex){
    int i = 0,j=0;
    char *ptr;
    //long ret;
    int len = strlen(str);
    len = len/2;
    char temp[128][3];
    for(i=0;i<len;i++){
        temp[i][j++] = *str;
        str++;
        temp[i][j++] = *str;
        str++;
        temp[i][j] = '\0';
        hex[i] = strtol(temp[i], &ptr, 16);
        j= 0;
        //xil_printf("hex[%d] = 0x%x\r\n",i,hex[i]);
    }
    return len;

}
uint32_t build_aes_block(uint8_t *d, uint32_t len){
	uint32_t i = 0,j=0,k=0;
	uint32_t noOfBlocks = len/16;
	xil_printf("AES: noOfBlocks = %d\r\n",noOfBlocks);
	//for(i=0;i<noOfBlocks;i++){
		while(k<(16*noOfBlocks)){
			data[j++] = (d[k]<<24)|(d[k+1]<<16)|(d[k+2]<<8)|(d[k+3]<<0);
			k += 4;
		}
		//k= 0;
	//}
//	xil_printf("AES: noOfWords = %d\r\n",j);
	return j;
}
uint32_t convert_aes_datablock_to_byte(uint8_t *d,uint32_t *encData, uint32_t noOfWords){
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
	   //uint32_t tempBuff[NO_OF_DATA_WORDS];
       //xil_printf("-- My Algorithm --\r\n");

	   //xil_printf("-- Loading Keys --\r\n");
	   loadExpandedKeysInBram(key,NO_OF_KEY_WORDS);
	   //xil_printf("***** Keys *****\n\r");  // Printing loaded keys
	   //displayBram(key_addr_start,NO_OF_KEY_WORDS);

	   //loadPlainTextInBram(data,NO_OF_DATA_WORDS);
	   //xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
	   //displayBram(data_readaddr_start,NO_OF_DATA_WORDS);

	   //startEncryption(NO_OF_DATA_WORDS);
	   //xil_printf("***** Encrypted Data *****\n\r");  // Printing loaded keys
	   //displayBram(data_writeaddr_start + NO_OF_DATA_WORDS,NO_OF_DATA_WORDS);

	   //loadFromBramToBuf(data_writeaddr_start + NO_OF_DATA_WORDS,tempBuff,NO_OF_DATA_WORDS);
	   //loadPlainTextInBram(tempBuff,NO_OF_DATA_WORDS);
	   //xil_printf("***** Plaintext Data *****\n\r");  // Printing loaded keys
	   //displayBram(data_readaddr_start,NO_OF_DATA_WORDS);

	   //startDecryption(NO_OF_DATA_WORDS);
	   //xil_printf("***** Decrypted Data *****\n\r");  // Printing loaded keys
	   //displayBram(data_writeaddr_start + NO_OF_DATA_WORDS,NO_OF_DATA_WORDS);

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

/************************************************************************/
/*                                                                      */
/*              SET THESE VALUES FOR YOUR NETWORK                       */
/*                                                                      */
/************************************************************************/
const u8 aes_page[]=
{"<!DOCTYPE html>"
		"<html>"
		"<script>\r\n"
		"function pad(value, width, padchar) {\r\n"
		"    while (value.length < width) {\r\n"
		"        value += padchar;\r\n"
		"    }\r\n"
		"    return value;\r\n"
		"}\r\n"
		"</script>\r\n"
		"<script>\r\n"
		"function aes_ecb_encrypt()\r\n"
		"{\r\n"
		"var data= document.getElementById(\"PlaintextInput\").value;\r\n"
		"var length = data.length;\r\n"
		"var width;\r\n"
		"var mod = length % 16;\r\n"
		"if(mod==0){\r\n"
		"	width = length;\r\n"
		"}else{\r\n"
		"	width = length + (16 - length % 16);\r\n"
		"}\r\n"
		"var key = [ 00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31];\r\n"
		"var text = pad(data,width,'\\0');\r\n"
		"var textBytes = aesjs.utils.utf8.toBytes(text);\r\n"
		"var aesEcb = new aesjs.ModeOfOperation.ecb(key);\r\n"
		"var encryptedBytes = aesEcb.encrypt(textBytes);\r\n"
		"var encryptedHex = aesjs.utils.hex.fromBytes(encryptedBytes);\r\n"
		"document.getElementById(\"CiphertextOutput\").value = encryptedHex;\r\n"
		"}\r\n"
		"</script>\r\n"
		"<script>\r\n"
		"function aes_ecb_decrypt()\r\n"
		"{\r\n"
		"var data= document.getElementById(\"CipherTextInput\").value;\r\n"
		"var length = data.length;\r\n"
		"var width;\r\n"
		"var mod = length % 16;\r\n"
		"if(mod==0){\r\n"
		"	width = length;\r\n"
		"}else{\r\n"
		"	width = length + (16 - length % 16);\r\n"
		"}\r\n"
		"var key = [ 00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31];\r\n"
		"var text = pad(data,width,'\\0');\r\n"
		"var textBytes = aesjs.utils.utf8.toBytes(text);\r\n"
		"var aesEcb = new aesjs.ModeOfOperation.ecb(key);\r\n"
		"var decryptedBytes = aesEcb.decrypt(textBytes);\r\n"
		"var decryptedHex = aesjs.utils.hex.fromBytes(decryptedBytes);\r\n"
		"document.getElementById(\"PlaintextOutput\").value = decryptedHex;\r\n"
		"}\r\n"
		"</script>\r\n"
		"<body bgcolor=\"#0065A4\">"
		"<p><font size=\"8\">"
		"<b><center>WELCOME TO THESIS DEMO</center></b><br>"
		"</font>"
		"</p>"
		"<p>"
		"<font size=\"5\">"
		"<b><ins>AES-256 ENCRYPTION TEST</ins></b><br><br>"
		"<b>Input to Encrypt(ASCII):</b> <input type=\"text\" name=\"PlaintextInput\" maxlength=\"64\" id=\"PlaintextInput\" style=\"width: 500px;\"><button onClick=\"aes_ecb_encrypt()\"> Encrypt </button><br>"
		"<b>Input to Decrypt(ASCII):</b> <input type=\"text\" name=\"CipherTextInput\" maxlength=\"64\" id=\"CipherTextInput\" style=\"width: 500px;\"><button onClick=\"aes_ecb_decrypt()\"> Decrypt </button><br>"
		"<form action=\"\" method=\"get\">"
		"<b>Encrypted Data(HEX):</b> <input type=\"text\" name=\"CiphertextOutput\" maxlength=\"64\" id=\"CiphertextOutput\" style=\"width: 500px;\"><br>"
		"<b>Decrypted Data(HEX):</b> <input type=\"text\" name=\"PlaintextOutput\" maxlength=\"64\" id=\"PlaintextOutput\" style=\"width: 500px;\"><br><br>"
		"<input type=\"submit\">"
		"</form>"
		"</font>"
		"</p>"
		"<script type=\"text/javascript\" src=\"https://cdn.rawgit.com/ricmoo/aes-js/e27b99df/index.js\"></script>"
		"</body>"
		"</html>"
};
const u8 http_header[100] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
const u8 http_404_header[100] = "HTTP/1.1 404\r\nConnection: close\r\n\r\n";

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
char gPlaintext[128]="";
char gCiphertext[128]="";
uint32_t gLedBitMap={0};
uint8_t gSevenSegDigit=0;
int extract_web_page_info(char *rx_buff){
    int i = 0,j = 0;
    char *ret=(char*)0;
    char *ret1=(char*)0;
    uint32_t totalWords = 0;
    uint32_t len = 0;
    uint8_t hex[128];
	uint32_t tempBuff[128] = {0};
	uint32_t totalBytes = 0;
	uint8_t temp_str[130];
    ret = strstr(rx_buff,"GET /config HTTP/1.1");
    if(ret!=0){
    	return 1;
    }
    ret = strstr(rx_buff,"GET /aes HTTP/1.1");
    if(ret!=0){
    	return 2;
    }
    ret = strstr(rx_buff,"GET /config");
    ret1= strstr(rx_buff,"GET /aes");
    if((ret==0) && (ret1==0)){
    	xil_printf("GET /config and GET/aes not found!\r\n");
    	return -1;
    }else{
    	if(ret != 0){
			ret = strstr(rx_buff,"Led1");
			if(ret==0){
				return -2;
			}
			ret = strstr(rx_buff,"=");
			if(ret==0){
				return -3;
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
			return 1;
    	}else if(ret1!=0){
			ret = strstr(rx_buff,"CiphertextOutput");
			if(ret==0){
				return -4;
			}
			ret = strstr(rx_buff,"=");
			if(ret==0){
				return -5;
			}
			ret++;
			j= 0;
			while(*ret!='&'){
				   gCiphertext[j++] = *ret;
				   ret++;
			}
			gCiphertext[j] = '\0';
		   j=0;
			ret = strstr(ret,"=");
			if(ret==0){
				return -6;
			}
			ret++;
			j= 0;
			while(*ret!=' '){
				gPlaintext[j++] = *ret;
				   ret++;
			}
			gPlaintext[j] = '\0';
		   j=0;
		   xil_printf("CiphertextOutput = %s\r\nPlaintextOutput = %s\r\n", gCiphertext,gPlaintext);
		   //len = strlen(gCiphertext);
		   len = str_to_hex(gCiphertext,hex);
		   totalWords = build_aes_block(hex,len);
		   xil_printf("AES: noOfWords = %d\r\n",totalWords);
		   loadPlainTextInBram(data,totalWords);
		   xil_printf("***** Received CipherText Data *****\n\r");
		   displayBram(data_readaddr_start,totalWords);
		   startDecryption(totalWords);
		   xil_printf("***** Decrypted Data of Received CipherText Data *****\n\r");
		   displayBram(data_writeaddr_start + totalWords,totalWords);
		   loadFromBramToBuf(data_writeaddr_start + totalWords,tempBuff,totalWords);
		   totalBytes = convert_aes_datablock_to_byte(temp_str,tempBuff,totalWords);
		   temp_str[totalBytes] = '\0';
		   xil_printf("*******************************************************************\r\n");
		   xil_printf("DECRYPTED STRING IS: %s\r\n",temp_str);
		   xil_printf("*******************************************************************\r\n");


		   totalWords = 0;
		   len = 0;
		   totalBytes = 0;
		   memset(hex,0,128);
		   memset(tempBuff,0,128*sizeof(uint32_t));
		   memset(temp_str,0,130);
		   len = str_to_hex(gPlaintext,hex);
		   totalWords = build_aes_block(hex,len);
		   xil_printf("AES: noOfWords = %d\r\n",totalWords);
		   loadPlainTextInBram(data,totalWords);
		   xil_printf("***** Received PlainText Data *****\n\r");
		   displayBram(data_readaddr_start,totalWords);
		   startEncryption(totalWords);
		   xil_printf("***** Encrypted Data of Received Plaintext Data *****\n\r");
		   displayBram(data_writeaddr_start + totalWords,totalWords);
		   loadFromBramToBuf(data_writeaddr_start + totalWords,tempBuff,totalWords);
		   totalBytes = convert_aes_datablock_to_byte(temp_str,tempBuff,totalWords);
		   temp_str[totalBytes] = '\0';
		   xil_printf("*******************************************************************\r\n");
		   xil_printf("ENCRYPTED STRING IS: %s\r\n",temp_str);
		   xil_printf("*******************************************************************\r\n");
		   return 2;
    	}
    }
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
//const char * szSsid = "Himalayan Panthers";
const char * szSsid = "SM-G950U5F0";
// select 1 for the security you want, or none for no security
#define USE_WPA2_PASSPHRASE
//#define USE_WPA2_KEY
//#define USE_WEP40
//#define USE_WEP104
//#define USE_WF_CONFIG_H

// modify the security key to what you have.
#if defined(USE_WPA2_PASSPHRASE)

    //const char * szPassPhrase = "Aayogorkhali@2016";
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
	aestest();
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
	int http_ret = 0;
	int i = 0;
//	uint32_t totalWords = 0;
//	uint32_t tempBuff[32] = {0};
//	uint32_t totalBytes = 0;
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
		            http_ret = extract_web_page_info((char*)rgbRead);
		            if(http_ret == 1){
						update_webpage();
						LED_IP_mWriteReg(0x44a40000,0,gLedBitMap);
						SEVENSEGMENT_mWriteReg(0x44a50000,0,gSevenSegDigit);
		            }else if(http_ret == 2){

		            }else if(http_ret == -1){
		            	xil_printf("A:404 not found!\r\n");
		            }
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
		            if(http_ret == 1){
						ptcpClient->writeStream(http_header, strlen((const char*)http_header));
						ptcpClient->writeStream(homepage, strlen((const char*)homepage));
		            }else if(http_ret == 2){
						ptcpClient->writeStream(http_header, strlen((const char*)http_header));
						ptcpClient->writeStream(aes_page, strlen((const char*)aes_page));
		            }else if(http_ret == -1){
		            	xil_printf("B:404 not found!\r\n");
		            	ptcpClient->writeStream(http_404_header, strlen((const char*)http_404_header));
		            }
		            state = CLOSE;
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
