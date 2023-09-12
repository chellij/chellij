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
%% TASK 1
% Setup Variables
armLen = [0.4 0.4];
origin = [0 0];

samples = 1000;
ang = [0 pi];

% y = offset + range * rand(array size)
dataset1 = ang(1) + (ang(2)-ang(1)).*rand(2, samples);
dataset2 = ang(1) + (ang(2)-ang(1)).*rand(2, samples);

theta = [dataset1 dataset2];

% Generate Data
[elbowjoint, endpoint] = RevoluteForwardKinematics2D(armLen, theta, origin);

%% Plot endpoint positions and arm origin position
figure;
hold on;
plot(endpoint(1,1:1000), endpoint(2,1:1000), 'g.');
plot(origin, origin, 'k.', 'MarkerSize', 20);
title("10611816: Arm Endpoints");
xlabel("x (m)");
ylabel("y (m)");
legend( "Endpoint", "Origin");

%% Save data to use elsewhere
save("ArmData")

