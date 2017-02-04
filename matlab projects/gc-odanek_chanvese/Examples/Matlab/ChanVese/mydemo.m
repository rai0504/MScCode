clc
clear
close all

% raw image
orig=0;
imagenumber = 3;
if orig
    a = rgb2gray(imread(strcat(num2str(imagenumber),'gray.jpg')));    
else
    a = rgb2gray(imread(strcat(num2str(imagenumber),'diffused.jpg')));
end
%a = imread('somefile.tif');
%an = gc_normalize_image(single(a));
an = im2double(a);
[seg energy iter c1 c2] = gc_chan_vese(an, 1, 10, 10);

imshow(seg);