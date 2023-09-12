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
load('ArmData.mat')

% set up subplot style
figure('units','normalized','outerposition',[0 0 1 1]);
vPlot = 3;
hPlot = 2;
stuNo = "10611816: ";

%% Task 2, Part 1
%{
Build a multi-layer perceptron in MATLAB from first principles
    - 2 layers
        - a sigmoid hidden layer 
        - a linear output layer

    You will first need to implement the 2-layer network feedforward pass (with nh
    hidden units that can be specified as a parameter, and a linear output). Given
    the weight matrix, this will generate the network output activation for a given
    input vector
    Then implement backpropagation to train all the weights.
    Make sure you initialize the weights in the network appropriately before running
    the iterative training.
%}

% Implement the 2 layer network feed forward pass
% - sigmoid hidden layer 
% - linear output layer

data = endpoint(:, 1:1000); % 2x1000;
target = theta(:, 1:1000);  % 2x1000;

% initialise input and output values
X = [data; ones(1, length(data))];   % training data augmented with 1 = 3x1000

% hidden nodes
hN = 10;
[numRowsX,numColsX] = size(X);

% initialise weights
W1 = rand(hN, numRowsX) - 0.5;
W2 = rand(2, hN+1) - 0.5;

a = 0.0003;     % learning rate
i = 10000;      % iterations

%% Task 2 Part 2
%{
    Now you are required to train your network to perform the inverse kinematics of the planar arm using the 2-layer neural network you developed above. 
    Using the dataset, you generated in section 1.
        - Train your linear output 2-layer network with the augmented end effect positions as input, and the two joint angles as outputs.
        - Plot the error as training proceeds.
%}
% run network training
[output, W1, W2, e] = TwoLayerNetworkTraining(hN, a, i, X, W1, W2, target);

%% Plot endpoint positions and arm origin position
subplot(vPlot, hPlot, 1);
hold on;
plot(endpoint(1,1:1000), endpoint(2,1:1000), 'r.');
plot(origin, origin, 'k.', 'MarkerSize', 20);
title(stuNo + "Arm Endpoints");
xlabel("x (m)");
ylabel("y (m)");
legend( "Endpoint", "Origin");
axis("equal");

%% Plot error
subplot(vPlot, hPlot, 2);
hold on;
plot(e, 'g.');
title(stuNo + "Error Curve");
xlabel("Iterations");
ylabel("Error");

% min
[val0,idx0] = min(e) ;
plot(10000,e(idx0),'*r')
%text(10000, e(idx0), e(idx0))

%% Task 2 Part 3
subplot(vPlot, hPlot, 3);
plot(X(1,:), X(2,:), 'b.');
title(stuNo + "Random Endpoint Data");
xlabel("x coordinate");
ylabel("y coordinate");
axis("equal");

subplot(vPlot, hPlot, 4);
plot(target(1,:), target(2,:), 'b.');
title(stuNo + "Random Joint Angle Data");
xlabel("theta 1");
ylabel("theta 2");

[P1, P2] = RevoluteForwardKinematics2D(armLen, output, origin);

subplot(vPlot, hPlot, 5);
plot(P2(1, :), P2(2, :), 'r.');
title(stuNo + "Regenerated Endpoint Data");
xlabel("x coordinate");
ylabel("y coordinate");
axis("equal");

subplot(vPlot, hPlot, 6);
plot(output(1,:), output(2,:), 'r.');
title(stuNo + "Inverse Model Joint Angle Data");
xlabel("theta 1");
ylabel("theta 2");
