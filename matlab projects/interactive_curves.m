clc
clear
close all

Imax = 200;
x = linspace(0,255,256);

% foreground curve
mu1FG = 50;
sigma1FG = 15;
y1FG = 1/sqrt(2*pi*sigma1FG^2) * exp(-(x-mu1FG).^2/(2*sigma1FG^2));

mu2FG = 150;
sigma2FG = 30;
y2FG = 1/sqrt(2*pi*sigma2FG^2) * exp(-(x-mu2FG).^2/(2*sigma2FG^2));

kfg = 10;
yFG = kfg*(y1FG + y2FG);
yFGnorm = yFG/max(yFG);

xpara = mu1FG:Imax;
% parabola part
pFG_Imax = yFGnorm(Imax);
yFGpara = (pFG_Imax-1)/(Imax-mu1FG)^2*(xpara-mu1FG).^2+1;

yFGfin = 1:256;
yFGfin(mu1FG:Imax-1) = yFGpara(1:Imax-mu1FG);
a = mu1FG:Imax-1;
aa = 1:Imax-mu1FG;
for i=1:mu1FG
    yFGfin(i) = yFGnorm(i);
end
for i=mu1FG:Imax
    yFGfin(i) = yFGpara(i-mu1FG+1);
end
for i=Imax+1:256
    yFGfin(i) = 0;
end

figure
plot(x,yFG,'--r', x,yFGnorm, '--m', Imax, pFG_Imax, '*m', x, yFGfin, 'r');
neworder = {
    '0' [0]
    '\mu_1' [mu1FG]
    '100' [100]
    '\mu_2' [mu2FG]
    'I_{max}' [Imax]
   % '200' [200]
    '250' [250]
    '300' [300]};
%set(gca,'XTickLabel',{'0','\mu_1', '100','\mu_2','200','250','300'});
set(gca,'XTickLabel',neworder(:,1));
hold on
xlabel('Intensity');
ylabel('Source Terminal Link Weight');
rect = [0.605 0.755 0.2 0.1]; % position of legend
l=legend('EMGMM k=2','Normalised EMGMM k=2','F','Final S-Link Weight Function');
set(l,'Position',rect);
plot([Imax, Imax],[0,1], '--r',[mu1FG, mu1FG],[0,1], '--r',[mu2FG, mu2FG],[0,0.5], '--r');


% background curve
xbgmax = 50;
x = 0:xbgmax;

mu1BG = 5;
sigma1BG = 5;
y1BG = 1/sqrt(2*pi*sigma1BG^2) * exp(-(x-mu1BG).^2/(2*sigma1BG^2));

mu2BG = 20;
sigma2BG = 4;
y2BG = 1/sqrt(2*pi*sigma2BG^2) * exp(-(x-mu2BG).^2/(2*sigma2BG^2));

kbg = 5;
yBG = kbg*(y1BG + y2BG);
yBGnorm = yBG/max(yBG);

 xpara = 0:mu2BG;
 % parabola part
 pBG0 = yBGnorm(1);
 yBGpara = (pBG0-1)/mu2BG^2*(xpara-mu2BG).^2+1;
 
 yBGfin = 1:xbgmax;
% yBGfin(mu1FG:Imax-1) = yFGpara(1:Imax-mu1FG);
% a = mu1FG:Imax-1;
% aa = 1:Imax-mu1FG;
for i=1:mu2BG
    yBGfin(i) = yBGpara(i);
end
for i=mu2BG+1:xbgmax
    yBGfin(i) = yBGnorm(i);
end

figure
plot(x,yBG,'--b',x,yBGnorm,'--c', 0, pBG0, '*b', 0:xbgmax-1, yBGfin, '-b');
neworder = {
    '0' [0]
    '\mu_1' [mu1FG]
     '10' [10]
    '15' [15]
    '\mu_2' [mu2FG]
    '25' [25]
    '30' [30]
    '35' [35]
    '40' [40]
    '45' [45]
    '50' [50]};
set(gca,'XTickLabel',neworder(:,1));
xlabel('Intensity');
ylabel('Sink Terminal Link Weight');
rect = [0.605 0.755 0.2 0.1]; % position of legend
l=legend('EMGMM k=2','Normalised EMGMM k=2','B','Final T-Link Weight Function');
set(l,'Position',rect);
hold on;
plot([mu1BG mu1BG],[0,0.8],'--b',[mu2BG mu2BG],[0,1],'--b');

