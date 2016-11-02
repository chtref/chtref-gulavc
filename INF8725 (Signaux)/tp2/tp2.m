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

%%
%Exercice 2.2

img_bw = Binariser(img,250);

img_bw_inv = 255 - img_bw;

figure(6)
imshow(img_bw_inv);

%%
%Exercice 2.3

se = strel('disk',1);
img_close = imclose(img_bw_inv,se);

figure(7)
imshow(img_close);

%%
%Exercice 2.4
% On pr�sume que les pi�ces sont les suivantes, de gauche � droite et de
% haut en bas:
% 5� 5� 10� 10�
% 25�   1$  10�
% 5� 1$     2$


[value,count] = Compter_Monnaie(img_close);

disp('Valeur mon�taire: ')
disp(value)

%%
%Partie 2
%Exercice 3

%%
%Exercice 3.1

img_h = imread('Barres_Horizontales.png');
img_v = imread('Barres_Verticales.png');
img_o = imread('Barres_Obliques.png');

figure(8)
subplot(221)
imshow(img_h)
subplot(222)
imshow(img_v)
subplot(223)
imshow(img_o)

%%
%Exercice 3.2

fft2_h = fft2(img_h,size(img_h,1),size(img_h,2));
fft2_h = abs(fft2_h/sum(sum(fft2_h)));

fft2shift_h = fftshift(fft2_h);
fft2shift_h = 1 + log(fft2shift_h);

figure(9)
subplot(221)
imshow(fft2shift_h,[])

%
fft2_v = fft2(img_v,size(img_v,1),size(img_v,2));
fft2_v = abs(fft2_v/sum(sum(fft2_v)));

fft2shift_v = fftshift(fft2_v);
fft2shift_v = 1 + log(fft2shift_v);

subplot(222)
imshow(fft2shift_v,[])

%
fft2_o = fft2(img_o,size(img_o,1),size(img_o,2));
fft2_o = abs(fft2_o/sum(sum(fft2_o)));

fft2shift_o = fftshift(fft2_o);
fft2shift_o = 1 + log(fft2shift_o);

subplot(223)
imshow(fft2shift_o,[])


%%
%Exercice 3.3

img_v_rotate = imrotate(img_v,70,'bilinear','crop');

figure(8)
subplot(224)
imshow(img_v_rotate)

fft2_v_rot = fft2(img_v_rotate,size(img_v_rotate,1),size(img_v_rotate,2));
fft2_v_rot = abs(fft2_v_rot/sum(sum(fft2_v_rot)));

fft2shift_v_r = fftshift(fft2_v_rot);
fft2shift_v_r = 1 + log(fft2shift_v_r);

figure(9)
subplot(224)
imshow(fft2shift_v_r,[])

%%
%Exercice 3.4

%

%%
%Exercice 4

%%
%Exercice 4.1