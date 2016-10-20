function [ img_out ] = Binariser( img_in, threshold )
%Binariser Binariser

img_out = zeros(size(img_in));

for i = 1:size(img_in,1)
    for j = 1:size(img_in,2)
        if(img_in(i,j) < threshold)
            img_out(i,j) = 0;
        else
            img_out(i,j) = 255;
        end
    end
end

end

