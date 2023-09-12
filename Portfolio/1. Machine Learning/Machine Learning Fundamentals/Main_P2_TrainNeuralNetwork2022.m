% P2 Train Neural Network - 10611816
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% all rights reserved
% Author: Dr. Ian Howard
% Associate Professor (Senior Lecturer) in Computational Neuroscience
% Centre for Robotics and Neural Systems
% Plymouth University
% 116 Reynolds
% PL4 8AA
% Plymouth, Devon, UK
%                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
% howardlab.com
% 21/09/2022
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Train a neural network to implement inverse model
% you need to implement this script to run the assignment section 2

close all
clear all
clc

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% your script from here onwards

% set up subplot style
g = figure('units','normalized','outerposition',[0 0 1 1]);

%% Task 2, Part 1
%{
    • Multi-layer perceptron in MATLAB from first principles
    - 2 layers
        - a sigmoid hidden layer 
        - a linear output layer

    • Implement the 2-layer network feedforward pass to generate the 
      network output activation.
    • Implement backpropagation to train all the weights.
    • Initialize the weights appropriately before running the iterative training
%}
disp("P2 Task 1 running...")
% Load P1 datasets
load('P1_workspace.mat')

%specify the input data and the target data
data = [P2_train; ones(1, length(P2_train))];
target = theta1;

% specify the number of hidden nodes
hN = 12;

% specify learning rate and iterations 
learning_rate = 0.00025;     
iterations = 12000;   

% initialise weights according to size of input data and number of nodes
[numRowsX, numColsX] = size(data); 
W1 = rand(hN, numRowsX) - 0.5;
W2 = rand(2, hN+1) - 0.5; 

% TwoLayerNetworkTraining.m function created as part of this task
fprintf("    Hidden Nodes: %d \n    Learning Rate: %f \n    Iterations: %d\n\n", hN, learning_rate, iterations)

%% Task 2, Part 2
%{
    • Train your network to perform the inverse kinematics of the planar arm
        • Use the 2-layer neural network you developed above. 
        • Use the dataset generated in section 1
            INPUTS: Augmented end effect positions
            OUTPUTS: Two joint angles
    • Plot the error as training proceeds.
%}

disp("P2 Task 2 running...")
% run network training
[output, W1, W2, e] = TrainingTwoLayerNetwork(hN, learning_rate, iterations, data, W1, W2, target);

fprintf("    Final Error: %f\n    Iterations Used: %d/%d\n\n", e(end), length(e), iterations);

% Plot 1: Error Curve
subplot(3, 2, 1);
hold on;
plot(e, 'g.-');
title(stuNo + "Error Curve");
xlabel("Iterations");
ylabel("Error");

%% Task 2, Part 3 
%{
    Test your inverse model looking at how the 2-link arm endpoint is mapped back to itself.
    • Run a forward network pass on the TEST position data to generate predicted arm angles.
    • Run the forward kinematics of these predicted angles and generate the corresponding end points
    • Plot the joint angles and endpoints and compare with those you started with.
%}

disp("P2 Task 3 running...")

% Plot 2: Test Data - Random Endpoint Data
subplot(3, 2, 3);
hold on
plot(P2_test(1,:), P2_test(2,:), 'b.');
plot(origin, origin, 'k.', 'MarkerSize', 20);
title(stuNo + "Random Endpoint Data");
xlabel("x coordinate");
ylabel("y coordinate");
legend( "Endpoint", "Origin");
axis("equal");

% Plot 3: Test Data - Random Target Data
subplot(3, 2, 4);
hold on
plot(theta2(1,:), theta2(2,:), 'b.');
title(stuNo + "Random Joint Angle Data");
xlabel("theta 1");
ylabel("theta 2");


% run trained two layer network to generate predicted arm angles
test_data = [P2_test; ones(1, length(P2_test))];
[output_test] = TrainedTwoLayerNetwork(test_data, W1, W2); % output_test = 2x1000

% run forward kinematics to get end points
[P1_inverse, P2_inverse] = RevoluteForwardKinematics2D(armLen, output_test, origin);

% Plot 4: Test Data - Regenerated Endpoint Data
subplot(3, 2, 5);
hold on
plot(P2_inverse(1, :), P2_inverse(2, :), 'r.');
plot(origin, origin, 'k.', 'MarkerSize', 20);
title(stuNo + "Regenerated Endpoint Data");
xlabel("x coordinate");
ylabel("y coordinate");
legend( "Endpoint", "Origin");
axis("equal");

% Plot 5: Test Data - Regenerated Joint Angles
subplot(3, 2, 6);
hold on
plot(output_test(1,:), output_test(2,:), 'r.');
title(stuNo + "Inverse Model Joint Angle Data");
xlabel("theta 1");
ylabel("theta 2");

save("P2_workspace.mat", "W1", "W2", "output");
saveas(g, "figures/Task2_allPlots.png")

disp("    Done")