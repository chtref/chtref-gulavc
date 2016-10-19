%%
%Exercice 1
%%
%Exercice 1.1

img = imread('img1.jpg');
figure(1)
img_eq = uint8(Egalisation_Histogramme(img));
imshow(img_eq);

%%
%Exercice 1.2
mask_gaussien = (1/90) * [1 2 1 2 1;
                          2 4 8 4 2;
                          1 8 18 8 1;
                          2 4 8 4 2;
                          1 2 1 2 1];
                      
img_gauss = uint8(Filtre_Gaussien(img_eq,mask_gaussien));
figure(2)
imshow(img_gauss);

%%
%Exercice 1.3
img_lap = Filtre_Laplacien(img_gauss);

img_lap = img_lap - min(min(img_lap));
img_lap = uint8(255 * img_lap/max(max(img_lap)));

figure(3)
imshow(img_lap);


%%
%Exercice 1.4
mask_g = (1/16) * [1 2 1;
                   2 4 2;
                   1 2 1];
               
img_cont = Rehaussement_Contour(img_gauss,mask_g,1.2);

img_cont = img_cont - min(min(img_cont));
img_cont = uint8(255 * img_cont/max(max(img_cont)));

figure(4)
imshow(img_cont);

%%
%Exercice 1.5

%On observe une diminution de contraste dans la derni�re image. Par contre,
%on observe aussi que l'image a moins de "bruit" que dans l'image
%initiale, car les filtres gaussiens et laplaciens se sont charg�s de
%filtrer l'image.

%%
%Exercice 2
%%
%Exercice 2.1

img = imread('monnaie.png');
figure(5)
imshow(img);





