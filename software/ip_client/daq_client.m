clc; clearvars;

daq = tcpclient("192.168.137.20",8080);

daq.ByteOrder = "little-endian";
a = read(daq,5,"uint32");