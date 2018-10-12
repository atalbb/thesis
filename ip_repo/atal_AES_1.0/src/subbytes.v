`timescale 10ns / 1ps

//module subbytes(clk,data,sb,decrypt);


//input [127:0] data;
//input decrypt,clk;
//output [127:0] sb;

//     sbox q0( .a(data[127:120]),.c(sb[127:120]),.decrypt(decrypt) );
//     sbox q1( .a(data[119:112]),.c(sb[119:112]),.decrypt(decrypt) );
//     sbox q2( .a(data[111:104]),.c(sb[111:104]),.decrypt(decrypt) );
//     sbox q3( .a(data[103:96]),.c(sb[103:96]),.decrypt(decrypt) );
     
//     sbox q4( .a(data[95:88]),.c(sb[95:88]),.decrypt(decrypt) );
//     sbox q5( .a(data[87:80]),.c(sb[87:80]),.decrypt(decrypt) );
//     sbox q6( .a(data[79:72]),.c(sb[79:72]),.decrypt(decrypt) );
//     sbox q7( .a(data[71:64]),.c(sb[71:64]),.decrypt(decrypt) );
     
//     sbox q8( .a(data[63:56]),.c(sb[63:56]),.decrypt(decrypt) );
//     sbox q9( .a(data[55:48]),.c(sb[55:48]),.decrypt(decrypt) );
//     sbox q10(.a(data[47:40]),.c(sb[47:40]),.decrypt(decrypt) );
//     sbox q11(.a(data[39:32]),.c(sb[39:32]),.decrypt(decrypt) );
     
//     sbox q12(.a(data[31:24]),.c(sb[31:24]),.decrypt(decrypt) );
//     sbox q13(.a(data[23:16]),.c(sb[23:16]),.decrypt(decrypt) );
//     sbox q14(.a(data[15:8]),.c(sb[15:8]),.decrypt(decrypt) );
//     sbox q16(.a(data[7:0]),.c(sb[7:0]),.decrypt(decrypt) );

module subbytes(data,sb,decrypt);

input [127:0] data;
input decrypt;
output [127:0]sb;
wire [127:0]sb;
//wire [127:0]dataIn;
//wire [127:0]Sbox;
//wire [127:0]InvSbox;
//assign dataIn = (decrypt==0)?{data[127:120],data[87:80],data[47:40],data[7:0],data[95:88],data[55:48],data[15:8],data[103:96],data[63:56],data[23:16],data[111:104],data[71:64],data[31:24],data[119:112],data[79:72],data[39:32]}
//                             :{data[127:120],data[23:16],data[47:40],data[71:64],data[95:88],data[119:112],data[15:8],data[39:32],data[63:56],data[87:80],data[111:104],data[7:0],data[31:24],data[55:48],data[79:72],data[103:96]};




     sbox q0(.a(data[127:120]),.b(data[127:120]),.c(sb[127:120]),.decrypt(decrypt) );
     sbox q1(.a(data[87:80]),.b(data[23:16]),.c(sb[119:112]),.decrypt(decrypt) );
     sbox q2(.a(data[47:40]),.b(data[47:40]),.c(sb[111:104]),.decrypt(decrypt) );
     sbox q3(.a(data[7:0]),.b(data[71:64]),.c(sb[103:96]),.decrypt(decrypt) );
     
     sbox q4(.a(data[95:88]),.b(data[95:88]),.c(sb[95:88]),.decrypt(decrypt) );
     sbox q5(.a(data[55:48]),.b(data[119:112]),.c(sb[87:80]),.decrypt(decrypt) );
     sbox q6(.a(data[15:8]),.b(data[15:8]),.c(sb[79:72]),.decrypt(decrypt) );
     sbox q7(.a(data[103:96]),.b(data[39:32]),.c(sb[71:64]),.decrypt(decrypt) );
     
     sbox q8(.a(data[63:56]),.b(data[63:56]),.c(sb[63:56]),.decrypt(decrypt) );
     sbox q9(.a(data[23:16]),.b(data[87:80]),.c(sb[55:48]),.decrypt(decrypt) );
     sbox q10(.a(data[111:104]),.b(data[111:104]),.c(sb[47:40]),.decrypt(decrypt) );
     sbox q11(.a(data[71:64]),.b(data[7:0]),.c(sb[39:32]),.decrypt(decrypt) );
     
     sbox q12(.a(data[31:24]),.b(data[31:24]),.c(sb[31:24]),.decrypt(decrypt) );
     sbox q13(.a(data[119:112]),.b(data[55:48]),.c(sb[23:16]),.decrypt(decrypt) );
     sbox q14(.a(data[79:72]),.b(data[79:72]),.c(sb[15:8]),.decrypt(decrypt) );
     sbox q15(.a(data[39:32]),.b(data[103:96]),.c(sb[7:0]),.decrypt(decrypt) );






endmodule
