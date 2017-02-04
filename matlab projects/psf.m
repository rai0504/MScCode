clc
clear
close all

%I = checkerboard(8);
I = imread('psf_original.jpg');
I = rgb2gray(I);
PSF = fspecial('gaussian',7,10);
V = .0001;
BlurredNoisy = imnoise(imfilter(I,PSF),'gaussian',0,V);
NOISEPOWER = V*prod(size(I));
[J LAGRA] = deconvreg(BlurredNoisy,PSF,NOISEPOWER);

PSFtemp = fspecial('gaussian',9,10);
V = .001;
Blurredtemp = imfilter(I,PSFtemp);
Noisytemp = imnoise(Blurredtemp,'gaussian',0,V);


maxPSF = max(max(PSF));
PSF = fspecial('gaussian',128,10);
PSFdisp = 255/maxPSF.*PSF;

OTF  = psf2otf(PSF,[128 128]); % PSF --> OTF
PSF2 = otf2psf(OTF,size(PSF)); % OTF --> PSF2
maxPSF2 = max(max(PSF2));
PSFdisp2 = 255/maxPSF2.*PSF2;

figure, imshow(I), title('Original');
figure, imshow(PSFdisp), title('PSF');
figure, imshow(PSFdisp2), title('PSF2');
figure, imshow(Blurredtemp), title('Blurred');
figure, imshow(Noisytemp), title('Noisy');
figure, imshow(J), title('Deconvolved');
%figure, imshow(deconvreg(BlurredNoisy,PSF,[],LAGRA/10)), title('deconvreg(A,PSF,[],0.1*LAGRA)');
%figure, imshow(deconvreg(BlurredNoisy,PSF,[],LAGRA*10)), title('deconvreg(A,PSF,[],10*LAGRA)');