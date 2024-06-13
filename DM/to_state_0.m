function to_state_0(app)
global state;
global stimL;
global stimR;
global hsquare;
global o_stim;
global x_stim;
global vsquare;
global l;
global r;
global l50;
global l75;
global l80;
global l85;
global l90;
global l95;
global r50;
global r75;
global r80;
global r85;
global r90;
global r95;
global white_background;
global background;
global fan;
global marbles;
global icecream;
global pizza;
global screen_a;
global screen_b;
global screen_c;

state = 0;
app.statenumber.Text = '0';

%load imgwks.mat
%load('Y:\Ali O\yarm_rig\yarm\img test\imgwks.mat');

if app.fanmarbles.Value == 1
    load('Y:\Ali O\yarm_rig\yarm\img test\fanmarbles.mat');
%    disp('fanmarbles_trial');
elseif app.icecream_pizza.Value == 1
    load('Y:\Ali O\yarm_rig\yarm\img test\icecream_pizza.mat');
else
    load('Y:\Ali O\yarm_rig\yarm\img test\imgwks.mat');
%    disp('verticalhorizontal_trial');
end

%if app.nostim.Value == 1
%    disp('nostim_trial')
%end

% return;
fullscreen_a(white_background,screen_a);
fullscreen_b(white_background,screen_b);
fullscreen_c(white_background,screen_c);