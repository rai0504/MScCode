clc
clear
close all

image_set = ['188  ';'195  ';'228  ';'1057 ';'1265 ';'10093';'10102';'10104';'12294';'12627';'13432';'13438';'13899';...
    '13901';'21749';'21759';'32140';'35278';'37338';'37339';'38974';'40217';'40968';'41066';'42451'];
image_set = cellstr(image_set);
c0 = 0.079524;
c1 = 0.153956;
imageNumber = 1;
groundtruth = rgb2gray(imread(strcat('\test_set_ground_truth\',char(image_set(imageNumber)),'gt.png')));
%figure, imshow(groundtruth);

image = rgb2gray(imread(strcat('\test_set\',char(image_set(imageNumber)),'gray.jpg')));
%figure, imshow(image);

% groundtruth = reshape(groundtruth,1,[]);
% image = reshape(image,1,[]);
fg=[];
bg=[];
for r=1:256
    for c=1:256
        if (groundtruth(r,c)>=127)
            fg = [fg image(r,c)];
        else
            bg = [bg image(r,c)];
        end
    end
end
fprintf('c0 = %f\nc1 = %f\ndiff = %f\n',mean(bg)/255,mean(fg)/255,abs(c0-mean(bg)/255+abs(c1-mean(fg)/255)));

