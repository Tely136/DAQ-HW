`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/01/2026 04:23:36 PM
// Design Name: 
// Module Name: pos_edge_detect
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


module pos_edge_detect(
    input a,
    input clk,
    output wire b
    );
    
    reg a_delay;
    
    assign b = a & ~a_delay;
    
    always @(posedge clk) begin
        a_delay <= a;
    end

endmodule
