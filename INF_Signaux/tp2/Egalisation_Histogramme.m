function [ img_out ] = Egalisation_Histogramme( img_in )
%Egalisation_Histogramme Applique un filtre d'�galisation d'histogramme �
%une image

img_out = zeros(size(img_in));

f = imhist(img_in);
fn = f/sum(f); %Histogramme normalis�

for i = 1:size(img_in,1)
    for j = 1:size(img_in,2)
        img_out(i,j) = 255*T_transform(img_in(i,j),fn);
    end
end

end

function [ value_out ] = T_transform(value_in, hist_n)

value_out = sum(hist_n(1:value_in+1));

end
