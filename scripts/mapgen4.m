% function [] = mapgen()
%
% generate the SceneryMap file for GOOSE
%
% Ingo Forkel, Aug 2001
%
% The SceneryMap file contains path loss data in a 2-dimensional
% matrix. The first data entry is for the upper left hand corner of
% the scenario area. The following entries are for the upper row of
% path loss data entries (ascending x-value). The next row (ascending
% y-value) follows.
%
% Each path loss data entry contains three parts: a unsigned char
% value for the MORPHOLOGY, a SHADOWING_SIZE x SHADOWING_SIZE field of
% shadowing values (usually in a higher resolution), the PATHLOSS
% values for all BS. See following figure.
%
% +---+---+---+---+ - +---+---+---+---+ - +---+
% | M | Shadowing ...     |PL1|PL2|PL3|...|PLn|
% +---+---+---+---+ - +---+---+---+---+ - +---+
%      Structure of a path loss data enty:
%      M   = Morphology type
%      PL1 = Path loss to BS 1
%      PLn = Path loss to BS n
%      n   = Number of BS
%
%---------------------------------------------------------------------

function [] = mapgen4(varargin)

if length(varargin) > 0
  SceneryMap = varargin{1};
else
  SceneryMap = 'Scenery.sc'
end

% declarations
Pi          = 3.141592653589793238462643;
frequency   = 2000000000;  % in Hz
c           = 299792458;   % in m/s

% size of scenario in meter
% AmsterdamSmall:  2200 2200 10
% AmsterdamLarge:  3700 3360 20
% BernWankdorf:    3700 3625 25
% Biel:            8000 8000 40
% Manhattan:       2560 2790 10
size_x = 8000;
size_y = 8000;
resolution = 400;

% fixed tilting of the antenna
downtiltangle = 0; % already considered in antenna pattern!

% decorrelation length in meter
d_cor = 20;

% min and max pathloss entry
min_pathloss = 42;
max_pathloss = 255;

