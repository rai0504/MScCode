% Clean up
clc;
clear all;
% Read in standard MATLAB demo image.
originalImage = imread('poisson_original.jpg');
% Convert to double.
originalImage1 = double(originalImage(:,:,1));
originalImage2 = double(originalImage(:,:,2));
originalImage3 = double(originalImage(:,:,3));
%originalImage = double(originalImage(:,:,1));
%subplot(1,3,1);
%imshow(originalImage, []);
max_value1 = max(max(originalImage1)); % take the maximum value
max_value2 = max(max(originalImage2)); % take the maximum value
max_value3 = max(max(originalImage3)); % take the maximum value
normalizedImage1 = originalImage1/max_value1; % normalize, now pixel values range from [0,1] (I know that there's no negative values in my image.
normalizedImage2 = originalImage2/max_value2; % normalize, now pixel values range from [0,1] (I know that there's no negative values in my image.
normalizedImage3 = originalImage3/max_value3; % normalize, now pixel values range from [0,1] (I know that there's no negative values in my image.
% The Poisson option of imnoise wants the values to be scaled by 1e-12,
% so let's have the values go from 0 to 10e-12:
normalizedImage1 = normalizedImage1 * 10e-12;
normalizedImage2 = normalizedImage2 * 10e-12;
normalizedImage3 = normalizedImage3 * 10e-12;
% Now we'd consider the image as a Poisson process with values of 0-10.
% Now plot.
%subplot(1,3,2);
%imshow(normalizedImage, []);
scale = 2e-9;
noisyImage1 = max_value1 * imnoise(normalizedImage1, 'poisson')/scale; % generate noisy image and scale back to the original intensities.
noisyImage2 = max_value2 * imnoise(normalizedImage2, 'poisson')/scale; % generate noisy image and scale back to the original intensities.
noisyImage3 = max_value3 * imnoise(normalizedImage3, 'poisson')/scale; % generate noisy image and scale back to the original intensities.
%subplot(1,3,3);
%imshow(noisyImage, []);
figure, imshow(originalImage, []), title('Original');
%figure, imshow(originalImage1, []), title('orig 1');
%figure, imshow(originalImage2, []), title('orig 2');
%figure, imshow(originalImage3, []), title('orig 3');
figure, imshow(noisyImage1,[]), title('noise 1');
figure, imshow(noisyImage2,[]), title('noise 2');
figure, imshow(noisyImage3,[]), title('noise 3');
newImage = double(originalImage);
newImage(:,:,1) = noisyImage1;
newImage(:,:,2) = noisyImage2;
newImage(:,:,3) = noisyImage3;

max_valuen1 = max(max(noisyImage1));
max_valuen2 = max(max(noisyImage2));
max_valuen3 = max(max(noisyImage3));

figure, imshow(newImage,[]), title('new image');
imwrite(noisyImage1,'noisy1.png');
imwrite(noisyImage2,'noisy2.png');
imwrite(noisyImage3,'noisy3.png');