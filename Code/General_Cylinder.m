
fid = fopen('General Cylinder.txt', 'r');
if fid == -1
    error('Unable to open file');
end

% Read the data from the file
data = fscanf(fid, '(%f, %f, %f)\n', [3, Inf]);
fclose(fid);

% Transpose the data matrix
data = data';

% Plot the points
scatter3(data(:, 1), data(:, 2), data(:, 3), 'filled');
xlabel('X');
ylabel('Y');
zlabel('Z');
title('Ruled Surface');

% Optionally, connect the points to visualize the surface
hold on;
plot3(data(:, 1), data(:, 2), data(:, 3));
hold off;
