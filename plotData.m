%% AUTOR Kamil Wieczorek
clc
clear all
close all
%% load data
fd = fopen('speech_example_8KHz.txt','r');
fd1 = fopen('IIR.txt','r');
fd2 = fopen('FIR.txt','r');
%fd3 = fopen('fixed_point_IIR.txt','r');
fd4 = fopen('fixed_point_FIR.txt','r');
if(fd < 0 || fd1 < 0 || fd2 < 0 || fd4 < 0)
    disp("Can't open file");
    return;
end
formatSpec = '%d';
y = fscanf(fd,formatSpec);
y_iir = fscanf(fd1,formatSpec);
y_fir = fscanf(fd2,formatSpec);
%y_fixed_point_iir = fscanf(fd3,formatSpec);
y_fixed_point_fir = fscanf(fd4,formatSpec);
fclose(fd);
fclose(fd1);
fclose(fd2);
%fclose(fd3);
fclose(fd4);
%% plot speech
plotSpeech(y,y2,'Example speech - fir filter');
%% plot Power Spectral Density
plotPSD(y,y2, 64000,'Power Spectral Density');
%% functions section
function plotSpeech(yBefore, yAfter, plotTitle)
    figure
    plot(yBefore)
    title(plotTitle);
    hold on
    plot(yAfter)
    legend('Before', 'After');
    xlabel('Sample number');
    ylabel('Sample value');
end
function plotPSD(yBefore, yAfter, N, plotTitle)
    figure
    Tp=1/N;
    X_fft = Tp*fft(yBefore(8001:16000), N);
    X_gwm = (abs(X_fft(1:N/2)).^2)/N/Tp;
    plot(0:N/2-1,10*log10(X_gwm));
    hold on
    X_fft = Tp*fft(yAfter(8001:16000), N);
    X_gwm = (abs(X_fft(1:N/2)).^2)/N/Tp;
    plot(0:N/2-1,10*log10(X_gwm));
    title(plotTitle);
    legend('Before', 'After');
    xlabel('Frequency [Hz]');
    ylabel('PSD [dB]');
    xlim([0 N/2])
end