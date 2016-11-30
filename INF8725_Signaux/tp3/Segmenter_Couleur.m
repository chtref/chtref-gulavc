function [ img_seg ] = Segmenter_Couleur( img, lut_r, lut_g, lut_b )
%SEGMENTER_COULEUR Segmenter_Couleur
%   Exercice 3.3

img_seg = zeros(size(img,1),size(img,2));

for i = 1:size(img,1)
    for j = 1:size(img,2)
        
        img_seg(i,j,1) = Apply_LUT(img(i,j,1),lut_r);
        img_seg(i,j,2) = Apply_LUT(img(i,j,2),lut_g);
        img_seg(i,j,3) = Apply_LUT(img(i,j,3),lut_b);
        
    end
end

img_seg = uint8(img_seg);

end

