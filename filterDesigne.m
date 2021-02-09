%% AUTOR Kamil Wieczorek
clc
clear all
close all
%% init
fp = 64000;
fn = fp/2;
N = fp;
fd = fopen("params.h", "w");
%% IIR
Wp = 7000/fn;
Ws = 8000/fn;
Rp = 0.5;
Rs = 70;
[n_iir, Wn] = ellipord(Wp, Ws, Rp, Rs);
[b, a] = ellip(n_iir, Rp, Rs, Wn);
[h, w] = freqz(b, a, N, N);
x = w;
y = 20*log10(abs(h));
figure
plot(x,y)
title("IIR");
xlabel('Frequency [Hz]');
ylabel('Magnitude [dB]');
xlim([0 32000]);
%% export filter params
fprintf(fd, "static double b_iir[] = {");
for i=1:size(b,2)
        fprintf(fd, "%.16e", b(i));
        if(i == size(b, 2))
                fprintf(fd, "};");
        else
                fprintf(fd, ",");
        end
end
fprintf(fd, "\n");
fprintf(fd, "static double a_iir[] = {");
for i=1:size(a,2)
        fprintf(fd, "%.16e", a(i));
        if(i == size(a, 2))
                fprintf(fd, "};");
        else
                fprintf(fd, ",");
        end
end
fprintf(fd, "\n");

%% FIR
n_fir = 150;
b = fir2(n_fir, [0 7000/fn 7800/fn 1], [1 1 0.00028 0.00028]);
[h, w] = freqz(b, 1, N, N);
x = w;
y = 20*log10(abs(h));
figure
plot(x,y)
title("FIR");
xlabel('Frequency [Hz]');
ylabel('Magnitude [dB]');
xlim([0 32000]);
%% export filter params
fprintf(fd, "static double b_fir[] = {");
for i=1:size(b,2)
        fprintf(fd, "%.16e", b(i));
        if(i == size(b, 2))
                fprintf(fd, "};");
        else
                fprintf(fd, ",");
        end
end
%% close file
fclose(fd);