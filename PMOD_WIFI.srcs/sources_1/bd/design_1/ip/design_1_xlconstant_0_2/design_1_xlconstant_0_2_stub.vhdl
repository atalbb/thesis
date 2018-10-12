-- Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2017.2 (win64) Build 1909853 Thu Jun 15 18:39:09 MDT 2017
-- Date        : Tue Oct  9 21:00:06 2018
-- Host        : DESKTOP-GJPCRJL running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               c:/Xilinx/Vivado/MyProjects/PMOD_WIFI_CLIENT/PMOD_WIFI.srcs/sources_1/bd/design_1/ip/design_1_xlconstant_0_2/design_1_xlconstant_0_2_stub.vhdl
-- Design      : design_1_xlconstant_0_2
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a100tcsg324-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity design_1_xlconstant_0_2 is
  Port ( 
    dout : out STD_LOGIC_VECTOR ( 31 downto 0 )
  );

end design_1_xlconstant_0_2;

architecture stub of design_1_xlconstant_0_2 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "dout[31:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "xlconstant_v1_1_3_xlconstant,Vivado 2017.2";
begin
end;
