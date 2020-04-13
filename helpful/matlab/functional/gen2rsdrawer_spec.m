function [ output_args ] = gen2rsdrawer_spec(data, in_title)
%GEN2RSDRAWER_SPEC Summary of this function goes here
%   Detailed explanation goes here
    hold on;
    grid on;
    mtr = data.Z.multi;
   [m, n] = size(mtr);
   legends = cell(1, n);
   
   for colIdx=1:1:n
       plot([1:1:10], mtr(:,colIdx), '*');%, 'DisplayName', num2str(5*colIdx));
       legends{colIdx} = num2str(5*colIdx);
   end 
   lgd = legend(legends);
   title(lgd,['Радиус' newline 'видимости']);
   legend('Location','northeastoutside')
   title(in_title);
   xlabel('Количество роботов');
   ylabel('Пройденное расстояние');
   axis([0 10 0 15000]);
end

