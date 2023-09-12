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

% Find path through maze
% you need to expand this script to run the assignment section 3

close all
clear all
clc

stuNo = "10611816: ";

% These define the axis of the maze
limits = [-0.7 -0.2; -0.3 0.2;];

% build the maze
maze = CMazeMaze11x11(limits);

% draw the maze
p = maze.DrawMaze();

%% Task 3, Part 1: Random start state
% test random start and display histogram
for i = 1:1000
    [randomStartState(i)] = maze.RandomStartingState();
end

hist = figure;
hold on
histogram(randomStartState, 121);
title(stuNo + "Histogram of starting states")
xlabel("starting state")
ylabel("number of occurances")
%% Task 3, Part 2: Build a reward function [3 marks]
% example
s = 7;
a = 2;
reward = maze.RewardFunction(s, a);

%% Task 3, Part 3: Generate the transition matrix [3 marks]
maze = maze.BuildTransitionMatrix();
disp("Transition Matrix")
maze.tm

%% Task 3, Part 4: Init Q Values [3 marks]
minVal = 0.001;
maxVal = 0.1;
maze = maze.InitQTable(minVal, maxVal);
disp("Initial QValues")
maze.QValues

%% Task 3, Part 5/6: Implement Q-Learning Algorithm [15 marks]
alpha = 0.2;   % learning rate
gamma = 0.9;   % temporal discount rate
epsilon = 0.1; % exploration rate

[maze, stepsEp] = maze.Episode(alpha, gamma, epsilon);

%% Task 3, Part 6: Run Q Learning Trial 
episodes = 1000;
[maze, stepsTr] = maze.Trial(episodes, alpha, gamma, epsilon);

% plot mean and standard deviation across trials of the 
% stepts takenn against episodes
stepsEpisode = figure;
hold on
plot(stepsTr)
title(stuNo + "Q-Learning performance improvement")
xlabel("episode")
ylabel("steps")

%% Task 3, Part 6: Run one experiment of 100 trials
%%{
trials = 100;
[maze, stepsEx] = maze.Experiment(episodes, trials, alpha, gamma, epsilon);

for i = 1:trials
    mean_data(i) = mean(stepsEx(i,:));
    std_data(i) = std(stepsEx(i,:));
end

errorPlot = figure;
hold on
errorbar(mean_data, std_data);
title(stuNo + "Q-Learning in operation across multiple trials");
xlabel("Episode Number");
ylabel("Number of steps");
saveas(errorPlot,'figures/Task3_errorAcrossTrials.png')
%}

%% Task 3, Part 7: Exploitation of Q-Values
% run an episode with no exploration and record the visited states
fprintf("Learning Complete.\n\nExploitation Starting:\n")

epsilon = 0;
path = maze.QExploit(epsilon);

% variables to scale the path down to maze size
sX = (maze.limitsXY(1,2) - maze.limitsXY(1,1)) / maze.xStateCnt;
sY = (maze.limitsXY(2,2) - maze.limitsXY(2,1)) / maze.yStateCnt;

% variables to centre the path on the maze
cX = (maze.cursorSizeX - maze.limitsXY(1,1));
cY = (maze.cursorSizeY - maze.limitsXY(2,1));
 
for i = 1:length(path)
   scalePath(1, i) = path(1, i) * sX - cX;
   scalePath(2, i) = path(2, i) * sY - cY;
end

% plot the path taken over the map
figure(p)
hold on
plot(scalePath(1,:), scalePath(2,:), 'm+-', 'MarkerSize', 10, 'LineWidth', 5)

%% Save plots
saveas(p,'figures/Task3_plottedMazewithPath.png')
saveas(hist,'figures/Task3_histogram.png')
saveas(stepsEpisode,'figures/Task3_stepsInEpisode.png')

%% Save items for Task 4
save("P3_workspace", "maze", "scalePath", "sX", "sY", "cX", "cY")