% scenario (x/[km] y/[km] h/[m] A_O/[°] Gain/[dBi])
bs_data = [ ...
%     [1.050 1.395 25.93  90 18.6]; ...
%     [1.050 1.395 25.93 210 18.6]; ...
%     [1.050 1.395 27.44 330 18.6]; ...
%     [0.750 1.675 28.50  90 18.6]; ...
%     [0.750 1.675 28.50 210 18.6]; ...
%     [0.750 1.675 28.50 330 18.6]; ...
%     [0.700 0.825 33.00  90 18.6]; ...
%     [0.700 0.825 33.00 210 18.6]; ...
%     [0.700 0.825 33.00 330 18.6]; ...
%     [1.475 1.475 28.00  90 18.6]; ...
%     [1.475 1.475 28.00 210 18.6]; ...
%     [1.475 1.475 28.00 330 18.6]; ...
%     [1.675 0.575 29.00  90 18.6]; ...
%     [1.675 0.575 29.00 210 18.6]; ...
%     [1.675 0.575 29.00 330 18.6]; ...
%     [0.925 0.475 30.00  90 18.6]; ...
%     [0.925 0.475 30.00 210 18.6]; ...
%     [0.925 0.475 30.00 330 18.6]; ...
%     [1.169 1.013 27.00  90 18.6]; ...
%     [1.169 1.013 27.00 210 18.6]; ...
%     [1.169 1.013 27.00 330 18.6]];
%     [1.710 1.920 25.93  90 18.6]; ...
%     [1.710 1.920 25.93 210 18.6]; ...
%     [1.710 1.920 27.44 330 18.6]; ...
%     [1.400 3.300 32.90  90 18.6]; ...
%     [1.400 3.300 32.00 210 18.6]; ...
%     [1.400 3.300 32.00 330 18.6]; ...
%     [0.610 2.850 42.50  90 18.6]; ...
%     [0.610 2.850 42.50 210 18.6]; ...
%     [0.610 2.850 42.50 330 18.6]; ...
%     [0.610 0.280 25.88  90 18.6]; ...
%     [0.610 0.280 24.20 210 18.6]; ...
%     [0.610 0.280 25.88 330 18.6]; ...
%     [0.060 1.580 35.50  90 18.6]; ...
%     [0.060 1.580 35.50 210 18.6]; ...
%     [0.060 1.580 35.50 330 18.6]; ...
%     [1.410 2.200 28.50  90 18.6]; ...
%     [1.410 2.200 28.50 210 18.6]; ...
%     [1.410 2.200 28.50 330 18.6]; ...
%     [1.360 1.350 33.00  90 18.6]; ...
%     [1.360 1.350 33.00 210 18.6]; ...
%     [1.360 1.350 33.00 330 18.6]; ...
%     [0.320 1.940 30.00  90 18.6]; ...
%     [0.320 1.940 30.00 210 18.6]; ...
%     [0.320 1.940 30.00 330 18.6]; ...
%     [2.135 2.000 28.00  90 18.6]; ...
%     [2.135 2.000 28.00 210 18.6]; ...
%     [2.135 2.000 28.00 330 18.6]; ...
%     [2.335 1.100 29.00  90 18.6]; ...
%     [2.335 1.100 29.00 210 18.6]; ...
%     [2.335 1.100 29.00 330 18.6]; ...
%     [0.685 1.075 22.00  90 18.6]; ...
%     [0.685 1.075 25.28 210 18.6]; ...
%     [0.685 1.075 25.28 330 18.6]; ...
%     [3.235 1.625 18.65  90 18.6]; ...
%     [3.235 1.625 18.65 210 18.6]; ...
%     [3.235 1.625 18.65 330 18.6]; ...
%     [1.110 0.525 31.00  90 18.6]; ...
%     [1.110 0.525 31.00 210 18.6]; ...
%     [1.110 0.525 31.00 330 18.6]; ...
%     [2.350 2.520 31.50  90 18.6]; ...
%     [2.350 2.520 31.50 210 18.6]; ...
%     [2.350 2.520 31.50 330 18.6]; ...
%     [1.585 1.050 30.00  90 18.6]; ...
%     [1.585 1.050 30.00 210 18.6]; ...
%     [1.585 1.050 30.00 330 18.6]; ...
%     [1.829 1.538 27.00  90 18.6]; ...
%     [1.829 1.538 27.00 210 18.6]; ...
%     [1.829 1.538 27.00 330 18.6]; ...
%     [3.360 2.350 29.20  90 18.6]; ...
%     [3.360 2.350 29.20 210 18.6]; ...
%     [3.360 2.350 29.20 330 18.6]];
%     [5.914 6.033 24.25  20 18.6]; ...
%     [5.914 6.033 24.25 130 18.6]; ...
%     [5.914 6.033 24.25 270 18.6]; ...
%     [5.823 2.776 18.50  60 18.6]; ...
%     [5.823 2.776 18.50 180 18.6]; ...
%     [5.823 2.776 18.50 270 18.6]; ...
%     [6.773 1.419 29.20   0 18.6]; ...
%     [6.773 1.419 29.20 120 18.6]; ...
%     [6.773 1.419 29.20 240 18.6]; ...
%     [4.590 2.712 24.70  30 18.6]; ...
%     [4.590 2.712 24.70 150 18.6]; ...
%     [4.590 2.712 24.70 270 18.6]; ...
%     [2.974 4.643 35.00  60 18.6]; ...
%     [2.974 4.643 35.00 180 18.6]; ...
%     [2.974 4.643 35.00 300 18.6]; ...
%     [4.552 3.704 22.50   0 18.6]; ...
%     [4.552 3.704 22.50 120 18.6]; ...
%     [4.552 3.704 22.50 240 18.6]; ...
%     [5.295 3.485 33.25   0 18.6]; ...
%     [5.295 3.485 33.25 100 18.6]; ...
%     [5.295 3.485 33.25 250 18.6]; ...
%     [3.880 5.057 19.80  60 18.6]; ...
%     [3.880 5.057 19.80 180 18.6]; ...
%     [3.880 5.057 19.80 300 18.6]; ...
%     [3.385 3.791 25.20   0 18.6]; ...
%     [3.385 3.791 25.20 120 18.6]; ...
%     [3.385 3.791 25.20 270 18.6]; ...
%     [2.326 4.435 22.10  20 18.6]; ...
%     [2.326 4.435 22.10 160 18.6]; ...
%     [2.326 4.435 22.10 250 18.6]; ...
%     [3.924 4.161 23.90   0 18.6]; ...
%     [3.924 4.161 23.90 120 18.6]; ...
%     [3.924 4.161 23.90 240 18.6]; ...
%     [2.593 5.393 18.00   0 18.6]; ...
%     [2.593 5.393 18.00 120 18.6]; ...
%     [2.593 5.393 18.00 240 18.6]; ...
%     [2.802 6.258 21.50   0 18.6]; ...
%     [2.802 6.258 21.50 120 18.6]; ...
%     [2.802 6.258 21.50 220 18.6]; ...
%     [3.640 6.309 24.20 130 18.6]; ...
%     [3.640 6.309 24.20 210 18.6]; ...
%     [3.773 5.767 24.30  60 18.6]; ...
%     [3.773 5.767 24.30 180 18.6]; ...
%     [3.773 5.767 24.30 300 18.6]];
    [5.914    6.033   22.15    20 14.0]; ...
    [5.914    6.033   22.15   130 14.0]; ...
    [5.914    6.033   22.15   270 14.0]; ...
    [6.162    1.727   23.8      0 14.0]; ...
    [6.162    1.727   23.8    120 14.0]; ...
    [6.162    1.727   23.8    240 14.0]; ...
    [5.823    2.776   20.3     60 14.0]; ...
    [5.823    2.776   20.3    180 14.0]; ...
    [5.823    2.776   20.3    270 14.0]; ...
    [6.773    1.419   31.2      0 14.0]; ...
    [6.773    1.419   31.2    120 14.0]; ...
    [6.773    1.419   31.2    240 14.0]; ...
    [3.989    3.297   20.9    120 14.0]; ...
    [3.989    3.297   20.9    240 14.0]; ...
    [3.989    3.297   20.9    340 14.0]; ...
    [4.59     2.712   22.6    120 14.0]; ...
    [4.59     2.712   22.6    240 14.0]; ...
    [4.59     2.712   22.6    350 14.0]; ...
    [2.974    4.643   46.7      0 14.0]; ...
    [2.974    4.643   46.7    120 14.0]; ...
    [2.974    4.643   48.9    240 14.0]; ...
    [4.552    3.704   24.3      0 14.0]; ...
    [4.552    3.704   24.3    140 14.0]; ...
    [4.552    3.704   24.3    240 14.0]; ...
    [5.295    3.485   35.55     0 14.0]; ...
    [5.295    3.485   35.55   100 14.0]; ...
    [5.295    3.485   35.55   240 14.0]; ...
    [3.88     5.057   21.6      0 14.0]; ...
    [3.88     5.057   21.6     90 14.0]; ...
    [3.88     5.057   21.6    240 14.0]; ...
    [3.385    3.791   3.0       0 14.0]; ...
    [3.385    3.791   25.2    120 14.0]; ...
    [3.385    3.791   25.2    250 14.0]; ...
    [2.326    4.435   24.25    20 14.0]; ...
    [2.326    4.435   24.25   110 14.0]; ...
    [2.326    4.435   24.25   270 14.0]; ...
    [3.924    4.161   21.8      0 14.0]; ...
    [3.924    4.161   21.8    120 14.0]; ...
    [3.924    4.161   21.8    240 14.0]; ...
    [3.018    7.906   17.3     70 14.0]; ...
    [3.018    7.906   17.3    160 14.0]; ...
    [3.018    7.906   17.3    300 14.0]; ...
    [4.721    5.8     29.7     70 14.0]; ...
    [4.721    5.8     29.7    160 14.0]; ...
    [4.721    5.8     29.7    290 14.0]; ...
    [3.784    0.795   37.8    245 14.0]; ...
    [0.342    4.152   14.0    310 14.0]; ...
    [2.593    5.393   18        0 14.0]; ...
    [2.593    5.393   18      120 14.0]; ...
    [2.593    5.393   18      240 14.0]; ...
    [2.802    6.258   23.2      0 14.0]; ...
    [2.802    6.258   23.2    120 14.0]; ...
    [2.802    6.258   23.2    220 14.0]; ...
    [7.959    4.344   24.5      0 14.0]; ...
    [7.959    4.344   24.5    120 14.0]; ...
    [7.959    4.344   24.5    220 14.0]; ...
    [0.938    2.177   11.2     20 14.0]; ...
    [0.938    2.177   11.2    270 14.0]; ...
    [3.64     6.309   22.1    130 14.0]; ...
    [3.64     6.309   22.1    210 14.0]; ...
    [3.773    5.767   22.2      0 14.0]; ...
    [3.773    5.767   22.2    120 14.0]; ...
    [3.773    5.767   22.2    230 14.0]; ...
    [6.939    6.452   22.2      0 14.0]; ...
    [6.939    6.452   22.2    120 14.0]; ...
    [6.939    6.452   22.2    240 14.0]; ...
    [4.435    1.723   10.6     40 14.0]];

