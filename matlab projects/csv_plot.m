clc
clear
close all

Array=csvread('Alexa Fluor 532.csv',1,0,[1,0,400,2]);
wavelength = Array(:, 1);
excitation = Array(:, 2);
emmission = Array(:, 3);
plot(wavelength, excitation,'m', wavelength, emmission,'b', 'LineWidth', 1.1);
hold on
n = 100;
y = (0:n);
excitationMax = 534;
emissionMax = 553;
ex = [repmat(excitationMax,1,n) excitationMax];
em = [repmat(emissionMax,1,n) emissionMax];
plot(ex,y,'--m');
plot(em,y,'--b');
emTxt = '\leftarrow \lambda_{em} = 553nm';
text(553,50,emTxt);
exTxt = '\lambda_{ex} = 538nm \rightarrow ';
text(449,50,exTxt);
xlabel('Wavelength [nm]');
ylabel('Normalised Transmittance [%]');
title('Excitation and Emission Spectra of Alexa Fluor 532');