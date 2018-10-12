`timescale 10ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/18/2018 02:08:00 PM
// Design Name: 
// Module Name: aes256
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module aes256(input clkIn,
               input resetIn,
               input [1:0]startIn,
               input [8:0]lengthIn,
               input [31:0]keyIn,
               input [31:0]dataIn,               
               output [7:0]keyAddrOut,
               output [8:0]dataAddrOut,               
              // output [127:0]dataOut,
               output reg[31:0]data32,
               output reg[2:0]doneOut,
               output reg[3:0]weOut
               //output dataBufReady,
               //output keyBufReady                                             
              );
//wire en;
wire [127:0]dataO;
wire [1919:0]keyO;
wire [127:0]dataOut;
wire dataBufReady,keyBufReady;
//assign en = (startIn > 0)?1:0;
//assign dataOut = keyIn;
wire [127:0] r0_out;
wire [127:0] r1_out,r2_out,r3_out,r4_out,r5_out,r6_out,r7_out,r8_out,r9_out,r10_out,r11_out,r12_out,r13_out;
reg [5:0]doneCount;
reg [2:0]data32Count;
reg [5:0]totalCount;
reg decrypt,flag; 
reg wrEn;
assign r0_out = dataO^keyO[1919:1792]; // initial round

keyAddrCounter KC0(.clk(clkIn),
                   .rst(resetIn),
                   .start(startIn),
                   .keyAddrCount(keyAddrOut)                    
                   );
keyBuffer KB0(.clk(clkIn),
              .reset(resetIn),
              .start(startIn),
              .addr(keyAddrOut),
              .keyIn(keyIn),
              .keyOut(keyO),
              .keyReady(keyBufReady)
             );
dataAddrCounter DC0(.clk(clkIn),
                    .rst(resetIn),
                    .start(keyBufReady),
                    .wrEn(wrEn),
                    .dataAddrCount(dataAddrOut)                    
                   );
dataBuffer DB0(.clk(clkIn),
               .reset(resetIn),
               .start(startIn),
               .addr(dataAddrOut),
               .dataIn(dataIn),
               .dataOut(dataO),
               .dataReady(dataBufReady)
               );
rounds r1(.clk(clkIn),.decrypt_i(decrypt),.data(r0_out),.keyin(keyO[1791:1664]),.rndout(r1_out));
rounds r2(.clk(clkIn),.decrypt_i(decrypt),.data(r1_out),.keyin(keyO[1663:1536]),.rndout(r2_out));
rounds r3(.clk(clkIn),.decrypt_i(decrypt),.data(r2_out),.keyin(keyO[1535:1408]),.rndout(r3_out));
rounds r4(.clk(clkIn),.decrypt_i(decrypt),.data(r3_out),.keyin(keyO[1407:1280]),.rndout(r4_out));
rounds r5(.clk(clkIn),.decrypt_i(decrypt),.data(r4_out),.keyin(keyO[1279:1152]),.rndout(r5_out));
rounds r6(.clk(clkIn),.decrypt_i(decrypt),.data(r5_out),.keyin(keyO[1151:1024]),.rndout(r6_out));
rounds r7(.clk(clkIn),.decrypt_i(decrypt),.data(r6_out),.keyin(keyO[1023:896]),.rndout(r7_out));
rounds r8(.clk(clkIn),.decrypt_i(decrypt),.data(r7_out),.keyin(keyO[895:768]),.rndout(r8_out));
rounds r9(.clk(clkIn),.decrypt_i(decrypt),.data(r8_out),.keyin(keyO[767:640]),.rndout(r9_out));
rounds r10(.clk(clkIn),.decrypt_i(decrypt),.data(r9_out),.keyin(keyO[639:512]),.rndout(r10_out));
rounds r11(.clk(clkIn),.decrypt_i(decrypt),.data(r10_out),.keyin(keyO[511:384]),.rndout(r11_out));
rounds r12(.clk(clkIn),.decrypt_i(decrypt),.data(r11_out),.keyin(keyO[383:256]),.rndout(r12_out));
rounds r13(.clk(clkIn),.decrypt_i(decrypt),.data(r12_out),.keyin(keyO[255:128]),.rndout(r13_out));
rounndlast r14(.clk(clkIn),.decrypt_i(decrypt),.rin(r13_out),.keylastin(keyO[127:0]),.fout(dataOut));

always @(posedge clkIn or negedge resetIn)begin
      if(!resetIn) begin
            decrypt <= 0;
      end 
      else begin
            if(startIn == 1)begin
                decrypt <= 0;
            end else if(startIn == 2)begin
                decrypt <= 1;
            end
      end
end

always @(posedge clkIn or negedge resetIn)begin
      if(!resetIn) begin
            weOut <= 4'b0;
            doneCount <= 0;
            data32Count <= 0;
            totalCount <= 0;
            doneOut <= 0;
            wrEn <= 0;
      end 
      else begin
            if(dataBufReady)begin
                if(doneCount < 41)begin
                    doneCount <= doneCount + 1;
                end else if(totalCount < 16)begin
                    weOut <= 4'b1111;
                    wrEn <= 1;
                    totalCount <= totalCount + 1;
                    if(data32Count == 0)begin
                        data32 <= dataOut[127:96];
                        data32Count <= data32Count + 1;
                    end else if(data32Count == 1)begin
                        data32 <= dataOut[95:64];
                        data32Count <= data32Count + 1;
                    end else if(data32Count == 2)begin
                        data32 <= dataOut[63:32];
                        data32Count <= data32Count + 1;
                    end else if(data32Count == 3)begin
                        data32 <= dataOut[31:0];
                        data32Count <= 0;
                    end else begin
                        //data32Count <= 0;
                    end
                end else begin
                       doneOut <= 3'b111;
                       weOut <= 4'b0000;
                       wrEn <= 0;
                end
            end
      end
    end
endmodule
