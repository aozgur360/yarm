function create_mixed_rates_table(app)

global rtable;
global randbuf randptr;
max_rate = app.high_rate.Value;

% rtable = cell(7,4);
rtable = [];

switch str2double(app.mixed_rates_steps.Value)
    
    case 3
        rtable{1} = [1 4; 2 8; 3 12];
        rtable{2} = [2 4; 3 6; 5 10; 6 12];
        rtable{3} = [5 6; 6 7; 9 11; 10 12];
        rtable{4} = [6 5; 7 6; 11 9; 12 12];
        rtable{5} = [4 2; 6 3; 10 5; 12 6];
        rtable{6} = [4 1; 8 2; 12 3];
        
    case 4
        rtable{1} = [1 4; 2 8; 3 12];
        rtable{2} = [2 6; 3 9; 4 12];
        rtable{3} = [2 4; 3 6; 5 10; 6 12];
        rtable{4} = [5 6; 6 7; 9 11; 10 12];
        rtable{5} = [6 5 ;7 6; 11 9; 12 10];
        rtable{6} = [4 2; 6 3;10 5; 12 6];
        rtable{7} = [6 2; 9 3; 12 4];
        rtable{8} = [4 1; 8 2; 12 3];

    case 5 
        rtable{1} = [1 4; 2 8; 3 12];
        rtable{2} = [2 6; 3 9; 4 12];
        rtable{3} = [2 5; 4 10];
        rtable{4} = [2 4; 3 6; 5 10; 6 12];
        rtable{5} = [5 6; 6 7; 9 11; 10 12];
        rtable{6} = [6 5; 7 6; 11 9; 12 10];
        rtable{7} = [4 2; 6 3; 10 5; 12 6];
        rtable{8} = [5 2; 10 4];
        rtable{9} = [6 2; 9 3; 12 4];
        rtable{10} = [4 1; 8 2; 12 3];
        

    case 6
        rtable{1} = [1 4; 2 8; 3 12];
        rtable{2} = [2 6; 3 9; 4 12];
        rtable{3} = [2 5; 4 10];
        rtable{4} = [2 4; 3 6; 5 10; 6 12];
        rtable{5} = [1 3 ; 4 6; 6 9 ; 8 12];
        rtable{6} = [ 5 6; 6 7; 9 11; 10 12];
        rtable{7} = [ 6 5; 7 6; 11 9; 12 10];
        rtable{8} = [3 1 ; 6 4; 9  6; 12 8];
        rtable{9} =  [4 2; 6 3; 10 5; 12 5];
        rtable{10} = [5 2; 10 4];
        rtable{11} = [ 6 2; 9 3; 12 4];
        rtable{12} = [4 1; 8 2; 12 3];
end

[r,c] = size(rtable);

% get rid of frequency pairs that are higher than maximum
for i = 1:c
    temp = rtable{i};
    [k,l] = size(temp);
    ridx = [];
    for j = 1:k
        tp = temp(j,:);
        if max(tp) > max_rate
            ridx = [ridx j];
        end
    end
    temp(ridx,:) = [];
    rtable{i} = temp;
end

% rtable now contains  the frequency pairs

 n = 2*str2double(app.mixed_rates_steps.Value) - 2;
randbuf = randperm(n);
randptr = 1;
