%% Initialize
clc; clearvars;

% keep clock_hz consistent with CLOCK_HZ in params.h
clock_hz = 50000000;
server_address = "192.168.137.20";
server_port = 8080;

daq = tcpclient(server_address, server_port, "Timeout", 120);
daq.ByteOrder = "little-endian";
ready_message = readline(daq);
assert(ready_message == "READY", "DAQ not ready");

%% Configure
n_shots = 10;
n_bins = 10;
cycles_per_bin = 10;
source_period_cycles = 4;
trigger_rate_hz = 20;

validateattributes(n_shots, {'numeric'}, {'scalar','integer','positive','<=',intmax('int32')});
validateattributes(n_bins, {'numeric'}, {'scalar','integer','>=',1,'<=',128});
validateattributes(cycles_per_bin, {'numeric'}, {'scalar','integer','positive','<=',intmax('int32')});
validateattributes(source_period_cycles, {'numeric'}, {'scalar','integer','>=',0,'<=',double(intmax('uint32'))});
validateattributes(trigger_rate_hz, {'numeric'}, {'scalar','real','finite','>=',clock_hz / double(intmax('uint32')),'<=',clock_hz / 2});
assert(double(n_shots) * double(cycles_per_bin) <= double(intmax('int32')), ...
       "Requested counts could overflow MCS accumulation");

% allow for the acquisition time when using a slower trigger
trigger_period_cycles = round(clock_hz / trigger_rate_hz);
actual_trigger_rate_hz = clock_hz / trigger_period_cycles;
daq.Timeout = max(120, 1.2 * n_shots / actual_trigger_rate_hz + 5);
fprintf("Trigger rate: requested %.9g Hz, programmed %.9g Hz\n", ...
        trigger_rate_hz, actual_trigger_rate_hz);

% CONFIG n_shots n_bins cycles_per_bin source_period_cycles trigger_rate_hz
config_message = sprintf("CONFIG %u %u %u %u %.17g", ...
    n_shots, n_bins, cycles_per_bin, source_period_cycles, trigger_rate_hz);
writeline(daq, config_message);

%% Start and read data
writeline(daq, "START");
counts = double(read(daq, n_bins, "uint32"));
counts = counts(:);

bin_width_s = cycles_per_bin / clock_hz;
exposure_s = n_shots * bin_width_s;
counts_per_shot = counts / n_shots;
count_rate_hz = counts / exposure_s;

% each shot can start at an unknown phase of the continuous source
if source_period_cycles == 0
    expected_min = 0;
    expected_max = 0;
    source_rate_hz = 0;
else
    expected_min = n_shots * floor(cycles_per_bin / source_period_cycles);
    expected_max = n_shots * ceil(cycles_per_bin / source_period_cycles);
    source_rate_hz = clock_hz / source_period_cycles;
end

passed = counts >= expected_min & counts <= expected_max;
bin = (0:n_bins-1)';
results = table(bin, counts, counts_per_shot, count_rate_hz, ...
    repmat(expected_min, n_bins, 1), repmat(expected_max, n_bins, 1), passed, ...
    'VariableNames', {'bin','counts','counts_per_shot','rate_hz', ...
                      'expected_min','expected_max','passed'});
fprintf("Source rate: %.3f events/s; bin width: %.3f us\n", source_rate_hz, bin_width_s * 1e6);
disp(results);
fprintf("Bounds check: %d/%d bins passed\n", sum(passed), n_bins);

%% Finish DAQ session
writeline(daq, "END");
clear daq;