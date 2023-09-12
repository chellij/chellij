function [o] = TwoLayerTrained(data, W1, W2)
    % TWOLAYERTRAINED Summary of this function goes here
    % Detailed explanation goes here
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % all rights reserved
    % Author: Rachel Ireland-Jones
    % 14/11/2022
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %calculate internal activations of layer 1 
    net2 = W1 * data;                               % 2x3 * 3x1 = 2x1000
    a2 = Sigmoid(net2);                             % 2x1

    %augment a2 to account for bias term in W2 
    a2Hat = [a2; ones(1, length(a2))];              % 3x1 

    %calculate ouput activations of layer 2
    net3 = W2 * a2Hat;                            	% 1x1
    o = net3;                                       % 1x1
end  