num_bs = size(bs_data, 1)

ms_heigth   = 1.5;
roof_height = mean(bs_data(:,3)) - 15;

% Shadowing data
shadowing_size = 4;
shadowing_STD = 8;
shadow_x = (size_x / resolution) * shadowing_size;
shadow_y = (size_y / resolution) * shadowing_size;
% used for graphics
low_x  = shadow_x * 0.25;
high_x = shadow_x * 0.75;
diff_x = high_x - low_x;
low_y  = shadow_y * 0.25;
high_y = shadow_y * 0.75;
diff_y = high_y - low_y;

% generate a random matrix of shadowing values for all filtering algorithms
shadow = shadowing_STD * randn(2 * shadow_x, 2 * shadow_y);

%-------------------------------------------------------------------------------------------%
%----------------------------------Random Variables-----------------------------------------%      
%-------------------------------------------------------------------------------------------%

% disp('Random Variables:');

% % calculate random shadowing values what happens here is that the
% % scenario is subdivided into it's resolution both in the x and y
% % direction and filled by randiom numbers. The reason that a '1' is
% % added is to increase the dimension of the matrix to the next
% % integer value so that all possible positions are covered.
                                        
% shadow_random = shadow(1:shadow_x, 1:shadow_y);
% disp([mean(mean(shadow_random)) mean(std(shadow_random,0,1)) mean(std(shadow_random,0,2))]);

