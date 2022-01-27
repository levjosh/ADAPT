delimiter = ',';
loc = "D:\4thYear\ADAPT\Data\Other_Data\121221\Test5.txt";
data = importdata(loc, delimiter);
time = data(:,1);
bicep = data(:,2);
% tricep = data(:,3);
% tricep = tricep - 600;

[B, A] = butter(8, 0.2, 'high');  % 4th order butterworth filter set to highpass that keeps normalized frequency values over 10%
%fvtool(B,A);                      % visualize the filter              

xF = filter(B,A,bicep);  % apply the filter to xF
var_xF = zeros(size(bicep));
var_of_var = zeros(size(bicep));

% calculate variance over a window of winLength
winLength = 243;
for i = 1:length(xF)-winLength
    var_xF(i+winLength) = var(bicep(i:i+winLength));
    var_of_var(i+winLength) = var(var_xF(i:i+winLength))/1000;
end

mov_avg = movmean(bicep, winLength);

rectified_signal = abs(bicep - mov_avg);
mov_rect_avg = movmean(rectified_signal, winLength);
mov_var_xF = movmean(var_xF, winLength);

figure;
plot(time, bicep);
hold on;
xF = xF - 600;
plot(time, xF);
% hold on;
% plot(time, var_xF);
% hold on;
% plot(time, rectified_signal);
% hold on;
% plot(time, mov_rect_avg)
title('EMG sensor signal over time');
xlabel('Time (s)');
ylabel('Incoming Signal');
legend({'Bicep', 'Moving Avg'}, 'FontSize', 14);


% plot(VarName1(1:end-100), var_xF);


% figure
% spectrogram(VarName3,100,50)