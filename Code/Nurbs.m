% Read NURBS curve points from file
curveData = importdata('nurbs_curve.txt');
curveX = curveData(:, 1);
curveY = curveData(:, 2);

% Plot NURBS curve
figure;
plot(curveX, curveY, 'b', 'LineWidth', 2);
title('NURBS Curve');
xlabel('X');
ylabel('Y');
grid on;
xlim([min(curveX) - 1, max(curveX) + 1]); % Adjust x-axis range

% Read NURBS surface points from file
surfaceData = importdata('nurbs_surface.txt');
surfaceX = surfaceData(:, 1);
surfaceY = surfaceData(:, 2);
surfaceZ = surfaceData(:, 3);

% Plot 3D surface
figure;
scatter3(surfaceX, surfaceY, surfaceZ, 'r', 'filled');
title('Revolved NURBS Surface');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;
xlim([min(surfaceX) - 1, max(surfaceX) + 1]); % Adjust x-axis range