% % correct mean and STD
% shadow_random = (shadow_random - mean(mean(shadow_random))) ...
%     * shadowing_STD / mean(std(shadow_random,0,1));
% disp([mean(mean(shadow_random)) mean(std(shadow_random,0,1)) mean(std(shadow_random,0,2))]);
% disp(size(shadow_random));

% figure('NumberTitle', 'off', ...
%        'Name', 'Random', ...
%        'PaperPositionMode', 'auto',...
%        'Position', [150 250 500 400],...
%        'Pointer', 'arrow');
% % button to create eps-file
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Print',...
% 	  'Position', [2 2 75 30],...
% 	  'Callback', 'print -loose -noui -depsc area_random.eps;');
% % button to exit
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Exit',...
% 	  'Position', [423 2 75 30],...
% 	  'Callback', 'close;');

% set(gcf, 'DefaulttextInterpreter', 'none');
% set(gcf, 'DefaulttextFontSize', 14);
% set(gca, 'DefaulttextFontSize', 14);
% %set(gca, 'Position', [0.25 0.12 0.7 0.83]);
% box on;

% graph = pcolor(shadow_random);
% xlim([low_x high_x]);
% ylim([low_y high_y]);
% set(graph, 'LineStyle', 'none');
% set(gca, 'CLimMode', 'manual');
% set(gca, 'CLim', [-3*shadowing_STD, +3*shadowing_STD]);
% colorbar;
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel')) * resolution/shadowing_size));
% set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel')) * resolution/shadowing_size));
% xlabel('x [m]', 'FontSize', 14, 'Interpreter', 'none');
% ylabel('y [m]', 'FontSize', 14, 'Interpreter', 'none');

%-------------------------------------------------------------------------------------------%
%----------------------------------Goose Filtering------------------------------------------%
%-------------------------------------------------------------------------------------------%

% disp('Filtering with four Coefficients:');

% % filter shadowing values (smoothing) with the shadowing coefficients
% shadowing_coeff = [.5 .25 .125 .06125]; % as many as shadowing_size

% for y=shadowing_size+1:shadowing_size+shadow_y
%   for x=shadowing_size+1:shadowing_size+shadow_x
%     s = shadow(x, y);
%     for corr=1:shadowing_size
%       s = s + shadowing_coeff(corr) * (shadow(x-corr, y));
%       s = s + shadowing_coeff(corr) * (shadow(x, y-corr));
%     end
%     if     (s < -127)
%       s = -127;
%     elseif (s > 127)
%       s = 127;
%     end
%     shadow_4coeff(x-shadowing_size, y-shadowing_size) = s;
%   end
% end
% disp([mean(mean(shadow_4coeff)) mean(std(shadow_4coeff,0,1)) mean(std(shadow_4coeff,0,2))]);

% % correct mean and STD
% shadow_4coeff = (shadow_4coeff - mean(mean(shadow_4coeff))) ...
%     * shadowing_STD / mean(std(shadow_4coeff,0,1));
% disp([mean(mean(shadow_4coeff)) mean(std(shadow_4coeff,0,1)) mean(std(shadow_4coeff,0,2))]);
% disp(size(shadow_4coeff));

% figure('NumberTitle', 'off', ...
%        'Name', '4 Coefficient', ...
%        'PaperPositionMode', 'auto',...
%        'Position', [150 250 500 400],...
%        'Pointer', 'arrow');
% % button to create eps-file
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Print',...
% 	  'Position', [2 2 75 30],...
% 	  'Callback', 'print -loose -noui -depsc area_4coeff.eps;');
% % button to exit
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Exit',...
% 	  'Position', [423 2 75 30],...
% 	  'Callback', 'close;');

% set(gcf, 'DefaulttextInterpreter', 'none');
% set(gcf, 'DefaulttextFontSize', 14);
% set(gca, 'DefaulttextFontSize', 14);
% %set(gca, 'Position', [0.25 0.12 0.7 0.83]);
% box on;

% graph = pcolor(shadow_4coeff);
% xlim([low_x high_x]);
% ylim([low_y high_y]);
% set(graph, 'LineStyle', 'none');
% set(gca, 'CLimMode', 'manual');
% caxis([-3*shadowing_STD +3*shadowing_STD]);
% colorbar;
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel')) * resolution/shadowing_size));
% set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel')) * resolution/shadowing_size));
% xlabel('x [m]', 'FontSize', 14, 'Interpreter', 'none');
% ylabel('y [m]', 'FontSize', 14, 'Interpreter', 'none');
               
