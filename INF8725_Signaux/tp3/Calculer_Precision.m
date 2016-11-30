function [ performance, tauxFauxPositif, tauxFauxNegatif ] = Calculer_Precision( imgSeg, imgRef )
%CALCULER_PRECISION Calculer_Precision
%   Calculer_Precision

contourDetectes = sum(sum(imgSeg > 0));
contourReferences = sum(sum(imgRef > 0));
contourCorrects = sum(sum((imgSeg .* imgRef) > 0));

fauxPositifs = contourDetectes - contourCorrects;
fauxNegatifs = contourReferences - contourCorrects;

decimator = contourCorrects + fauxPositifs + fauxNegatifs;

performance = contourCorrects / decimator;
tauxFauxPositif = fauxPositifs / decimator;
tauxFauxNegatif = fauxNegatifs / decimator;


end

