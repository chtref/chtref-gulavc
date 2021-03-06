type Binariser.m
type Compter_Monnaie.m
type Egalisation_Histogramme.m
type Filtre_Gaussien.m
type Filtre_Laplacien.m
type Rehaussement_Contour.m

%%
%Exercice 1
%%
%Exercice 1.1

img = imread('img1.jpg');
figure(1)
img_eq = uint8(Egalisation_Histogramme(img));
imshow(img_eq);
title('Figure 1: Image �galis�e')

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
title('Figure 2: Image apr�s filtre Gaussien')

%%
%Exercice 1.3
img_lap = Filtre_Laplacien(img_gauss);

img_lap = img_lap - min(min(img_lap));
img_lap = uint8(255 * img_lap/max(max(img_lap)));

figure(3)
imshow(img_lap);
title('Figure 3: Image apr�s filtre Laplacien')


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
title('Figure 4: Image apr�s rehaussement de contour')


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
title('Figure 5: Image originale')

%%
%Exercice 2.2

img_bw = Binariser(img,250);

img_bw_inv = 255 - img_bw;

figure(6)
imshow(img_bw_inv);
title('Figure 6: Image binaris�e')

%%
%Exercice 2.3

se = strel('disk',1);
img_close = imclose(img_bw_inv,se);

figure(7)
imshow(img_close);
title('Figure 7: Image apr�s fermeture')

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
title('Figure 8.1: Image originale (Barres Horizontales)')
subplot(222)
imshow(img_v)
title('Figure 8.2: Image originale (Barres Verticales)')
subplot(223)
imshow(img_o)
title('Figure 8.3: Image originale (Barres Obliques)')

%%
%Exercice 3.2

fft2_h = fft2(img_h,size(img_h,1),size(img_h,2));
fft2_h = abs(fft2_h/numel(fft2_h));

fft2shift_h = fftshift(fft2_h);
fft2shift_h = log(1 + fft2shift_h);

figure(9)
subplot(221)
imshow(fft2shift_h,[])
title('Figure 9.1: Spectre (Barres Horizontales)')

%
fft2_v = fft2(img_v,size(img_v,1),size(img_v,2));
fft2_v = abs(fft2_v/numel(fft2_v));

fft2shift_v = fftshift(fft2_v);
fft2shift_v = log(1 + fft2shift_v);

subplot(222)
imshow(fft2shift_v,[])
title('Figure 9.2: Spectre (Barres Verticales)')

%
fft2_o = fft2(img_o,size(img_o,1),size(img_o,2));
fft2_o = abs(fft2_o/numel(fft2_o));

fft2shift_o = fftshift(fft2_o);
fft2shift_o = log(1 + fft2shift_o);

subplot(223)
imshow(fft2shift_o,[])
title('Figure 9.3: Spectre (Barres Obliques)')

%%
%Exercice 3.3

img_v_rotate = imrotate(img_v,70,'bilinear','crop');

figure(8)
subplot(224)
imshow(img_v_rotate)
title('Figure 8.4: Image Oblique Tourn�e')

fft2_v_rot = fft2(img_v_rotate,size(img_v_rotate,1),size(img_v_rotate,2));
fft2_v_rot = abs(fft2_v_rot/numel(fft2_v_rot));

fft2shift_v_r = fftshift(fft2_v_rot);
fft2shift_v_r = log(1 + fft2shift_v_r);

figure(9)
subplot(224)
imshow(fft2shift_v_r,[])
title('Figure 9.4: Spectre Oblique Tourn�')

%%
%Exercice 3.4

% On peut observer que l'orientation des fr�quences dans le spectre
% correspond � l'orientation des fr�quences dans l'image. En effet, en
% analysant la figure 1 qui est clairement en vertical (les changements
% occurent uniquement de haut vers le bas, et non de gauche � droite), donc
% on obtient un spectre uniquement vertical et centr�.

%%
%Exercice 4

%%
%Exercice 4.1

img_house = imread('NewHouse.png');
img_house_gray = rgb2gray(img_house);
figure(10)
subplot(121)
imshow(img_house_gray,[])
title('Figure 10.1: Image en nuances de gris')

fft2_house = fft2(img_house_gray);
fft2_house = fftshift(fft2_house/numel(fft2_house));


subplot(122)
imshow(abs(fft2_house))
title('Figure 10.2: Spectre de cette image')

%%
%Exercice 4.2

schema = imread('schema_colored.png');
figure(11)
imshow(schema)
title('Figure 11: Schema du spectre identifi�')

% Les diff�rentes couleurs du sch�ma repr�sentes les sections suivantes de
% la maison:
% Red: Door
% Orange: Chimney
% Yellow: Left Window
% Purple: Right Window
% Green: Wall
% Blue: Roof



%%
%Exercice 4.3

H = fspecial('gaussian', size(img_house_gray), 12);
filtre_toit = imfilter(img_house_gray,H);

figure(12)
imshow(filtre_toit)
title('Figure 12: Texture de toit r�v�l�e par filtre Gaussien')


%%
%Exercice 4.4

ideal_filter = double(rgb2gray(imread('ideal_filter.png')));
ideal_filter = ideal_filter/sum(sum(ideal_filter));
ideal_filter = imresize(ideal_filter,[825 936]);

filtre_windows = fft2_house .* ideal_filter;

filtered_img = ifftshift(filtre_windows);
filtered_img = ifft2(filtered_img);
filtered_img = Binariser(filtered_img,0.0000001);
%ifft2 donne des valeurs complexes tres petites donc la valeur utilis�e
%pour le seuillage est aussi tres petite
figure(13)
imshow(abs(filtered_img),[])
title('Figure 13: Texture de fen�tre r�v�l�e par filtre id�al')


%%
%Exercice 4.5
door_filter = double(rgb2gray(imread('filtre_door.png')));
door_filter = door_filter/sum(sum(door_filter));
door_filter = imresize(door_filter,[825 936]);

filtre_door = fft2_house .* door_filter;

filtered_img = ifftshift(filtre_door);
filtered_img = ifft2(filtered_img);
filtered_img = Binariser(filtered_img,0.0000001);
%ifft2 donne des valeurs complexes tres petites donc la valeur utilis�e
%pour le seuillage est aussi tres petite
figure(14)
imshow(abs(filtered_img),[])
title('Figure 14: Texture de porte r�v�l�e par filtre id�al')

%%
%exercice 4.6

% Ne pas utiliser la fr�quence 0 permet d'�liminer la couleur moyenne dans 
% l'image. Par exemple, dans l'image de la maison, lorsque
% nous utilisons la fr�quence centrale, la quasi-totalit� de l'image apr�s
% filtrage est blanche, vs ne aps l'utliser qui donne une image presque
% enti�rement noire sauf la fr�quence voulue.

%%
%Exercice 4.7

% Le fitre butterworth permet d'�liminer les artefects d'ondulation dues au
% filtre id�al. Toutefois, plus l'ordre du butterworth augmente, plus on se
% rapproche du filtre id�al et donc les ondulations r�apparaissent.

%%
%Exercice 4.8

% Highpass, car on ajoute toujours un �l�ment � chaque it�ration. Ceci pour
% nous permet de conclure qu'il s'agit soit d'un highpass ou d'un lowpass.
% Par la suite, puisque la couleur moyenne de l'image (repr�sent�e par le
% point central du spectre) apparait en dernier, on peut conclure qu'il
% s'agit d'un highpass puisque la fr�quence 0 apparait en dernier.