%-------------------------------------------------------------------------------------------%
%----------------------------------Spline Interpolation-------------------------------------%
%-------------------------------------------------------------------------------------------%

% disp('Spline Interpolation:');

% % To increase the autocorrelation, Mark has come up with an
% % exceptional idea in that he first subdivided the scenario into
% % widths corresponding to the decorrelation lengths and then adding
% % 8 more lines to later extract the rest of the coefficients after
% % the 'spline' function has been called up.

% % The pcolorgrid functions simply lays a finer grid. The first call up
% % of the function lays a grid from 0 to size_x/d_cor + 8*d_cor with
% % a step size of d_cor. 

% % The second call up of the function lays a grid over the first grid
% % with a different resolution of resolution/scenrio_size.

% % shadow_part = shadow(1:d_cor:1+size_x+8*d_cor, 1:d_cor:1+size_y+8*d_cor);
% % [shad_x, shad_y] = meshgrid(0:d_cor:size_x+8*d_cor, 0:d_cor:size_y+8*d_cor);
% % [shad_x_smth, shad_y_smth] = meshgrid(0:resolution/shadowing_size:size_x+8*d_cor, ...
% % 				      0:resolution/shadowing_size:size_y+8*d_cor); 

% % tmp = interp2(shad_x, shad_y, shadow, shad_x_smth, shad_y_smth, 'spline');

% step = (d_cor/resolution) * shadowing_size;
% shadow_part = shadow(1:step:1+shadow_x+8*step, 1:step:1+shadow_y+8*step);
% disp([size(shadow) size(shadow_part)]);
% [shad_x, shad_y] = meshgrid(0:step:shadow_x+8*step, 0:step:shadow_y+8*step);
% disp([size(shad_x) size(shad_y)]);
% [shad_x_smth, shad_y_smth] = meshgrid(0:shadow_x+8*step, ...
% 				      0:shadow_y+8*step);
% disp([size(shad_x_smth) size(shad_y_smth)]);

% % Now the first grid[shad_x, shad_y] with the random numbers are
% % interpolated and with the 'spline' function, additional random
% % numbers are generated to fit the 2nd grid [shad_x_smth, shad_y_smth].

% tmp = interp2(shad_x, shad_y, shadow_part, shad_x_smth, shad_y_smth, 'spline');

% % What the spacer does is to cut off the remaining random numbers
% % that have been additionally generated by the addition of the 8

% %spacer = round(1.5*d_cor*shadowing_size/resolution);
% shadow_spline = tmp(5:shadow_x+4, 5:shadow_y+4);
% disp([mean(mean(shadow_spline)) mean(std(shadow_spline,0,1)) mean(std(shadow_spline,0,2))]);

% % correct mean and STD
% shadow_spline = (shadow_spline - mean(mean(shadow_spline))) ...
%     * shadowing_STD / mean(std(shadow_spline,0,1));
% disp([mean(mean(shadow_spline)) mean(std(shadow_spline,0,1)) mean(std(shadow_spline,0,2))]);
% disp(size(shadow_spline));

% % hist(shadow_spline);

% figure('NumberTitle', 'off', ...
%        'Name', 'Spline Interpolation', ...
%        'PaperPositionMode', 'auto',...
%        'Position', [150 250 500 400],...
%        'Pointer', 'arrow');
% % button to create eps-file
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Print',...
% 	  'Position', [2 2 75 30],...
% 	  'Callback', 'print -loose -noui -depsc area_spline.eps;');
% % button to exit
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Exit',...
% 	  'Position', [423 2 75 30],...
% 	  'Callback', 'close;');

% set(gcf, 'DefaulttextInterpreter', 'none');
% set(gcf, 'DefaulttextFontSize', 14);
% set(gca, 'DefaulttextFontSize', 14);
% % set(gca, 'Position', [0.25 0.12 0.7 0.83]);
% box on;

% graph = pcolor(shadow_spline);
% xlim([low_x high_x]);
% ylim([low_y high_y]);
% set(graph, 'LineStyle', 'none');
% set(gca, 'CLimMode', 'manual');
% caxis([-3*shadowing_STD +3*shadowing_STD]);
% colorbar;
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel')) * resolution/shadowing_size));
% set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel')) * resolution/shadowing_size));
% xlabel('x [m]', 'FontSize', 14, 'Interpreter', 'none');
% ylabel('y [m]', 'FontSize', 14, 'Interpreter', 'none');

% clear tmp;

%-------------------------------------------------------------------------------------------%
%-------------------------------------2D-acorf Filtering------------------------------------%
%-------------------------------------------------------------------------------------------%

disp('2-Dimensional Convolution:');

