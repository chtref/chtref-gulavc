% Exercice 1
%%

% Exercice 1.1

img_ford = imread('Formula_Ford.png');

figure(1)
imshow(img_ford)
title('Exercice 1.1: image originale')

% Exercice 1.2

angle = rad2deg(atan2(16,31));
juste_h = fspecial('motion', 30, angle);


% Exercice 1.3
img_panic = deconvwnr(img_ford,juste_h,0);
figure(2)
imshow(img_panic)
title('Exercice 1.3: image d�convolu�e avec K=0')

% L'image est compl�tement ruin�e � cause du bruit. Il est extremement
% difficile de trouver des morceaux de l'image originale dans ce mer de
% bruit.

% Exercice 1.4
meilleur_k = 0.001 / sqrt(var(im2double(img_ford(:))));

% Exercice 1.5
img_ok = deconvwnr(img_ford, juste_h, meilleur_k);
figure(3)
imshow(img_ok)
title('Exercice 1.3: image d�convolu�e avec le meilleur K')


%%
% Exercice 2
%%

% Exercice 2.1

img_stairs = imread('escaliers.jpg');

figure(4)
imshow(img_stairs)
title('Exercice 2.1: image originale')

% Exercice 2.2

th = 128;
gauss = fspecial('gaussian');
img_f = Filtre_Canny(img_stairs,gauss,th);


figure(5)
imshow(img_f,[])
title('do we panic?')