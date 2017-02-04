clc
clear
close all

x = linspace(0, 1, 256);
line = x;
%plot(x,line);
% hold on

Kmu = 1;
muNormalise = 2*sqrt(2) + 4;
l0 = 200*50;     c0 = 0.042026;
l1 = 200*1;   c1 = 0.103124;

% crossing point of energy functions
%p = c0 + (c1-c0)/(sqrt(l0/l1) + 1)
%e0 = l0*(p-c0)^2
%e1 = l1*(p-c1)^2

p = c0 + (c1-c0)/5.5;
e0 = l0*(p-c0)^2;
e1 = l1*(p-c1)^2;
lambda1 = 1.5/(p-c1)^2;
kLAM = ((c1-c0)/(p-c0)-1)^2;
lambda0 = lambda1*kLAM;
%v*255

% E(0) = l0*|v_i - c0|^2
E0 = l0*(x-c0).^2;
% E(1) = l1*|v_i - c1|^2
E1 = l1*(x-c1).^2;
% plot(x, E0, '-b');
% plot(x, E1, '-r');

% mean markers
y = [0, 1];
c0x = [c0 c0];
c1x = [c1 c1];
% plot(c0x, y, '--b');
% plot(c1x, y, '--r');


% mark absolute fg and bbg labels
e0_labels = zeros(256);
e1_labels = zeros(256);
for i=1:256
    if (E1(i)>E0(i)+Kmu*muNormalise)
        e1_labels(i)=1;
    end
    
    if (E0(i)>E1(i)+Kmu*muNormalise)
        e0_labels(i)=1;
    end
end

% grid on;
% 
% title('Normalised Pixel Intensity');
% xlabel('Normalised Pixel Intensity');
% ylabel('Data Energy Weights');
% l=legend('E^{i}(0)','E^{i}(1)');
% rect = [0.65 0.775 0.2 0.1]; % position of legend
% set(l,'Position',rect);


x = linspace(0.018, 0.019, 256);
s0 = 0.00211362; u0 = 0.010768;
s1 = 0.140498; u1 = 0.321058;

a = s1-s0;
b = 2*u1*s0 - 2*u0*s1;
c = (u0^2)*s1 - (u1^2)*s0 + 2*s0*s1*log(s0/s1);
x_ = (-b + sqrt(b*b - 4*a*c))/(2*c);

bgNorm = normpdf(x,u0,s0);
fgNorm = normpdf(x,u1,s1);
% figure
% plot(x,bgNorm,'-b', x,fgNorm,'-r');

%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%kMu = 1;    muNormalise = 2*sqrt(2) + 4;
alpha = 50;    l1 = 200; l0 = alpha*l1;   kMu = 1.5;    
c0 = 0.00668219;%0.05; %0.042026;  
c1 = 0.110302;%0.95; %0.103124;
fprintf('alpha = %f, l0 = %f, l1 = %f\n', alpha, l0, l1);
fprintf('c0 = %f, c1 = %f\n\n', c0, c1);

fprintf('kMu Calculation\n');
% The point at which the energies are equal
p1 = c0 + (c0-c1)/(sqrt(alpha)-1);
p2 = c0 + (c0-c1)/(-sqrt(alpha)-1);
fprintf('E(0) = E(1) at p = %f and %f\n',p1, p2);

% kMu lower bound
fMu_max = (2*sqrt(2)+4);
kMu_lb = ((2*c1*l1 - 2*c0*l0)^2/(4*(l0-l1)) - l0*c0^2 + l1*c1^2)/fMu_max;
fprintf('Lowerbound on kMu = %f\n',kMu_lb);

% h0 = c0
kMu_h0_c0 = (l1*(c0-c1)^2)/fMu_max;
fprintf('kMu for h0=c0: %f\n',kMu_h0_c0);

% % h0 = p
% kMu_h0_p = (l1*(p-c1)^2 - l0*(p-c0)^2)/fMu_max;
% fprintf('kMu for h0=p: %f\n',kMu_h0_p);
% 
% % h1 = p
% kMu_h1_p = (l0*(p-c0)^2 - l1*(p-c1)^2)/fMu_max;
% fprintf('kMu for h1=p: %f\n',kMu_h1_p);

% h1 = c1
kMu_h1_c1 = (l0*(c1-c0)^2)/fMu_max;
fprintf('kMu for h1=c1: %f\n',kMu_h1_c1);

fprintf('Complete\n\n');

