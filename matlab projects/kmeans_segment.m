clc
clear
close all

he = imread('original_uneven.jpg');
%figure, imshow(he), title('Original');

ab = double(he(:,:,1));
nrows = size(ab,1);
ncols = size(ab,2);
ab = reshape(ab,nrows*ncols,1);

nColours = 2;
[cluster_idx, cluster_center] = kmeans(ab,nColours,'distance','sqEuclidean','Replicates',3);

pixel_labels = reshape(cluster_idx,nrows,ncols);
figure, imshow(pixel_labels,[]), title('Normal');

un = imread('uneven.jpg');
%figure, imshow(un), title('Uneven Illumination');

unab = double(un(:,:,1));
unab = reshape(unab,nrows*ncols,1);
[cluster_idx2, cluster_center2] = kmeans(unab,nColours,'distance','sqEuclidean','Replicates',3);
pixel_labels2 = reshape(cluster_idx2,nrows,ncols);
figure, imshow(pixel_labels2,[]), title('Uneven');

% detecting edges
edge_norm = edge(pixel_labels,'canny');
edge_uneven = edge(pixel_labels2,'canny');
figure, imshow(edge_norm,[]), title('Normal edges');
figure, imshow(edge_uneven,[]), title('Uneven edges');