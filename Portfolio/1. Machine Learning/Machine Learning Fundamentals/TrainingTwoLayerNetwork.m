%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% all rights reserved
% Author: Rachel Ireland-Jones
% 14/11/2022
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [o, W1, W2, e] = TwoLayerNetworkTraining(hN, alpha, iterations, data, W1, W2, t)
%{ 
    iteration to train 2 layer sigmoid network 
    % net2 = summed weighted inputs in layer 1
    % a2 = layer 1 network activation after non-linearity
    % W1 are the weights in layer 1
    % W2 are the weights in layer 2
    % net3 ares summed weighted inputs in layer 2
    % a3 is layer 2 network activation after non linearity, it is also the overall network output o


    % training involves minimizing error of calculated output
    % we want to adjust the weights by performing gradient descent
    % we need to find the error gradients which require repetitive application
    % of the chain rule

    % training involves
        % - a forward phase to generate network output
        % - a back propogation of error phase to estimate weight changes
    % we can carry out the update
        % - after presentation of each data sample
        % - over batches of data presentations
    
%}    
    er = 5;
    for idx = 1:iterations
        %calculate internal activations of layer 1 
        net2 = W1 * data;                               % 2x3 * 3x1000 = 2x1000
        a2 = Sigmoid(net2);                             % 2x1000

        %augment a2 to account for bias term in W2 
        a2Hat = [a2; ones(1, length(a2))];              % 3x1000 

        %calculate ouput activations of layer 2
        net3 = W2 * a2Hat;                            	% 1x1000
        o = net3;                                       % 1x1000
        
        %calculate output layer delta term
        delta3 = -(t-o);                           % 2x1000

        %back propogate to calculate input (lower) layer delta term
        W2Hat = W2(:, 1:hN);                         % 1x2 W2(3) is bias weight so not needed
        delta2 = (W2Hat' * delta3) .* a2 .* (1-a2);    % (1x2 * 2x1000) . 2x1000 . 2x1000;
       
        %calculate error gradient w.r.t W1
        gradW1 = delta2 * data';                        % 2x1000 * 3x1000(transposed) = 2*3

        %calculate error gradient w.r.t W2
        gradW2 = delta3 * a2Hat';                       % 2x1000 * 3x1000(transposed)= 2*3

        % update W1 and W2 by moving down towards minumum error 
        % (alpha = learning rate)
        W1 = W1 - alpha*gradW1;                         % 2x3
        W2 = W2 - alpha*gradW2;                         % 2x3
        
        % calculate error
        er = mean(mean((t-o).^2));
        e(:,idx) = er;   
        if (er < 0.03)
            break
        end
    end
end  

