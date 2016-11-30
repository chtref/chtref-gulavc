function [ imbin ] = Filtre_Canny( img, gauss, th )
%Filtre_Canny Exercice 2.2

tb = 0.4 * th; %Pourquoi? Parce que.

img_gauss = imfilter(img,gauss);

sobel_x = [-1, -2, -1;
            0, 0, 0;
            1, 2, 1];
sobel_y = [-1, 0, 1;
            -2, 0, 2;
            -1, 0, 1];

g_x = convn(img_gauss,sobel_x,'same');
g_y = convn(img_gauss,sobel_y,'same');


grad_module = sqrt(g_x.^2 + g_y.^2);
grad_angle = rad2deg(atan2(g_y,g_x));

grad_angle_cat = zeros(size(grad_angle,1),size(grad_angle,2),2);

for i = 1:size(grad_angle,1)
   for j = 1:size(grad_angle,2)
       e = grad_angle(i,j);
      if (e > -22.5 && e <= 22.5) || (e > 157.5 || e <= -157.5)
              grad_angle_cat(i,j,1) = 1;
              grad_angle_cat(i,j,2) = 0;
      elseif (e > 22.5 && e <= 67.5) || (e > -157.5 && e <= -112.5)
              grad_angle_cat(i,j,1) = 1;
              grad_angle_cat(i,j,2) = 1;
      elseif (e > 67.5 && e <= 112.5) || (e > -112.5 && e <= -67.5)
              grad_angle_cat(i,j,1) = 0;
              grad_angle_cat(i,j,2) = 1;
      elseif (e > 112.5 && e <= 157.5) || (e > -67.5 && e <= -22.5)
              grad_angle_cat(i,j,1) = 1;
              grad_angle_cat(i,j,2) = -1;
      end       
   end    
end

grad_module_padded = padarray(grad_module,[1 1]);
grad_angle_cat = padarray(grad_angle_cat,[1 1]);

for i = 2:size(grad_module,1)+1
   for j = 2:size(grad_module,2)+1
       
       grad = grad_module_padded(i,j);
       grad_plus1 = grad_module_padded(i + grad_angle_cat(i,j,1), j + grad_angle_cat(i,j,2));
       grad_minus1 = grad_module_padded(i - grad_angle_cat(i,j,1), j - grad_angle_cat(i,j,2));
       
       if not(grad >= grad_minus1 && grad >= grad_plus1)
           grad_module_padded(i,j) = 0;
       end

   end
end

img_bin = zeros(size(grad_module_padded));
for i = 2:size(grad_module,1)+1
   for j = 2:size(grad_module,2)+1
       if grad_module_padded(i,j) >= th
           img_bin(i,j) = 255;
       else
           img_bin(i,j) = 0;
       end
   end
end


imlf_finale = zeros(size(grad_module_padded));
for i = 2:size(grad_module,1)+1
   for j = 2:size(grad_module,2)+1
       pixel_plus1 = img_bin(i + grad_angle_cat(i,j,1), j + grad_angle_cat(i,j,2));
       pixel_minus1 = img_bin(i - grad_angle_cat(i,j,1), j - grad_angle_cat(i,j,2));
       
       if (pixel_plus1 >= tb || pixel_minus1 >= tb)
           imlf_finale(i,j) = 255;
       else
           imlf_finale(i,j) = 0;
       end
       
   end
end

imbin = img_bin(2:end-1,2:end-1);
end

