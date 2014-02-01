function [Pn] = procPLp(n,Nc,shape)
%
%   Computes the probability that the n'th (Pn) entry in a sorted list (by
%    popularity) of size Nc will be selected.
%
%   I.e., this is the Si power law distribution:
%
%   Params:
%     n:  represents which element in the list 
%     Nc:  Size of the list
%     shape : The shape of the power law distribution


debugFlag = 0;

if (debugFlag == 1)
  fprintf(1,'procPLp: n:%d, Nc:%d, shape:%f\n',n,Nc,shape);
end


Numerator = (n^(-1*shape));
Denominator = 0.0;


for i=1:Nc
  Denominator = Denominator + (i^(-1*shape));
end


if (Denominator > 0)
  Pn= Numerator / Denominator;
else
  Pn= 0;
end


if (debugFlag == 1)
  fprintf(1,'procPLp: Numerator:%f,  Denominator:%f, Pn:%f \n',Numerator, Denominator,Pn);
end





