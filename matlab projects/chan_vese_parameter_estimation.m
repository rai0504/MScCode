clc
clear
close all

% The calculation of the parameters depends on how well we are able to
% approximate the final points
c0 = 0.010789;      % lower mean
c1 = 0.321058;      % higher mean

% Additional data we can gather
s0 = 0.00915068;
s1 = 0.140498;
fmax = 2*sqrt(2) + 4;

a = s0^2-s1^2;
b = -2*c1*s0^2 + 2*c0*s1^2;
c = c1^2*s0^2 - c0^2*s1^2 - 2*s0^2*s1^2*log(s0/s1);
pe = (-b - sqrt(b^2-4*a*c))/(2*a);
h = c1-1.85*s1;       % fg saturation point

% Parameters that need to be calculated from the approximated data
% alpha, u, l1
alpha_LB = ((h-c1)/(h-c0))^2;
alpha = 0.5*((c1-c0)/(pe-c0) - 1)^2;    % to ensure the energies meet at pe
u = 1;  % set this as the lowerbound of u always
l1 = (u*fmax)/(alpha*(h-c0)^2-(h-c1)^2); % to ensure we meet the fg saturation point

% do we meet the lower bound for mu
uLB = (l1*(alpha*c0^2-c1^2))/(fmax); % to ensure that values below c0 never have saturation source weights

fprintf('alpha = %f\n',alpha);
fprintf('mu = %f\n',u);
fprintf('l1 = %f\n',l1);
fprintf('\nDid we meet the minimum mu requirement\n');
fprintf('mu_LB = %f\n',uLB);
fprintf('alpha_LB = %f\n',alpha_LB);


% plot normal curves
% p = linspace(0,1,256);
% y0 = 1/(2*pi*s0^2)*exp(-(p-c0).^2/(2*s0^2));
% y1 = 1/(2*pi*s1^2)*exp(-(p-c1).^2/(2*s1^2));
% a = s0^2-s1^2;
% b = -2*c1*s0^2 + 2*c0*s1^2;
% c = c1^2*s0^2 - c0^2*s1^2 - 2*s0^2*s1^2*log(s0/s1);
% delta = b^2 - 4*a*c
% y_1 = (-b + sqrt(b^2-4*a*c))/(2*a)
% y_2 = (-b - sqrt(b^2-4*a*c))/(2*a)
% figure
% plot(p,y0,'-b', p,y1,'-r');
% xlabel('Intensity Value');
% ylabel('Probability');

% plot histogram
i = imread('2gray.jpg');
i = i(:,:,1);
histI = histogram(i);