clc; clearvars;
    
N_SHOT = 50;
N_BINS = 20;
N_CLK = 100;

% Connect to the DAQ server
daq = tcpclient("192.168.137.20",8080);
daq.ByteOrder = "little-endian";

% Check if the DAQ is ready
aq = readline(daq);
assert(aq == "READY", "DAQ not ready");

% Attempt to read data from the DAQ without configuring it first
% writeline(daq,"START");

% Configure the DAQ and read data 1
config_msg = sprintf("CONFIG %i %i %i", N_SHOT, N_BINS, N_CLK); 
writeline(daq,config_msg);

writeline(daq,"START");
a = read(daq, N_BINS, "uint32");
disp(a);

% % Configure the DAQ and read data 2
% config_msg = sprintf("CONFIG %i %i %i", 20, 5, 10);
% writeline(daq,config_msg);

% writeline(daq,"START");
% a = read(daq, 5, "uint32");
% disp(a);

% Finish the DAQ session
writeline(daq,"END");
