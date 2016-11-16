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
title('Exercice 1.3: image déconvoluée avec K=0')

% L'image est complètement ruinée à cause du bruit. Il est extremement
% difficile de trouver des morceaux de l'image originale dans ce mer de
% bruit.

% Exercice 1.4

% TODO: K représente le ratio du bruit et du signal (NSR) donc K ~= 0.001

meilleur_k = 0.001;

% Exercice 1.5
img_ok = deconvwnr(img_ford, juste_h, meilleur_k);
figure(3)
imshow(img_ok)
title('Exercice 1.3: image déconvoluée avec le meilleur K')


%%
% Exercice 2

%%

% Exercice 2.1