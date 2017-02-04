clc
clear
close all

% raw image
orig=0;
imagenumber = 4;
if orig
    A = rgb2gray(imread(strcat(num2str(imagenumber),'gray.jpg')));    
else
    A = rgb2gray(imread(strcat(num2str(imagenumber),'diffused.jpg')));
end
% mask
mrad = 0;
if (mrad==0) % no contour region    
    M = rgb2gray(imread(strcat(num2str(imagenumber),'gt.jpg'))); 
elseif (mrad==1) % calculate otsu mask
    Motsu_level = graythresh(A);
    M = uint8(im2bw(A,Motsu_level))*255;    
elseif (mrad==5) % radius=5 around contour region
    M = rgb2gray(imread(strcat(num2str(imagenumber),'gt_5radius.jpg')));
end
%imshow(M);

%-------------------------------------------------------------------------
% averaging kernels
% 0 - Gaussian      1 - Average     2 - Disk
% 3 - Laplacian     4 - LoG         5 - Prewitt
% 6 - Sobel
%-------------------------------------------------------------------------
filterType = 4;
if (filterType==0) % gaussian
    sigma = 25;
    B = imgaussfilt(A,sigma);
elseif (filterType==1) % average
    n = 3;
    h = fspecial('average', [n n]);
    B = imfilter(A, h);
elseif (filterType==2) % disk
    r = 3;
    h = fspecial('disk', r);
    B = imfilter(A, h);
elseif (filterType==3) % laplacian
    alpha = 0.2;
    h = fspecial('laplacian',alpha);
    B = imfilter(A, h);
elseif (filterType==4) % laplacian of gaussian
    n = 11;
    sigma = 0.4;
    h = fspecial('log', [n n], sigma);
    B = imfilter(A, h);
elseif (filterType==5) % prewitt
    h = fspecial('prewitt');
    B = imfilter(A, h);
else % sobel
    h = fspecial('sobel');
    B = imfilter(A, h);
end

%figure
%plot(A(:)', B(:)', '.')
%xlabel('Pixel Intensity');
%ylabel('Average Neighbourhood Intensity');
%title('Intensity vs Neighbourhood');


[m, n] = size(A);
Abg = []; Afg = []; 
Bbg = []; Bfg = []; 
Acont = []; Bcont = [];
for r = 1:m
    for c = 1:n
        if (M(r,c)<=50)
            Abg = [Abg A(r,c)];
            Bbg = [Bbg B(r,c)];
        elseif (M(r,c)>=200)
            Afg = [Afg A(r,c)];
            Bfg = [Bfg B(r,c)];
        else
            Acont = [Acont A(r,c)];
            Bcont = [Bcont B(r,c)];
        end
    end
end
figure
hold on
plot(Afg, Bfg, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'r');
plot(Abg, Bbg, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'b');
plot(Acont, Bcont, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'g');
hold off
xlabel('Pixel Intensity');
ylabel('Average Neighbourhood Intensity');
title('Intensity vs Neighbourhood');
l=legend('Object','Background','Grey Region');
set(l,'Position', [0.65 0.15 0.2 0.1]);

%----------------------------------------------------------------------
% LBP
%rr = generateRadialFilterLBP(8, 1)
lbpA = pixelwiseLBP(A,'filtR', generateRadialFilterLBP(8, 1), 'isRotInv', true,...
        'isChanWiseRot', false);
%radfilter = [1 1 1; 1 -8 1; 1 1 1];
%lbpA = pixelwiseLBP(A,'filtR', radfilter, 'isRotInv', true,...
%        'isChanWiseRot', false);
%figure;
%imshow(lbpA);

lbpAbg = []; lbpAfg = []; 
lbpBbg = []; lbpBfg = []; 
lbpAcont = []; lbpBcont = [];
for r = 1:m
    for c = 1:n
        if (M(r,c)<=50)
            lbpAbg = [lbpAbg A(r,c)];
            lbpBbg = [lbpBbg lbpA(r,c)];
        elseif (M(r,c)>=200)
            lbpAfg = [lbpAfg A(r,c)];
            lbpBfg = [lbpBfg lbpA(r,c)];
        else
            lbpAcont = [lbpAcont A(r,c)];
            lbpBcont = [lbpBcont lbpA(r,c)];
        end
    end
end
figure
hold on
plot(lbpAfg, lbpBfg, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'r');
plot(lbpAbg, lbpBbg, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'b');
plot(lbpAcont, lbpBcont, '.', 'MarkerSize', 1, 'MarkerEdgeColor', 'g');
hold off
xlabel('Pixel Intensity');
ylabel('Local Binary Pattern');
title('Intensity vs LBP Texture');
l=legend('Object','Background','Grey Region');
set(l,'Position', [0.65 0.15 0.2 0.1]);

%----------------------------------------------------------------------
% Histogram
figure
histA = histogram(A);
title('Histrogram of Intensity');
figure
histlbpA = histogram(lbpA);
title('Histrogram of Texture');
