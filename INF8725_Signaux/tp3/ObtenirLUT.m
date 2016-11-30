function [ lookup_tbl ] = ObtenirLUT( nb_segs )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

delta = 255 / nb_segs;

lookup_tbl = zeros(nb_segs,2);

for n = 1:nb_segs
    lookup_tbl(n,1) = round(delta*n);
    lookup_tbl(n,2) = round((round(delta*n) + round(delta*(n-1))) / 2);
end

end

