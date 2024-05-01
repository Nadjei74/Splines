% % Read the interpolated points from the file
fid = fopen('bspline.txt', 'r');
data = textscan(fid, '(%f, %f)');
fclose(fid);
% 
% % Extract x and y coordinates from the data
x = data{1};
y = data{2};
% 
% % Plot the interpolated points
plot(x, y, 'b-', 'LineWidth', 2);
hold on;
scatter(x, y, 50, 'r', 'filled'); % Scatter plot of points
hold off;
% 
% Customize the plot
title('B-spline Interpolation');
xlabel('X-axis');
ylabel('Y-axis');
grid on;
legend('B-spline Curve', 'Interpolated Points');


% fid = fopen('bspline.txt', 'r');
% if fid == -1
%     error('Unable to open file');
% end
% 
% % Read the data from the file
% data = fscanf(fid, '(%f, %f, %f)\n', [3, Inf]);
% fclose(fid);
% 
% % Transpose the data matrix
% data = data';
% 
% % Plot the points
% %scatter3(data(:, 1), data(:, 2), data(:, 3), 'filled');
% xlabel('X');
% ylabel('Y');
% zlabel('Z');
% title('Ruled Surface');
% 
% % Optionally, connect the points to visualize the surface
% hold on;
% plot3(data(:, 1), data(:, 2), data(:, 3));
% hold off;


