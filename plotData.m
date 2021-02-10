%% AUTOR Kamil Wieczorek
clc
clear all
close all
%% load data
fd = fopen('speech_example_8KHz.txt','r');
fd1 = fopen('IIR.txt','r');
fd2 = fopen('FIR.txt','r');
if(fd < 0 || fd1 < 0 || fd2 < 0 )
    disp("Can't open file");
    return;
end
formatSpec = '%d';
y = fscanf(fd,formatSpec);
y_iir = fscanf(fd1,formatSpec);
y_fir = fscanf(fd2,formatSpec);
fclose(fd);
fclose(fd1);
fclose(fd2);
%% plot speech
plotSpeech(y,y_iir,'Example sound - iir filter');
plotSpeech(y,y_fir,'Example sound - fir filter');
%% plot Power Spectral Density
plotPSD(y,y_iir, 64000,'Power Spectral Density - iir filter');
plotPSD(y,y_fir, 64000,'Power Spectral Density - fir filter');
%% functions section
function plotSignal(yBefore, yAfter, plotTitle)
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
    X_fft = Tp*fft(yBefore, N);
    X_gwm = (abs(X_fft(1:N/2)).^2)/N/Tp;
    plot(0:N/2-1,10*log10(X_gwm));
    hold on
    X_fft = Tp*fft(yAfter, N);
    X_gwm = (abs(X_fft(1:N/2)).^2)/N/Tp;
    plot(0:N/2-1,10*log10(X_gwm));
    title(plotTitle);
    legend('Before', 'After');
    xlabel('Frequency [Hz]');
    ylabel('PSD [dB]');
    xlim([0 N/2])
end