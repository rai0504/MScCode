clc
clear
close all

% raw image
orig=0;
imagenumber = 3;
if orig
    A = rgb2gray(imread(strcat(num2str(imagenumber),'gray.jpg')));    
else
    A = rgb2gray(imread(strcat(num2str(imagenumber),'diffused.jpg')));
end
%imshow(A);
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


% make longest side 256 pixels long, if longer than 256 pixels
maxlength = max(size(A));
if (maxlength>256)
    A=imresize(A,256/maxlength);
end
[rA, cA] = size(A);
A = im2double(A);
%---------------------------------------------------------------------
% Calculate Energy Map
calcGLCM = 0;
fprintf('Calculating...');
if calcGLCM
    energy = im2double(zeros(rA,cA)); % energy map [0,1]
    homogeneity = im2double(zeros(rA,cA)); % homogeneity map [0,1]
    contrast = im2double(zeros(rA,cA)); % contrast map [0, (size(GLCM,1)-1)^2]
    correlation = im2double(zeros(rA,cA)); % correlation map [-1 1] NaN if constant image
else
    invTexDiff = im2double(zeros(rA,cA)); % energy map [0,1]
end
nbhoodsize = 3;
padA = im2double(padarray(A,[(nbhoodsize-1)/2 (nbhoodsize-1)/2]));
offset = double((nbhoodsize-1)/2);
for r = 1+offset:rA-offset
    for c = 1+offset:cA-offset
        workingSubImage = padA(r-offset:r+offset, c-offset:c+offset);
         e = 0;                 
        if calcGLCM 
            glcm = graycomatrix(workingSubImage);
            e = graycoprops(glcm,{'contrast','correlation','energy','homogeneity'});
            energy(r,c) = e.Energy;
            homogeneity(r,c) = e.Homogeneity;
            contrast(r,c) = e.Contrast;
        else
            for err = 1:nbhoodsize
                for ecc = 1:nbhoodsize
                    if  (err==offset+1) && (ecc==offset+1) 
                        continue; 
                    end
                    ssq = (workingSubImage(err,ecc)-1*workingSubImage(offset+1,offset+1))^1;
                    e = e + ssq;
                end
             end % sum of squares
             invTexDiff(r-offset,c-offset) = e;
        end
    end
end
%-------------------------------------------------------------------------
figure
if calcGLCM
    MaxContrast = max(contrast(:));
    contrast = contrast/MaxContrast;

    subplot(3, 3, 1); imshow(A); title('Original');
    subplot(3, 3, 2); imshow(energy);  title('Energy');
    subplot(3, 3, 3); imshow(abs(A-energy)); title('Difference');

    subplot(3, 3, 4); imshow(A); title('Original');
    subplot(3, 3, 5); imshow(homogeneity);  title('Homogeneity');
    subplot(3, 3, 6); imshow(abs(A-homogeneity)); title('Difference');

    subplot(3, 3, 7); imshow(A); title('Original');
    subplot(3, 3, 8); imshow(contrast);  title('Contrast');
    subplot(3, 3, 9); imshow(abs(A-contrast)); title('Difference');
else
    MaxVal = max(invTexDiff(:));
    invTexDiff = invTexDiff/MaxVal;
    subplot(1, 2, 1); imshow(A); title('Original');
    subplot(1, 2, 2); imshow(invTexDiff); title('Neighbourhood Energy');
end


fprintf('Done\n');