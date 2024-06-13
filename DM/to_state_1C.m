function to_state_1C(app)
global state;
global sdata;
global prevlick;
global lickdetected;
global stim;
global stimL;
global stimR;
global background;
global screen_a;
global screen_b;
global screen_c;

state = 1;
app.statenumber.Text = '1';
if rand > app.percent_stim.Value/100
    stim = 1;
    stim_img = stimR;
else
    stim = 0;
    stim_img = stimL;
end

if lickdetected == 1
    str = 'L';
    fullscreen_a(stim_img,screen_a);

elseif lickdetected == 2
    str = 'R';
    fullscreen_b(stim_img,screen_b);
else
    str = 'C';
    fullscreen_c(stim_img,screen_c);
end

reward_Callback(app,str);
prevlick = lickdetected;
% lickdetected = 0;