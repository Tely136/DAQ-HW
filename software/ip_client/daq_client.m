%% Initialize
clc; clearvars;
    


% Connect to the DAQ server
daq = tcpclient("192.168.137.20",8080);
daq.ByteOrder = "little-endian";

% Check if the DAQ is ready
aq = readline(daq);
assert(aq == "READY", "DAQ not ready");

%% Configure
N_SHOT = 1201;
N_BINS = 10;
N_CLK = 10;

% Attempt to read data from the DAQ without configuring it first
% writeline(daq,"START");

% Configure the DAQ and read data 1
config_msg = sprintf("CONFIG %i %i %i", N_SHOT, N_BINS, N_CLK); 
writeline(daq,config_msg);

%% Start and read data
writeline(daq,"START");
a = read(daq, N_BINS, "uint32");
disp(a);

% % Configure the DAQ and read data 2
% config_msg = sprintf("CONFIG %i %i %i", 20, 5, 10);
% writeline(daq,config_msg);

% writeline(daq,"START");
% a = read(daq, 5, "uint32");
% disp(a);

%% Finish DAQ session
% Finish the DAQ session
writeline(daq,"END");
