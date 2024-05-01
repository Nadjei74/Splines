% Read the data files containing the initial and rotated curve points
initialCurveData = importdata('nurbs_curve_initial.txt');
rotatedCurveData = importdata('nurbs_curve_rotated.txt');

% Plot both curves in a single plot
figure;
plot3(initialCurveData(:,1), initialCurveData(:,2), initialCurveData(:,3), 'r-', 'LineWidth', 2);
hold on;
plot3(rotatedCurveData(:,1), rotatedCurveData(:,2), rotatedCurveData(:,3), 'b-', 'LineWidth', 2);
hold off;
xlabel('X');
ylabel('Y');
zlabel('Z');
title('Initial and Rotated NURBS Curves');
grid on;
axis equal;
legend('Initial Curve', 'Rotated Curve');
