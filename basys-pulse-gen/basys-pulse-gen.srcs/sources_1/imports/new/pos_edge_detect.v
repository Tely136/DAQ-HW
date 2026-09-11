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


module bus_pos_edge_det #(
    parameter WIDTH = 8 // Define the width of the bus
)(
    input  wire             clk,      // System clock
    input  wire [WIDTH-1:0] bus_in,   // Input bus to monitor
    output wire [WIDTH-1:0] edge_out  // Output pulse bus (1 cycle wide)
);

    // Register to store the delayed version of the bus
    reg [WIDTH-1:0] bus_dly;

    // Pipeline stage: Delay the input bus by exactly 1 clock cycle
    always @(posedge clk) begin
        begin
            bus_dly <= bus_in;
        end
    end

    // Bitwise combinational logic: (Current State) AND (NOT Previous State)
    assign edge_out = bus_in & ~bus_dly;

endmodule

