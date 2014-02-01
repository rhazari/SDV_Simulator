function plotPDF()
%
%  plotPDF: This program plots the distribution of the input file.
%           The input file format is a single number representing milliseconds.
%           For example:
%               49.9
%               50.6
%               50.0
%               49.6
%               49.5
%   params:
%       fieldNumber: this is optional. If the data file has multiple fields
%             this specifies.  If not given, we assume the data is a single field.
%
%  invocation:    plotPDF()
%

%for morning session
%load client_morning_ping.dat
%rng = client_morning_ping

%for night session
%load client_night_ping.dat;
%rng = client_night_ping;

load test.txt
rng  = test;

maxValue = 1000;
numberEntries=size(rng);
sampleSize=numberEntries(1);

for i = 1 : sampleSize
  if (rng(i) > maxValue)
    rng(i) = maxValue;
  end
end


meanStat = mean(rng);
medianStat = median(rng);
stdStat = std(rng);
Dmin = min(rng);

fprintf(1,'Mean:  %f,  median: %f,  std: %f, Dmin: %f\n',meanStat,medianStat,stdStat,Dmin);


numberBins = sampleSize;


[N,X] = hist(rng,numberBins);
%fprintf(1,'plotDistribution: N: %d \n',N);
%disp(N);
N=N/sampleSize;

%calculate end-to-end peak 
percentile = 0;
quantile = 0;
for i = 1:sampleSize
    percentile = percentile + N(i);
    if (percentile >= 0.95)
        fprintf(1, '1-5%% quantile is %f \n', X(i));
        quantile = X(i);
        break;
    end
end
fprintf(1, 'Peak-to-Peak CDV is %f\n', quantile - Dmin);

S=0;
for i = 1:numberBins

%Find the largest 2 modes ?
%disp(N(i));
   S = S + N(i);
   CDF(i) =S;
end

%to plot PDF ...
figure(1);
bar('v6',X,N,'r--');
hold on
grid
ylabel('probability density')
xlabel('Throughput in Mbps')
title('PDF')
hold off

%to plot CDF
figure(2);
bar('v6',X,CDF,'b--');
hold on
grid 
ylabel('probability distribution')
xlabel('Throughput in Mbps')
title('CDF')
hold off


