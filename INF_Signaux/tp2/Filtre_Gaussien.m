function [ img_out ] = Filtre_Gaussien( img_in, mask )
%Filtre_Gaussien Filtre_Gaussien

img_out = conv2(double(img_in),double(mask),'same');

end

