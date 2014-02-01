function [Pb] = procPBlocking(ExpectedStreams,Nc,Nsdv,shape,numberSlices)
%
%   ExpectedStreams: mean number of streams
%   Nc:  Total number of channels of content (the most popular will not
%           be managed by SDV
%   Nsdv : The subset of Nc that are managed by SDV. THe rest are 
%          broadcast.
%   Npvr : The number of peak unicast requests
%   Npmr : The number of peak multicast requests
%   shape : The shape parameter for the power law model used to
%              find the probabilities that particular channels are viewed.
%   numberOfSlices : The number of transmission channel slices
%
%   Computes the blocking probability if x channels are utilized
%           Computes P[X>numberOfSlices] =   (x/xmin) power -alpha +1
%
%


debugFlag = 0;

if (debugFlag == 1)
  fprintf(1,'procPBlocking: n:%d, Nc:%d, shape:%f\n',n,Nc,shape);
end

Xmin=1;


tailP=0.0;
probPL = 0.0;
if (Nsdv > numberSlices)
  for i= numberSlices: Nsdv
    probPL = procPLp(i,Nc,shape);
    tailP = tailP + probPL;
  end
end

Pb=tailP;


if (debugFlag == 1)
  fprintf(1,'procPBlocking: blocking probability:%3.3f \n',Pb);
end








