
% Read data from the first file
fileID1 = fopen('curve1.txt','r');
data1 = textscan(fileID1, '(%f, %f, %f)');
fclose(fileID1);
data1 = cell2mat(data1);

% Read data from the second file
fileID2 = fopen('curve2.txt','r');
data2 = textscan(fileID2, '(%f, %f, %f)');
fclose(fileID2);
data2 = cell2mat(data2);

% Ensure both data sets have the same number of points
if size(data1, 1) ~= size(data2, 1)
    error('The number of points in each curve does not match');
end

% Plotting the points and connecting lines
figure;
hold on;
grid on;  % Enable grid
axis equal;  % Equal scaling on all axes
xlabel('X');
ylabel('Y');
zlabel('Z');
title('3D Connection of Corresponding Points Between Two Curves');

for i = 1:size(data1, 1)
    % Plotting points for each curve
    plot3(data1(i, 1), data1(i, 2), data1(i, 3), 'bo');  % Blue dots for curve 1
    plot3(data2(i, 1), data2(i, 2), data2(i, 3), 'ro');  % Red dots for curve 2

    % Drawing a line between corresponding points
    plot3([data1(i, 1), data2(i, 1)], [data1(i, 2), data2(i, 2)], [data1(i, 3), data2(i, 3)], 'k-');
end

view(3);  % Ensure the plot is viewed in 3D
hold off;

