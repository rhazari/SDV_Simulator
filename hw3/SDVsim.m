function SDVsim(Nc,Nsdv,Npur,Npmr,shape,numberOfSlices)
% This finds the expected number of multicast and unicast streams to
%  support the demand described in the parameters.
%
%   Nc:  Total number of channels of content (the most popular will not
%           be managed by SDV
%   Nsdv : The subset of Nc that are managed by SDV. THe rest are 
%          broadcast.
%   Npvr : The number of peak unicast requests
%   Npmr : The number of peak multicast requests
%   shape : The shape parameter for the power law model used to
%              find the probabilities that particular channels are viewed.
%    numberOfSlices : The number of transmission channel slices
%
%  Example:
%       SDVsim(200,120,20,50,0.90,200)


%  Let Y be the sum of RVs Xi- each being a 1 or 0 if the channel is selected or not 
%  Results we compute:
% 
%    meanY : this is the sample mean of the number of streams required to support demand
%    stdY :  the std of Y
%    Pblocking : The blocking prob associated with this selection of V


debugFlag = 1;

sum=0.0;
if (nargin == 0)
  Nc = 350;
  Nsdv = Nc;
  Npur = 0;
  Npmr = 257;
  shape= 0.90;
  %numberOfSlices=Nsdv;
  numberOfSlices = 100;
end

if (debugFlag == 1)
  fprintf(1,'SDVsim: Nc:%d, Nsdb:%d, Npur:%d,  Npmr:%d, shape:%f\n',Nc,Nsdv,Npur,Npmr,shape);
end

for i=1:Nsdv
  [Pi] = procPLp(i,Nc,shape);
  sum = sum + power((1-Pi),Npmr);
end


meanY = Nsdv - sum;
Pblocking = procPBlocking(meanY,Nc,Nsdv,shape,numberOfSlices);


fprintf(1,'SDVsim: Expected number of broadcast streams:%d, Pblocking:%f \n',meanY,Pblocking);


