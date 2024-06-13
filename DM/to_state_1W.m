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
    stim_img = stimL;
else
    stim = 0;
    stim_img = stimR;
end

if lickdetected == 1
    fullscreen_a(stim_img,screen_a);

elseif lickdetected == 2
    fullscreen_b(stim_img,screen_b);
else
    fullscreen_c(stim_img,screen_c);
end

prevlick = lickdetected;
% lickdetected = 0;