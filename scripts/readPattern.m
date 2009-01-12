function [pattern pattern_dB] = readPattern(string)

% coordinates of stations 1, 2, 3, and 4
[theta, rho] = cart2pol([0 30 100 40], [0 20 10 100]); 
theta = round(theta/pi*180);

%read unittest output
fid = fopen(string,'r');

[MyData, count] = fscanf(fid, '%g %g', [2 inf]);
status = fclose(fid);
if(count ~= 2*360)
    disp('error');
    count
end;

pattern = zeros(1, 360);
sumPower = 0.0;
for i=1: 1: 359
    pattern(i) = sqrt( MyData(1, i+1)^2 + MyData(2, i+1)^2);
    sumPower = sumPower + pattern(i);
end
pattern(360) = sqrt( MyData(1, 1)^2 + MyData(2, 1)^2);
sumPower = sumPower + pattern(360);

if (abs(sumPower/360) < 0.01)
    disp('error');
    sumPower/360
end

pattern_dB = 10.*log10(pattern);

format long
disp('pattern characteristics')
disp(['max gain: ' num2str(max(pattern)) '  =  ' ...
    num2str( max(pattern_dB) ) ' dB']);
disp(['gain: ' num2str(pattern(theta(2))) '  =  ' ...
    num2str( pattern_dB(theta(2)) ) ' dB']);
disp(['gain: ' num2str(pattern(theta(2))) '  =  ' ...
    num2str( pattern_dB(theta(3)) ) ' dB']);
disp(['gain: ' num2str(pattern(theta(2))) '  =  ' ...
    num2str( pattern_dB(theta(4)) ) ' dB']);

for i=1:360
    if (pattern_dB(i) < 0) 
%        pattern_dB(i) = 0;
    end
end