i       = 0:shadow_x+1; %total number of spaces
delta_x = i'*(resolution/shadowing_size);
R_col   = exp(-delta_x/d_cor*log(2)); %calculating the akf coeff.
i       = 0:shadow_y+1; %total number of spaces
delta_y = i*(resolution/shadowing_size);
R_row   = exp(-delta_y/d_cor*log(2));    

shadow_2Dconv = conv2(R_col, R_row, shadow, 'same');
shadow_2Dconv = shadow_2Dconv(1:shadow_x, 1:shadow_y);

disp([mean(mean(shadow_2Dconv)) mean(std(shadow_2Dconv,0,1)) mean(std(shadow_2Dconv,0,2))]);

% correct mean and STD
shadow_2Dconv = (shadow_2Dconv - mean(mean(shadow_2Dconv))) ...
    * shadowing_STD / mean(std(shadow_2Dconv,0,1));
disp([mean(mean(shadow_2Dconv)) mean(std(shadow_2Dconv,0,1)) mean(std(shadow_2Dconv,0,2))]);
disp(size(shadow_2Dconv));

% hist(shadow_2Dconv);

figure('NumberTitle', 'off', ...
       'Name', '2D Convolution', ...
       'PaperPositionMode', 'auto',...
       'Position', [150 250 500 400],...
       'Pointer', 'arrow');
% button to create eps-file
uicontrol('Style', 'pushbutton',...
	  'String', 'Print',...
	  'Position', [2 2 75 30],...
	  'Callback', 'print -loose -noui -depsc area_2dconv.eps;');
% button to exit
uicontrol('Style', 'pushbutton',...
	  'String', 'Exit',...
	  'Position', [423 2 75 30],...
	  'Callback', 'close;');

set(gcf, 'DefaulttextInterpreter', 'none');
set(gcf, 'DefaulttextFontSize', 14);
set(gca, 'DefaulttextFontSize', 14);
% set(gca, 'Position', [0.25 0.12 0.7 0.83]);
box on;

graph = pcolor(shadow_2Dconv);
xlim([low_x high_x]);
ylim([low_y high_y]);
set(graph, 'LineStyle', 'none');
set(gca, 'CLimMode', 'manual');
caxis([-3*shadowing_STD +3*shadowing_STD]);
colorbar;
set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel')) * resolution/shadowing_size));
set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel')) * resolution/shadowing_size));
xlabel('x [m]', 'FontSize', 14, 'Interpreter', 'none');
ylabel('y [m]', 'FontSize', 14, 'Interpreter', 'none');

% hist(shadow_2Dconv(:)); %the (:) represents all the elements of a matrix in a single column

%-------------------------------------------------------------------------------------------%
%-----------------------------------------Acorf Plot----------------------------------------%
%-------------------------------------------------------------------------------------------%
                       
% cd ('/home/msg/DA/matlab/');

