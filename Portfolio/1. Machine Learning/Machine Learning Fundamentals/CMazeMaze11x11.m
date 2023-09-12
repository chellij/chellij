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

classdef CMazeMaze11x11
    % define maze work for RL
    % detailed explanation goes here

    properties

        % parameters for the gmaze grid management
        arrowPoint;
        blockedLocations;
        cursorCentre;
        limitsXY;
        xStateCnt
        yStateCnt;
        stateCnt;
        stateNumber;
        totalStateCnt
        squareSizeX;
        cursorSizeX;
        squareSizeY;
        cursorSizeY;
        stateOpen;
        statePath;
        stateStart;
        stateStartID;
        stateEnd;
        stateEndID;
        stateX;
        stateY;
        xS;
        yS
        stateLowerPoint;
        textLowerPoint;
        stateName;
        plottedMaze;

        % parameters for Q learning
        QValues;
        tm;
        actionCnt;
    end

    methods

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % constructor to specity maze
        function f = CMazeMaze11x11(limitsXY)

            % set scaling for display
            f.limitsXY = limitsXY;
            f.blockedLocations = [];

            % setup actions
            f.actionCnt = 4;

            % build the maze
            f = SimpleMaze11x11(f);

            % display progress
            disp(sprintf('Building Maze CMazeMaze11x11'));

        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % build the maze
        function f = SetMaze(f, xStateCnt, yStateCnt, startLocation, blockedLocations, endLocation)

            % set size
            f.xStateCnt=xStateCnt;
            f.yStateCnt=yStateCnt;
            f.stateCnt = xStateCnt*yStateCnt;

            % compute state countID
            for x =  1:xStateCnt
                for y =  1:yStateCnt

                    % get the unique state identified index
                    ID = x + (y -1) * xStateCnt;

                    % record it
                    f.stateNumber(x,y) = ID;

                    % also record how x and y relate to the ID
                    f.stateX(ID) = x;
                    f.stateY(ID) = y;
                end
            end

            % calculate maximum number of states in maze
            % but not all will be occupied
            f.totalStateCnt = f.xStateCnt * f.yStateCnt;


            % get cell centres
            f.squareSizeX= 1 * (f.limitsXY(1,2) - f.limitsXY(1,1))/f.xStateCnt;
            f.cursorSizeX = 0.5 * (f.limitsXY(1,2) - f.limitsXY(1,1))/f.xStateCnt;
            f.squareSizeY= 1 * (f.limitsXY(2,2) - f.limitsXY(2,1))/f.yStateCnt;
            f.cursorSizeY = 0.5 * (f.limitsXY(2,2) - f.limitsXY(2,1))/f.yStateCnt;

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            % init maze with no closed cell
            f.stateOpen = ones(xStateCnt, yStateCnt);
            f.stateEnd = endLocation;
            f.stateEndID = f.stateNumber(f.stateEnd(1),f.stateEnd(2));

            % put in blocked locations
            for idx = 1:size(blockedLocations,1)
                bx = blockedLocations(idx,1);
                by = blockedLocations(idx,2);
                f.stateOpen(bx, by) = 0;
            end
            f.blockedLocations = blockedLocations;

            % specify start location in (x,y) coordinates
            f.stateStart = startLocation;
            f.stateStartID = f.stateNumber(startLocation(1), startLocation(2));


            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % get locations for all states
            for x=1:xStateCnt
                for y=1:yStateCnt

                    % start at (0,0)
                    xV = x-1;
                    yV = y-1;

                    % pure scaling component
                    % assumes input is between 0 - 1
                    scaleX = (f.limitsXY(1,2) - f.limitsXY(1,1)) / xStateCnt;
                    scaleY = (f.limitsXY(2,2) - f.limitsXY(2,1)) / yStateCnt;

                    % remap the coordinates and add on the specified orgin
                    f.xS(x) = xV  * scaleX + f.limitsXY(1,1);
                    f.yS(y) = yV  * scaleY + f.limitsXY(2,1);

                    % remap the coordinates, add on the specified orgin and add on half cursor size
                    f.cursorCentre(x,y,1) = xV * scaleX + f.limitsXY(1,1) + f.cursorSizeX/2;
                    f.cursorCentre(x,y,2) = yV * scaleY + f.limitsXY(2,1) + f.cursorSizeY/2;

                    f.stateLowerPoint(x,y,1) = xV * scaleX + f.limitsXY(1,1);  - f.squareSizeX/2;
                    f.stateLowerPoint(x,y,2) = yV * scaleY + f.limitsXY(2,1); - f.squareSizeY/2;

                    f.textLowerPoint(x,y,1) = xV * scaleX + f.limitsXY(1,1)+ 10 * f.cursorSizeX/20;
                    f.textLowerPoint(x,y,2) = yV * scaleY + f.limitsXY(2,1) + 10 * f.cursorSizeY/20;
                    
                    f.arrowPoint(x,y,1) = xV * scaleX + f.limitsXY(1,1)+ 20 * f.cursorSizeX/20;
                    f.arrowPoint(x,y,2) = yV * scaleY + f.limitsXY(2,1) + 20 * f.cursorSizeY/20;
                    
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % draw rectangle
        function DrawSquare(f, pos, faceColour)
            % Draw rectagle
            rectangle('Position', pos,'FaceColor', faceColour,'EdgeColor','k', 'LineWidth', 3);
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % draw circle
        function DrawCircle(f, pos, faceColour)
            % Draw circle
            rectangle('Position', pos,'FaceColor', faceColour,'Curvature', [1 1],'EdgeColor','k', 'LineWidth', 3);
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % draw the maze
        function p = DrawMaze(f)
            p = figure('Name', 'plottedMaze', 'position', [50, 50, 900, 550]); %left, bottom , width, height
            fontSize = 12;
            hold on
            h=title(sprintf('10611816: Maze wth %d x-axis X %d y-axis cells', f.xStateCnt, f.yStateCnt));
            set(h,'FontSize', fontSize);

            for x=1:f.xStateCnt
                for y=1:f.yStateCnt
                    pos = [f.stateLowerPoint(x,y,1)  f.stateLowerPoint(x,y,2)  f.squareSizeX f.squareSizeY];

                    % if location open plot as blue
                    if(f.stateOpen(x,y))
                        DrawSquare( f, pos, 'b');
                        % otherwise plot as black
                    else
                        DrawSquare( f, pos, 'k');
                    end
                end
            end

            % put in start locations
            for idx = 1:size(f.stateStart,1)
                % plot start
                x = f.stateStart(idx, 1);
                y = f.stateStart(idx, 2);
                pos = [f.stateLowerPoint(x,y,1)  f.stateLowerPoint(x,y,2)  f.squareSizeX f.squareSizeY];
                DrawSquare(f, pos,'g');
            end

            % put in end locations
            for idx = 1:size(f.stateEnd,1)
                % plot end
                x = f.stateEnd(idx, 1);
                y = f.stateEnd(idx, 2);
                pos = [f.stateLowerPoint(x,y,1)  f.stateLowerPoint(x,y,2)  f.squareSizeX f.squareSizeY];
                DrawSquare(f, pos,'r');
            end

            % put on names
            for x=1:f.xStateCnt
                for y=1:f.yStateCnt
                    sidx=f.stateNumber(x,y);
                    stateNameID = sprintf('%s', f.stateName{sidx});
                    text(f.textLowerPoint(x,y,1),f.textLowerPoint(x,y,2), stateNameID, 'FontSize', fontSize)
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % setup 11x11 maze
        function f = SimpleMaze11x11(f)

            xCnt=11;
            yCnt=11;
            % specify end location in (x,y) coordinates
            endLocation = [11, 11];

            % specify blocked location in (x,y) coordinates
            blocks = 33;
            blockedLocations = [1, 2; 1, 3; 1, 7; 1,10;
                2, 2; 2, 3; 2, 9; 2,10;
                3, 8;
                4, 1; 4, 5; 4, 7; 4,10;
                5, 3; 5, 8;
                6, 1; 6, 4; 6, 6; 6, 9; 6,11;
                7, 6;
                8, 2; 8, 4; 8, 5; 8, 7;
                9, 2; 9, 9;
                10, 2;10, 7; 10,10;
                11, 1;11, 6; 11,10;
                ];
            
            startLocation = [1,1];
            
            % build the maze
            f = SetMaze(f, xCnt, yCnt, startLocation, blockedLocations, endLocation);

            % write the maze state
            maxCnt = xCnt * yCnt;
            for idx = 1:maxCnt
                f.stateName{idx} = num2str(idx);
            end

        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % reward function that takes a stateID and an action
        function reward = RewardFunction(f, stateID, action)

            % init to no reward
            reward = 0;
            if (stateID == f.stateEndID-1) && (action == 2)
                reward = 10;
            else
                reward = 0;
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % function  computes a random starting state
        function [state] = RandomStartingState(f)
            
            while(1)
                startLocation = [randi(f.xStateCnt), randi(f.yStateCnt)];
                x = startLocation(1);
                y = startLocation(2);
                if f.stateOpen(x,y) == 1
                    break;
                end
            end
            state = f.stateNumber(startLocation(1), startLocation(2));
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % look for end state
        function endState = IsEndState(f, s)

            endState = 0;
            
            if(f.stateEndID == s)
                endState = 1;
            else
                endState = 0;
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % init the q-table
        function f = InitQTable(f, minVal, maxVal)
            
            offset = minVal;
            range = maxVal - minVal;
            f.QValues = zeros(f.xStateCnt * f.yStateCnt, f.actionCnt);

            % YOUR CODE GOES HERE ...
            for s = 1:(f.xStateCnt * f.yStateCnt)
                for a = 1:f.actionCnt
                    f.QValues(s, a) = offset + range.*rand();
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % % build the transition matrix
        function f = BuildTransitionMatrix(f)

            north = 1;
            east = 2;
            south = 3;
            west = 4;

            % allocate
            f.tm = zeros(f.xStateCnt * f.yStateCnt, f.actionCnt);

            % YOUR CODE GOES HERE ....
            for x = 1:f.xStateCnt
                for y = 1:f.yStateCnt
                    currentPos = f.stateNumber(x, y);
                    tmRow = x-11+y*11;
                    if (f.stateOpen(x, y) == 0)
                        f.tm(tmRow, north) = currentPos;
                        f.tm(tmRow, west) = currentPos;
                        f.tm(tmRow, south) = currentPos;
                        f.tm(tmRow, east) = currentPos;
                    else                    
                        for dir = 1:4
                            if dir == 1
                                % NORTH
                                UP = y+1;
                                if (UP ~= 12)
                                    if (f.stateOpen(x, UP) == 0)           % 0 = blocked, 1 = open
                                        f.tm(tmRow, north) = currentPos;
                                    else
                                        f.tm(tmRow, north) = f.stateNumber(x, UP);
                                    end
                                else
                                    f.tm(tmRow, north) = currentPos;
                                end
                            elseif dir == 2
                                DOWN = y-1;
                                % SOUTH
                                if (DOWN ~= 0)
                                    if (f.stateOpen(x, DOWN) == 0)
                                        f.tm(tmRow, south) = currentPos;
                                    else
                                        f.tm(tmRow, south) = f.stateNumber(x, DOWN);
                                    end
                                else
                                    f.tm(tmRow, south) = currentPos;
                                end
                            elseif dir == 3
                                LEFT = x+1;
                                % EAST
                                if (LEFT ~= 12)
                                    if (f.stateOpen(LEFT, y) == 0)
                                        f.tm(tmRow, east) = currentPos;
                                    else
                                        f.tm(tmRow, east) = f.stateNumber(LEFT, y);
                                    end
                                else
                                    f.tm(tmRow, east) = currentPos;
                                end
                            elseif dir == 4
                                RIGHT = x-1;
                                % WEST
                                if (RIGHT ~= 0)
                                    if (f.stateOpen(RIGHT, y) == 0)
                                        f.tm(tmRow, west) = currentPos;
                                    else
                                        f.tm(tmRow, west) = f.stateNumber(RIGHT, y);
                                    end
                                else
                                    f.tm(tmRow, west) = currentPos;
                                end
                            end
                        end

                    end
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Q Table update
        function QT = QTableUpdate(f, state, action, nextState, reward, alpha, gamma)
            
            QT = f.QValues;
            maxV = max(QT(nextState,:)); %maximum value in next state

            % Q(St, At) = Q(St,At) + alpha(reward + gamma(max value in next state) - previous Q)
            QT(state, action) = QT(state, action) + alpha*(reward + gamma*(maxV) - QT(state, action));
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Greedy Function
        function action = ActionSelection(f, state, epsilon)

            if rand(1) <= epsilon
                action = randi(4);
            else
                [~,action] = max(f.QValues(state, :));
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Episode
        function [f, steps] = Episode(f, alpha, gamma, epsilon)
            % implements a Q-learning episode loop that runs from random start state
            % until the goal state is reached
            running=1;
            steps=0;
            [state] = RandomStartingState(f);
            while(running==1)
                action = ActionSelection(f, state, epsilon); % Greedy Action
                nextState = f.tm(state, action);             % next state from action
                reward = RewardFunction(f, state, action);   % reward at next state

                % update Q Table using this info
                f.QValues = QTableUpdate(f, state, action, nextState, reward, alpha, gamma);
                % continue or stop?
                if IsEndState(f, nextState)
                    running=0;
                else
                    steps = steps+1;
                    state = nextState;
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Trial
        function [f, stepsAcrossEpisodes] = Trial(f, episodes, alpha, gamma, epsilon)
            % trial consists of multiple episodes
            % trial starts with naïve Q-values
            % the algorithm learns throughout a trial
           
            f = InitQTable(f, 0.01, 0.1);

            for idx = 1:episodes
                [f, stepsAcrossEpisodes(idx)] = Episode(f, alpha, gamma, epsilon);
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Experiment
        function [f, stepsAcrossTrials] = Experiment(f, episodes, trials, alpha, gamma, epsilon)
            % experiment involves running a given number of trials
            for trial = 1:trials
                [f, stepsAcrossTrials(:, trial)] = Trial(f, episodes, alpha, gamma, epsilon);
                fprintf("Trial %d finished\n", trial)
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % QAnalysis
        function [path] = QExploit(f, epsilon)
            steps = 1;
            running = 1;
            state = f.stateNumber(1, 1);
            fprintf("\nStart state is %d\nTermination state is %d\nRunning analysis on maze...\n\n", state, f.stateEndID)

            while(running==1)
                action = ActionSelection(f, state, epsilon); % Greedy Action
                nextState = f.tm(state, action);             % Next state from action
                visitedStates(steps) = state;
                
                % continue or stop?
                if IsEndState(f, nextState)
                    visitedStates(steps+1) = nextState;
                    running=0;
                else
                    steps = steps+1;
                    state = nextState;
                end
            end
            fprintf("Q analysis finished\nNumber of steps taken was %d\n", steps)

            path = zeros(2, steps);
            for i = 1:steps+1
                [path(1, i), path(2, i)] = find(f.stateNumber == visitedStates(i));
            end
        end
    end
end
