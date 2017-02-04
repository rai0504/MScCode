close all
clear all
clc

uiopen('*');
%f = double(imread('split\41066B.jpg'));
f = f(:,:,1);
N = size(f); N = N(1);

%--------------------------------------------------------------
% TV Anisotropic Denoising
mu = 5;
lambda = 1;
tol = 1e-3;
f_denoise_atv = SB_ATV(f,mu,lambda,tol);
fprintf('ATV Rel.Err = %g\n',norm(f_denoise_atv(:) - f(:)) / norm(f(:)));
f = uint8(reshape(f_denoise_atv,N,N));
%figure; 
%colormap gray; 
%imagesc(f);
imwrite(f,'splittv\41066B.jpg');