% Plotting the Energy Function with the maximum smoothing shift
k = kMu*fMu_max;
fprintf('u = %f\n', k);
x = linspace(0,c1,256);
E0 = l0*(x-c0).^2;
E1 = l1*(x-c1).^2;
E0_shift = E0 + k;
E1_shift = E1 + k;

% alpha lower bound
alpha_min = k/(l1*(c1-c0)^2);
fprintf('Alpha lowerbound (h1=c1) = %f\n', alpha_min);

% alpha intersection
alpha_int = 1/((c1-c0)*sqrt(l1/k)-1)^2;
fprintf('Alpha intersection (h1=p) = %f\n', alpha_int);

% h1 (beyond this is definitely fg)
a = l0-l1;
b = -2*c0*l0 + 2*c1*l1;
c = l0*c0^2 - l1*c1^2 - k;
%h1_1 = (-b+sqrt(b^2-4*a*c))/(2*a)
h1_2 = (-b+sqrt(b^2-4*a*c))/(2*a);
fprintf('Beyond this is definitely FG. h1 = %f\n\n', h1_2);

% h0 (below this is definitely bg)
a = l1-l0;
b = -2*c1*l1 + 2*c0*l0;
c = l1*c1^2 - l0*c0^2 - k;
%h0_1 = (-b-sqrt(b^2-4*a*c))/(2*a)
%h0_2 = (-b+sqrt(b^2-4*a*c))/(2*a)

% mean markers
y = [0, E0_shift(256)];
c0x = [c0 c0];
c1x = [c1 c1];
px = [p2 p2];
muy = kMu*ones(256);
muy_shift = muy .* fMu_max;

figure
plot(x,E0,'-b', x,E1,'-r', x,E0_shift,'-c', x,E1_shift,'-m');
%plot(x,E0-E1,'-b');
hold on
plot(c0x,y,'--b', c1x,y,'--r', px,y,'--k', (c0x+c1x)/2,y, [h1_2 h1_2],y,'--k');
plot(x,muy,'k--', x,muy_shift,'k--');

% determining l1 from alpha and u
u_lowerb = (l1*(alpha*c0^2-c1^2))/(fMu_max)
u = 1;
h1 = 0.040187;
alpha_lowerb = c1^2/c0^2
l1 = (u*fMu_max)/(alpha*(h1-c0)^2-(h1-c1)^2);
l1_lowerb = (u*fMu_max)/(alpha*(h1-c0)^2-(h1-c1)^2)
fprintf('Manually set h1 = %f\n', h1);
fprintf('c1 = %f\n', c1)
fprintf('Calculation for l1 = %f\n', l1);
h1auto = ((alpha*c0-c1) + sqrt(alpha*(c0-c1)^2+(u*fMu_max)/l1*(alpha-1)))/(alpha-1);
fprintf('Reverse calculation for h1 = %f\n', h1auto);

c0 = 0.00668382;
c1 = 0.110308;
s0 = 0.00780965;
s1 = 0.0766097;
a = s1-s0;
b = 2*c0*s1-2*c1*s0;
c = c0^2*s1-c1^2*s0-2*s0^2;
gauss_equ = (-b + sqrt(b^2 - 4*a*c))/(2*a)
pe = gauss_equ/3.1
%pe = c0+1*s0;
alpha_calc = ((c1-c0)/(pe-c0)-1)^2
J0 = l0*c0^2;
J1 = 200*c1^2;




















% x = linspace(-10,10,200);
% a = [1, 1, 1];
% b = [0, -5, 5];
% c = [0, 0, 0];
% %y = [a(1)*x.^2 + b(1)*x + c(1), a(2)*x.^2 + b(2)*x + c(2), a(3)*x.^2 + b(3)*x + c(3)];
% y1 = a(1)*x.^2 + b(1)*x + c(1);
% y2 = a(2)*x.^2 + b(2)*x + c(2);
% y3 = a(3)*x.^2 + b(3)*x + c(3);
% figure
% plot(x,y1,'r', x,y2,'g', x,y3,'b');
% grid on
% %plot(x,y1,'r');


% % h1 (beyond this is definitely fg)
% a = linspace(0,5,100);
% b = -2*c0*l0 + 2*c1*l1;
% c = l0*c0^2 - l1*c1^2 - k;
% h = ones(100);
% for i=1:100
%     h(i) = ((a(i)*c0-c1)+sqrt(a(i)*(c0-c1)^2+(k/l1)*(a(i)-1)))/(a(i)-1);
% end
% 
% figure
% plot(a,h);
