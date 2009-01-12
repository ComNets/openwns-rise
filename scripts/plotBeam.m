% call this script with matlab, e.g.,  
% /software/matlab-2006a/bin/matlab -nodesktop -nojvm plotBeam

% it is not yet supported, but you can try it with octave (free software)

function plotBeam()
clear all

patternLocation = '../../../tests/unit/unitTests/output/';

% unit test specific station coordinates:
% coordinates of stations 1, 2, 3, and 4
[theta_radians, rho] = cart2pol([0 30 100 40], [0 20 10 100]); 
theta_grad = theta_radians/pi*180;
theta = round(theta_grad);

[pattern pattern_dB] = readPattern([patternLocation 'BFAntenna.dat']);
[pattern_error pattern_error_dB] = readPattern([patternLocation 'BFAntennaError.dat']);
[pattern_circular pattern_circular_dB] = readPattern([patternLocation 'BFAntennaCircular.dat']);

%%%%%%%%%%%%%%%%%%%%%%
% plot antenna pattern
%%%%%%%%%%%%%%%%%%%%%%

scale = (0:1:359);
scale = scale ./180 .*pi;

% prepare plot area
figure('NumberTitle', 'off', ...
    'Name', 'Antenna patterns', ...
    'PaperPositionMode', 'auto',...
    'Position', [150 250 500 400],...
    'Pointer', 'arrow');

%print -loose -noui -depsc antenna_patterns.eps;

% plot data
polar(scale,pattern,'k')
hold on
polar(scale,pattern_error,'r')
polar(scale,pattern_circular,'b')
%hold on
polar(theta(2)/180*pi, 5,'ko')
polar(theta(3)/180*pi, 5,'k*')
polar(theta(4)/180*pi, 5,'k*')
legend('antenna gain', 'erroneous pattern', 'circular layout', ...
    'desired station', 'undesired stations', 3)