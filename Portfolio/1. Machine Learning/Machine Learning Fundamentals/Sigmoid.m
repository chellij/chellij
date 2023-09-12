function [fx] = Sigmoid(Q)
% calculate the activations of Q
% where Q = weight * input data augmented with 1

fx = (1 + exp(-Q)).^-1;

end
