close all
clear all
clc

[pathname,dirname] = uigetfile('*');
f = fullfile(dirname,pathname);
%f = double(rgb2gray(imread(f)));
g = imread(f);
fR = double(g(:,:,1));
fG = double(g(:,:,2));
fB = double(g(:,:,3));
N = size(fR); N = N(1);

%--------------------------------------------------------------
% TV Anisotropic Denoising
gamma = input('What is the value for gamma? ');

% Red channel
fprintf('Red Channel\n');
fR_denoise_atv = SB_ATV(fR,gamma);
fprintf('ATV Rel.Err = %g\n',norm(fR_denoise_atv(:) - fR(:)) / norm(fR(:)));
fR = uint8(reshape(fR_denoise_atv,N,N));
%filepath = strcat(dirname,'tvR.jpg');
%imwrite(fR,filepath);
%[f,path] = uiputfile('tv.jpg','Save file as');

% Green channel
fprintf('Green Channel\n');
fG_denoise_atv = SB_ATV(fG,gamma);
fprintf('ATV Rel.Err = %g\n',norm(fG_denoise_atv(:) - fG(:)) / norm(fG(:)));
fG = uint8(reshape(fG_denoise_atv,N,N));
%filepath = strcat(dirname,'tvG.jpg');
%imwrite(fG,filepath);
%[f,path] = uiputfile('tv.jpg','Save file as');

% Blue channel
fprintf('Blue Channel\n');
fB_denoise_atv = SB_ATV(fB,gamma);
fprintf('ATV Rel.Err = %g\n',norm(fB_denoise_atv(:) - fB(:)) / norm(fB(:)));
fB = uint8(reshape(fB_denoise_atv,N,N));
%filepath = strcat(dirname,'tvB.jpg');
%imwrite(fB,filepath);
%[f,path] = uiputfile('tv.jpg','Save file as');

fTV = cat(3,fR,fG,fB);
filepath = strcat(dirname,'tv.jpg');
imwrite(fTV,filepath);
%[fTV,path] = uiputfile('tv.jpg','Save file as');