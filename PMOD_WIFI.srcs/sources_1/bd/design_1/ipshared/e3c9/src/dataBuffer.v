`timescale 10ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/11/2018 12:45:29 PM
// Design Name: 
// Module Name: dataBuffer
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


module dataBuffer(input clk,
                  input reset,
                  input [1:0]start,
                  input [8:0]length,
                  input [8:0]addr,
                  input [31:0]dataIn,
                  output reg [127:0]dataOut,
                  output reg dataReady
    );
reg [31:0] RAM [32:0]; // 32bit RAM x 16 slots= 512 bit(64 Byte) RAM
always @(posedge clk or negedge reset)begin
  if(!reset) begin
    dataOut <= 0;
    dataReady <= 0;
  end 
  else if(start > 0)begin
       if(addr < (length+1))begin 
             RAM[addr-1] <= dataIn;
       end 
       if(addr == 5)begin 
              dataOut <= {RAM[0],RAM[1],RAM[2],RAM[3]};
              dataReady <=  1;
       end else if(addr == 9)begin
              dataOut <= {RAM[4],RAM[5],RAM[6],RAM[7]};
              dataReady <=  1;       
       end else if(addr == 13)begin
              dataOut <= {RAM[8],RAM[9],RAM[10],RAM[11]};
              dataReady <=  1;     
       end else if(addr == 17)begin
              dataOut <= {RAM[12],RAM[13],RAM[14],RAM[15]};
              dataReady <=  1; 
       end else if(addr == 21)begin
            dataOut <= {RAM[16],RAM[17],RAM[18],RAM[19]};
            dataReady <=  1; 
       end else if(addr == 25)begin
            dataOut <= {RAM[20],RAM[21],RAM[22],RAM[23]};
            dataReady <=  1; 
       end else if(addr == 29)begin
            dataOut <= {RAM[24],RAM[25],RAM[26],RAM[27]};
            dataReady <=  1; 
       end else if(addr == 33)begin
            dataOut <= {RAM[28],RAM[29],RAM[30],RAM[31]};
            dataReady <=  1;
       end else begin
              //dataReady <=  0;
       end
  end
end
endmodule