% cor_random = 0;
% cor_4coeff = 0;
% cor_spline = 0;
% cor_2Dconv = 0;
% for j = 1:diff_x
%   cor_random = cor_random + acorf(shadow_random(low_x+j, low_y:high_y), diff_y-1);
%   cor_4coeff = cor_4coeff + acorf(shadow_4coeff(low_x+j, low_y:high_y), diff_y-1);
%   cor_spline = cor_spline + acorf(shadow_spline(low_x+j, low_y:high_y), diff_y-1);
%   cor_2Dconv = cor_2Dconv + acorf(shadow_2Dconv(low_x+j, low_y:high_y), diff_y-1);
% end
% for j = 1:diff_y
%   cor_random = cor_random + acorf(shadow_random(low_x:high_x, low_y+j)', diff_x-1);
%   cor_4coeff = cor_4coeff + acorf(shadow_4coeff(low_x:high_x, low_y+j)', diff_x-1);
%   cor_spline = cor_spline + acorf(shadow_spline(low_x:high_x, low_y+j)', diff_x-1);
%   cor_2Dconv = cor_2Dconv + acorf(shadow_2Dconv(low_x:high_x, low_y+j)', diff_x-1);
% end
% cor_random = cor_random / (diff_x + diff_y);
% cor_4coeff = cor_4coeff / (diff_x + diff_y);
% cor_spline = cor_spline / (diff_x + diff_y);
% cor_2Dconv = cor_2Dconv / (diff_x + diff_y);

% cd ('/home/ifl/Libertel/Tools/ScenarioGenerator/');

% figure('NumberTitle', 'off', ...
%        'Name', 'Autocorrelation Functions', ...
%        'PaperPositionMode', 'auto',...
%        'Position', [250 350 500 400],...
%        'Pointer', 'arrow');
% % button to create eps-file
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Print',...
% 	  'Position', [2 2 75 30],...
% 	  'Callback', 'print -loose -noui -depsc acorf.eps;');
% % button to exit
% uicontrol('Style', 'pushbutton',...
% 	  'String', 'Exit',...
% 	  'Position', [423 2 75 30],...
% 	  'Callback', 'close;');
% set(gcf, 'DefaulttextInterpreter', 'none');
% set(gcf, 'DefaulttextFontSize', 14);
% set(gca, 'DefaulttextFontSize', 14);
% set(gca, 'Position', [0.125 0.12 0.85 0.83]);

% hold on; 
% grid on;
% box on;

% plot(cor_random, '-kd', 'MarkerFaceColor', 'k', 'MarkerSize', 4);
% %plot(cor_4coeff, '-*', 'MarkerFaceColor', [0 0.5 0], 'MarkerSize', 4, 'Color', [0 0.5 0]);
% plot(cor_spline, '-bs', 'MarkerFaceColor', 'k', 'MarkerSize', 4);
% plot(cor_2Dconv, '-ro', 'MarkerFaceColor', 'k', 'MarkerSize', 4);

% t=(1:100);
% plot(exp(-t*resolution/shadowing_size/20*log(2)), '-k');

% axis([0 50 -0.2 1.0]);
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel')) * resolution/shadowing_size));
% xlabel('\Delta x [m]', 'FontSize', 14, 'Interpreter', 'tex');
% legend('Random Shadowing', 'Spline Interpolation', '2D Covolutional Filter', 'GUDMUNDSON Correlation')

%-------------------------------------------------------------------------------------------%
%------------------------------------------Scenery Map--------------------------------------%
%-------------------------------------------------------------------------------------------%

filename = SceneryMap;

disp(strcat('Preparing ', filename));

% prepare SceneryMap file
scen_file = fopen( filename ,'w');
fprintf(scen_file, 'SceneryMap3\n');
fprintf(scen_file, '%g %g\n', size_x/resolution, size_y/resolution);
fprintf(scen_file, '%g\n', shadowing_size);
fprintf(scen_file, '%g\n', num_bs);
fprintf(scen_file, '%g\n', resolution);

count_y = 0;
active_set1 = zeros(size_y/resolution,size_x/resolution);
active_set2 = zeros(size_y/resolution,size_x/resolution);
temp_pathloss(1, num_bs) = 0;
neighbours = zeros(num_bs, num_bs);

% antenna pattern
ant_pattern = load('741784_1920_CO_P45_06T');

gain_matrix = zeros(361, 181);

for i=1:(181*361)
  gain_matrix(ant_pattern(i,2)+181, ant_pattern(i,1)+1) = ant_pattern(i,3);
end

% for each row (y-value)
for y = resolution/2:resolution:size_y-resolution/2
  count_y=count_y+1;
  count_x=0;
  % for each column (x-value)
  for x = resolution/2:resolution:size_x-resolution/2
    count_x=count_x+1;    
    % writing the morpho-type
    fprintf(scen_file, '%c', 0);

    % writing the shadowing values
    for s_y=1:shadowing_size
      for s_x=1:shadowing_size
	s = round(shadow_2Dconv(round(x/resolution-.5)*shadowing_size+s_x, ...
				round(y/resolution-.5)*shadowing_size+s_y));
	if (s < 0)
	  s = s + 256;
	end
	fprintf(scen_file, '%c', s);
%  	fprintf(scen_file, '%c', 0);
      end
    end

    % calculating the path loss values
    for bs = 1:num_bs
      bs_x_pos = 1000*bs_data(bs,1);
      bs_y_pos = 1000*bs_data(bs,2);
      bs_height = bs_data(bs,3);
    
      % 3D distance
      distance = sqrt((x-bs_x_pos)*(x-bs_x_pos) + ...
		      (y-bs_y_pos)*(y-bs_y_pos) + ...
		      (bs_height-roof_height)*(bs_height-roof_height));
      % 2D distance
      distance2D = sqrt((x-bs_x_pos)*(x-bs_x_pos) + ...
			(y-bs_y_pos)*(y-bs_y_pos));

      % If we are too close, set distance to 1m, this should be enough
      if (distance2D < 1)
	distance2D = 1;
      end
     
      % elongation angle (in x/y-plane)
      if (y < bs_y_pos)
	phi = (180/Pi)*atan((x-bs_x_pos)/(bs_y_pos-y));
      elseif (y == bs_y_pos)
	phi = 90 * sign(x-bs_x_pos);
      else
	phi = (180/Pi)*atan((x-bs_x_pos)/(bs_y_pos-y)) + 180;
      end
      phi = phi - bs_data(bs,4);
      if (phi > 180)
	phi = phi - 360;
      end
      if (phi < -180)
	phi = phi + 360;
      end
      phi_index = floor(phi+180+1);

      % azimut angle (in z over x/y-plane)
      theta = 90 - downtiltangle + (180/Pi)*atan((bs_height-ms_heigth)/distance2D) ;
      theta_index_high = floor(theta+1);
      theta_index_low  = floor(theta);

      if ((phi_index < 1)   | (theta_index_high < 1) | ...
	  (phi_index > 361) | (theta_index_high > 181))
	phi
	phi_index
	theta
	theta_index_high
      end

      % Linear interpolation for direct path
      gain1 = (theta_index_high-theta)*gain_matrix(phi_index, theta_index_low) + ...
	      (theta-theta_index_low)*gain_matrix(phi_index, theta_index_high);

      % Second path with random angle offset dependent on distance
      delta_phi = 10 * exp(-distance/300) * (2*rand-1);
      phi = phi + delta_phi;
      if (phi > 180)
	phi = phi - 360;
      end
      if (phi < -180)
	phi = phi + 360;
      end
      phi_index = floor(phi+180+1);
      
      delta_theta = 10 * exp(-distance/300) * (2*rand-1);
      theta = theta + delta_theta;
      if theta > 180
	theta = 180;
      end
      if theta < 1
	theta = 1;
      end
      theta_index_high = floor(theta+1);
      theta_index_low  = floor(theta);
      gain2 = (theta_index_high-theta)*gain_matrix(phi_index, theta_index_low) + ...
	      (theta-theta_index_low)*gain_matrix(phi_index, theta_index_high);

      % take gain of strongest path
      ant_gain = bs_data(bs,5) + gain1;
      if gain2 > gain1
	ant_gain = bs_data(bs,5) + gain2;
% 	if (distance < 300 & gain2 > gain1 + 3)
% 	  disp('Taking stronger path 2 with offsets:');
% 	  disp([delta_phi delta_theta]);
% 	  disp(strcat('old gain: ', num2str(gain1)));
% 	  disp(strcat('new gain: ', num2str(gain2)));
% 	end
      end
      
%       % Vehicular Test Environment (UMTS 30.03, Sec B.1.4.1.3)
%       % pathloss = (40*(1-0.004*bs_height)*log(distance/1000) - ...
%       %             18*log(bs_height(bs)) + ...
%       %             21*log(frequency)) / log(10) + 80;
%       pathloss = 124.6 + 36.2*log(distance/1000)/log(10);
      
      % Hata-Okumura (Walke: Mobilfunknetze I, p. 50)
      % pathloss = 69.55 + 26.16*log(frequency) - 13.82*log(h_T) - ...
      %            a(h_R) + (44.9 - 6.55*log(h_T))*log(distance/1000);
      pathloss = 126.42 + 35.22*log(distance/1000)/log(10);
      
      % write path loss values
      pathloss = pathloss - ant_gain;
      if pathloss<min_pathloss
	disp('Warning: path loss below minimum!');
	pathloss = min_pathloss;
      end
      if pathloss>max_pathloss
        pathloss = max_pathloss;
      end
      temp_pathloss(1,bs) = pathloss;
      % disp(round(pathloss));
      fprintf(scen_file, '%c', round(pathloss));
      
    end % all BS
  
  end % all column entries (x-values)
  
end % all row entries (y-values)

% end writing
fclose(scen_file);

% F = figure;
% mesh(active_set1)
% view(0,270);
% colormap(gray);
% %set(gco, 'FaceColor', 'CData');
% box on;
% set(gco, 'LineStyle', 'none');
% set(gca, 'FontSize', 14);
% set(get(gca,'YLabel'),'String','y [km]');
% set(get(gca,'XLabel'),'String','x [km]');
% set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel'))*resolution/1000));
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel'))*resolution/1000));
% set(get(gca, 'XLabel'), 'FontSize', 14);
% set(get(gca, 'YLabel'), 'FontSize', 14);
% set(gca, 'Position', [0.1 0.1 0.6 0.8]);
% caxis([1 6]);
% G = figure;
% mesh(active_set2)
% shading flat;
% view(0,270);
% colormap(gray);
% %set(gco, 'FaceColor', 'CData');
% box on;
% set(gco, 'LineStyle', 'none');
% set(gca, 'FontSize', 14);
% set(gca, 'FontSize', 14);
% set(get(gca,'YLabel'),'String','y [km]');
% set(get(gca,'XLabel'),'String','x [km]');
% set(gca, 'YTickLabel', num2str(str2num(get(gca, 'YTickLabel'))*resolution/1000));
% set(gca, 'XTickLabel', num2str(str2num(get(gca, 'XTickLabel'))*resolution/1000));
% set(get(gca, 'XLabel'), 'FontSize', 14);
% set(get(gca, 'YLabel'), 'FontSize', 14);
% set(gca, 'Position', [0.1 0.1 0.6 0.8]);
% shading flat;
% caxis([1 6]);
% % end % all downtiltangles
