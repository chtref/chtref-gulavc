function [ out_value ] = Apply_LUT( in_value, lookup_tbl )
%APPLY_LUT Summary of this function goes here
%   Detailed explanation goes here

x = 1;

while lookup_tbl(x,1) < in_value
    x = x+1;
end

out_value = lookup_tbl(x,2);

end

