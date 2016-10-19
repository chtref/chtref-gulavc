function [ img_out ] = Filtre_Laplacien( img_in )
%Filtre_Laplacien Filtre_Laplacien

mask_l = [-1 -1 -1;
          -1 8 -1;
          -1 -1 -1];

img_out = conv2(double(img_in),double(mask_l),'same');

end

