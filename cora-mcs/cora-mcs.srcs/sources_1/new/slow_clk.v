`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/08/2026 08:26:31 PM
// Design Name: 
// Module Name: slow_clk
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


module slow_clk(
    input clk,
    output reg en
    );
    
    reg [31:0] div_count = 0;

    always @(posedge clk) begin
        if (div_count == 99999) begin
            div_count <= 0;
            en <= 1;
        end else begin
            div_count <= div_count + 1;
            en <= 0;
        end
    end
endmodule
