% One comment
C = A * B;   
% Comment 2
%{ D :: double^8 x double^10%}
% Ideally, I want pragmas but I'm not sure how to do this with Text.Parsec.Token
% x is a vector, D is a matrix
%{ x :: double^10
%} 
D :: double (5,5) \in [-10,10]
D = E * x; % Another comment
