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

% Move arm through maze
% you need to implement this script to run the assignment section 4

close all
clear all
clc

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% your script from here onwards
load("P1_workspace.mat")
load("P2_workspace.mat")
load("P3_workspace.mat")

axisX = [maze.limitsXY(1, 1) - 0.1, 0.1];
axisY = [maze.limitsXY(2, 1) - 0.1, maze.limitsXY(2,2) + 0.1];

% build the maze
p = figure('position', [50, 50, 900, 550]);
hold on
set(gca,'Color','k')
title(sprintf(stuNo + "Revolute arm moving along path in maze", maze.xStateCnt, maze.yStateCnt));
axis([axisX(1) axisX(2) axisY(1) axisY(2)])
for x=1:maze.xStateCnt
    for y=1:maze.yStateCnt
        pos = [maze.stateLowerPoint(x,y,1),  maze.stateLowerPoint(x,y,2),  maze.squareSizeX, maze.squareSizeY];
        % if location open plot as blue
        if(maze.stateOpen(x,y))
            maze.DrawSquare(pos, 'b');
            % otherwise plot as black
        else
            maze.DrawSquare(pos, 'k');
        end
    end
end

figure(p)
plot(scalePath(1,:), scalePath(2,:), 'm.-', 'MarkerSize', 8);
plot(origin(1), origin(2), 'g*', 'MarkerSize', 8);

data = [scalePath; ones(1, length(scalePath'))];

%% Animate Arm Moving

mov(1:30) = struct('cdata', [], 'colormap', []);

theta = TrainedTwoLayerNetwork(data, W1, W2);
[P1, P2] = RevoluteForwardKinematics2D(armLen, theta, origin);

video = VideoWriter('figures/Task4Animation.avi');
video.FrameRate=2;
open(video);

for idx = 1:length(scalePath')
    plot(P1(1, idx), P1(2, idx), 'ro', 'MarkerSize', 4);
    plot(P2(1, idx), P2(2, idx), 'co', 'MarkerSize', 4);
    plot([origin(1), P1(1, idx)], [origin(2),P1(2, idx)], 'b.-');
    plot([P1(1, idx), P2(1, idx)], [P1(2, idx),P2(2, idx)], 'b.-');
    pause(1);
    writeVideo(video, getframe(gcf));
end
saveas(p, "figures/Task4_RevoluteArm.png");
close(video);
