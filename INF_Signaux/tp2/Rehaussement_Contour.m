function [ img_out ] = Rehaussement_Contour( img_in, mask_g, k )
%Rehaussement_Contour Rehaussement_Contour

mask_l = [-1 -1 -1;
          -1 8 -1;
          -1 -1 -1];

img_g = conv2(double(img_in),double(mask_g),'same');
img_out = img_g + k * conv2(double(img_g),double(mask_l),'same');

end

