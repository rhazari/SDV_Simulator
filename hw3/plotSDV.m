function plotSDV(Nc,Nsdv,Npur)
%
%   
%  This computes the SDV results for a range of input parameters. 
%  
%   This finds the expected number of multicast and unicast streams to
%    support the demand described in the parameters.
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
%       plotSDV(287,191,0,257,0.90)
%       plotSDV


if (nargin < 5)
  Nc = 287;
  Nsdv = 191;
  Npur = 0;
  Npmr = 257;
  shape = 0.90;
end

tmpShape(1) = 0.20;
tmpShape(2) = 0.50;
tmpShape(3) = 0.80;
tmpShape(4) = 1.20;
tmpShape(5) = 1.50;
tmpShape(6) = 1.80;

tmpNpmr(1) = 50;
tmpNpmr(2) = 100;
tmpNpmr(3) = 150;
tmpNpmr(4) = 200;
tmpNpmr(5) = 250;
tmpNpmr(6) = 300;

numberInteriorRuns = 6;
numberOuterRuns = 6;
[yLabelString, errmsg] = sprintf(' not defined yet');


for i= 1: numberOuterRuns
  for j= 1: numberInteriorRuns
    Nsm(j) = procSDV(Nc,Nsdv,Npur,tmpNpmr(j),tmpShape(i));
    Npmr(j) = tmpNpmr(j);
% tmpNpmr = tmpNpmr * 2;
%    tmpNpmr = tmpNpmr + NpmrIncrement;
  end
% Plot this curve
  switch (i)
    case {1}
      plot(Npmr, Nsm,'k^-');
      hold on;
      grid on;
    case {2}
      plot(Npmr, Nsm,'k<-');
      hold on;
      grid on;
    case {3}
      plot(Npmr, Nsm,'k>-');
      hold on;
      grid on;
    case {4}
      plot(Npmr, Nsm,'ks-');
      hold on;
      grid on;
    case {5}
      plot(Npmr, Nsm,'kd-');
      hold on;
      grid on;
    case {6}
      plot(Npmr, Nsm,'kp-');
      hold on;
      grid on;
    case {10}
      plot(Npmr, Nsm,'k-');
      hold on;
      grid on;
  end
end

hold on;
grid on;
title('Expected Number of SDV Streams ');
xlabel('Peak Multicast Requests (Npmr) ')
ylabel('Average Number of SDV Streams')

legend('shape=0.20', 'shape=0.50','shape=0.80', 'shape=1.20','shape=1.5','shape=1.80', 0);


