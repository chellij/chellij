% P1 Generate Arm Data 2022 - 10611816
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% all rights reserved
% Author: Dr. Ian Howard
% Associate Professor (Senior Lecturer) in Computational Neuroscience
% Centre for Robotics and Neural Systems
% Plymouth University
% 116 Reynolds
% PL4 8AA
% Plymouth, Devon, UK
% howardlab.com
% 21/09/2022
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Generate arm data to train inverse model
% you need to implement this script to run the assignment section 1`
close all
clear all
clc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% your script from here onwards
stuNo = "10611816: ";

%% TASK 1 [3 Marks]
%{
    • Arm lengths = 0.4m
    • Arm base origin = (0, 0)
    • Generate TEST and TRAINING 1000 sample uniform datasets - range = 0 to pi 
    • Run the RevoluteForwardKinematics2D generating the endpoint locations
    • Plot the endpoint positions and the arm origin position too
    • Discuss the useful range of this arm.
%}

% Setup Variables
armLen = [0.4 0.4];
origin = [0 0];
samples = 1000;
ang = [0 pi];

% Generate Datasets
% y = offset + range * rand(array size)
theta1 = ang(1) + (ang(2)-ang(1)).*rand(2, samples);
theta2 = ang(1) + (ang(2)-ang(1)).*rand(2, samples);

% Run forward kinematics function on both datasets
[P1_train, P2_train] = RevoluteForwardKinematics2D(armLen, theta1, origin);
[P1_test, P2_test] = RevoluteForwardKinematics2D(armLen, theta2, origin);

save("P1_workspace.mat")
%% Plot endpoint positions and arm origin position
endpoints = figure;
hold on;
plot(P2_train(1,:), P2_train(2,:), 'g.');
plot(origin, origin, 'k.', 'MarkerSize', 20);
title(stuNo + "Arm Endpoints");
xlabel("x (m)");
ylabel("y (m)");
legend( "Endpoint", "Origin");

%% Save data to use elsewhere
saveas(endpoints, "figures/Task1_EndPointPositions.png")