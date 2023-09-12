% Neural Network
 
% add plotting command - plot as discrete data point (points should
% converge to a straight line

clc;
clear;
close all;

format long

% define the input
input = rand(20000,1);

% define the bias values
bias = [0, 0];

% define the correct output matrix
correct_Output = 2*(input);

% define the sample size
sample_Size_Total = 64;
sample_Size = 64;

% c_Weight is used to store the gradients, error_Total is defined
c_Weight = [];
error_Total = 0;

% define  the length of the epoch
epoch_Length = size(input,1);

% initialize the weights
Weight = {};
Weight{1} = [.1; .1];
Weight{2} = [.1 .1];

% weights will be trained epoch_Length times
for epoch = 1:epoch_Length
    if sample_Size ~= 0
        [Weight, error, sample_Size, o_Output, c_Weight] = SGD_Method(Weight, input(epoch,:), correct_Output(epoch,:), error_Total, sample_Size, sample_Size_Total, c_Weight);
    else
        sample_Size = sample_Size_Total;
        [Weight, error, sample_Size, o_Output, c_Weight] = SGD_Method(Weight, input(epoch,:), correct_Output(epoch,:), error_Total, sample_Size, sample_Size_Total, c_Weight);
    end
    error_Total = error;
end

fprintf("The error is:\n");
disp((1/size(input,1))*error);
fprintf("With a final input and output of:\n");
disp([input(end,:)']);
disp([o_Output]);
fprintf("That correlates to the final correct output of:\n");
disp(correct_Output(length(input),:)');

function y = Sigmoid(x)
    y = 1./(1+exp(-x));
end

function y = d_Sigmoid(x)
    y = Sigmoid(x) * (1 - Sigmoid(x));
end

function [Weight, error_Total, sample_Size, o_Output, c_Weight] = SGD_Method(Weight, input, correct_Output, error_Total, sample_Size, sample_Size_Total, c_Weight)
    alpha = .0001;
    
    % forward propagate
    h_Output = [Sigmoid(Weight{1}(1)*input); Sigmoid(Weight{1}(2)*input)];
    
    o_Output = Weight{2}(1)*(Sigmoid(Weight{1}(1)*input)) + Weight{2}(2)*(Sigmoid(Weight{1}(2)*input));
    
    error_Total = error_Total + (o_Output - correct_Output)^2;

    % backpropagate
    
    delta_h = [(o_Output - correct_Output) .* (arrayfun(@d_Sigmoid,(Weight{2}*h_Output)))];
    
    delta_i = [transpose(Weight{2})*delta_h .* (arrayfun(@d_Sigmoid,(Weight{1}*input)))];
    
    delta_h = delta_h * transpose(h_Output);
    
    delta_i = delta_i * transpose(input);
    
    % store each gradient into the list c_Weight
    if isempty(c_Weight) == 1
        c_Weight = {delta_h, delta_i};
    else
        c_Weight{1} = delta_h + c_Weight{1};
        c_Weight{2} = delta_i + c_Weight{2};
    end
    
    % decrease the running value sample_Size by one.
    % when the batch has, in its  entirety, been propagated through the network (when sample_Size = 0, you update the weights.
    sample_Size = sample_Size - 1;
    
    % update the weights
    if sample_Size == 0
        Weight{2} = Weight{2} - (alpha)*(c_Weight{1}/sample_Size_Total);
        Weight{1} = Weight{1} - (alpha)*(c_Weight{2}/sample_Size_Total);
    end
end