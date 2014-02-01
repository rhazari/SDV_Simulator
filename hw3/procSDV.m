function [Nms] = procSDV(Nc,Nsdv,Npur,Npmr,shape)
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
%
%  Example:
%       SDVsim(200,120,20,50,0.90)


%  Let Y be the sum of RVs Xi- each being a 1 or 0 if the channel is selected or not 
%  Results we compute:
% 
%    meanY : this is the sample mean of the number of streams required to support demand
%    stdY :  the std of Y
%    Pblocking : The blocking prob associated with this selection of V

%    The meanY is :   the sample mean of Y =   Nsdv - E[Number of channels NOT used]
%                  Latter term:   sum( (1-Si)EXPNpmr      )


debugFlag = 1;

sum=0.0;
if (nargin == 0)
  Nc = 100;
  Nsdv = 100;
  Npur = 0;
  Npmr = 200;
  shape= 0.90;
end

if (debugFlag == 1)
  fprintf(1,'SDVsim: Nc:%d, Nsdb:%d, Npur:%d,  Npmr:%d, shape:%f\n',Nc,Nsdv,Npur,Npmr,shape);
end

for i=1:Nsdv
  [Si] = procPLp(i,Nc,shape);
  sum = sum + power((1-Si),Npmr);
end


meanY = Nsdv - sum;
numberOfSlices=Nsdv;
Pblocking = procPBlocking(meanY,Nc,Nsdv,shape,numberOfSlices);


Nms = meanY;

if (debugFlag == 1)
  fprintf(1,'SDVsim: expected number of streams:%d, Pblocking:%3.3f \n',meanY,Pblocking);
end


