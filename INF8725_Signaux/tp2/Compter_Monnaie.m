function [ money_value, money_count ] = Compter_Monnaie( img_in )
%Compter_Monnaie Compter_Monnaie

line = strel('disk',5);
im2 = imerode(img_in,line,'same');
initial_count = max(max(bwlabel(im2))); %nb de pieces dans l'image

line = strel('disk',41);
im2 = imerode(im2,line,'same');
remaining_count = max(max(bwlabel(im2))); %nb de pieces restantes dans l'image
count_10c = initial_count - remaining_count;
current_count = remaining_count;

line = strel('disk',9);
im2 = imerode(im2,line,'same');
remaining_count = max(max(bwlabel(im2))); %nb de pieces restantes dans l'image
count_5c = current_count - remaining_count;
current_count = remaining_count;

line = strel('disk',5);
im2 = imerode(im2,line,'same');
remaining_count = max(max(bwlabel(im2))); %nb de pieces restantes dans l'image
count_25c = current_count - remaining_count;
current_count = remaining_count;

line = strel('disk',7);
im2 = imerode(im2,line,'same');
remaining_count = max(max(bwlabel(im2))); %nb de pieces restantes dans l'image
count_1d = current_count - remaining_count;
current_count = remaining_count;

line = strel('disk',7);
im2 = imerode(im2,line,'same');
remaining_count = max(max(bwlabel(im2))); %nb de pieces restantes dans l'image
count_2d = current_count - remaining_count;

money_value = (count_10c * 0.1) + (count_5c * 0.05) + (count_25c * 0.25) + (count_1d * 1) + (count_2d *2);
money_count = [count_5c count_10c count_25c count_1d count_2d];

end

