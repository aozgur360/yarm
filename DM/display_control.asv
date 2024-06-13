function str = display_control(app)
global state;
global sdata;
global prevlick;
global lickdetected;
global stim;
global stimL;
global stimR;
global white_background;
global samesidecount;
global jumpcount;
global stim_txt;
global o_stim;
global x_stim;
global hsquare;
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
global state1_counter;
global reward_count;
global screen_a;
global screen_b;
global screen_c;


state = 1;
app.statenumber.Text = '1';
state1_counter = state1_counter + 1;
app.state1_count.Text = num2str(state1_counter);
app.reward_counter.Text = num2str(reward_count);

if reward_count == app.corr_trials_lim.Value
    stoprun(app);
end


    
    


% percent stim_L 
if rand > app.percent_left.Value/100
    tempstim = 1;
else
    tempstim = 0;
end

if app.bypass.Value == 0
    if tempstim == stim
        samesidecount = samesidecount + 1;
        jumpcount = 0;
    else
        samesidecount = 1;
        jumpcount = jumpcount + 1;
    end

    if samesidecount > 3
        samesidecount = 1;
        jumpcount = 1;
        if tempstim == 0
            stim = 1;
        else
            stim = 0;
        end
    elseif jumpcount > 3
        jumpcount = 0;
        samesidecount = 2;
        if tempstim == 0
            stim = 1;
        else
            stim = 0;
        end
    else
        stim = tempstim;
    end
else
    stim = tempstim;
end












if stim == 1
%     stim_img = app.sfg_R.Value;
    stim_img = 1;
    stim_txt = 'stimR';
else
%     stim_img = app.sfg_L.Value;
    stim_img = 0;
    stim_txt = 'stimL';
end


% %spatial frequency generator
% width = 1024;
% height = 600;
% column_width = 100;
% 
% % x = zeros(height, width);
% 
% 
% xr = zeros(height, width);
% xl = zeros(height, width);
% 
% column_widthr = app.sfg_R.Value;
% for i = 1:width
%     indexr = i - 1;
%     roundedr = floor(indexr / column_widthr);
%     groupr = mod(roundedr, 2);
%     if groupr == 0
%         xr(:,i) = zeros(height,1);
%     elseif groupr == 1
%         xr(:,i) = ones(height,1);
%     end
% end
% column_widthl = app.sfg_L.Value;
% for i = 1:width
%     indexl = i - 1;
%     roundedl = floor(indexl / column_widthl);
%     groupl = mod(roundedl, 2);
%     if groupl == 0
%         xl(:,i) = zeros(height,1);
%     elseif groupl == 1
%         xl(:,i) = ones(height,1);
%     end
% end




% if app.rcMode.Value
%     if mod(state1_counter,app.rulechangecount.Value) == 0
%         if app.PatternMatchMode.Value == 1
%             app.PatternMatchMode.Value = 0;
%             app.AudioMode.Value = 1;
%         else
%             app.PatternMatchMode.Value = 1;
%             app.AudioMode.Value = 0;
%         end
%     end
% end

if app.rcMode.Value
    app.InitOnlyMode.Value = 1;
    if mod(state1_counter,app.rulechangecount.Value) == 0
        if app.fanmarbles.Value == 1
            app.fanmarbles.Value = 0;
            load('Y:\Ali O\yarm_rig\yarm\img test\imgwks.mat');
            disp('switch to horizontal_vertical');
        else
            app.fanmarbles.Value = 1;
            load('Y:\Ali O\yarm_rig\yarm\img test\fanmarbles.mat');
            disp('switch to fanmarbles');
        end
    end
end



% percent stage2    
if app.Mixstage.Value
    if rand > app.percent_stage2.Value/100
        app.FollowLightMode.Value = 0;
        app.InitOnlyMode.Value = 1;
    else
        app.FollowLightMode.Value = 1;
        app.InitOnlyMode.Value = 0;
    end
end

%Eztest
if app.Eztest.Value
    if state1_counter < 51
     %   disp('Eztest');
        app.percent_stage2.Value = 25;
        app.Mixstage.Value = 1;
    else
        app.InitOnlyMode.Value = 1;
        app.FollowLightMode.Value = 0;
        app.Mixstage.Value = 0;
        app.Eztest.Value = 0;
    end
end






if lickdetected == 1
%    disp(stim_txt);
    str = 'L';
%    disp(stim_txt);
    if app.EasyMode.Value
        fullscreen_a(white_background,screen_a);
    elseif app.nostim.Value
        fullscreen_a(white_background,screen_a);
        fullscreen_b(white_background,screen_b);
        fullscreen_c(white_background,screen_c);
        disp('nostim_trial')
    elseif app.FollowLightMode.Value
%         fullscreen2(x,2);
%         if stim_img == app.sfg_R.Value
        if stim_img == 1
            fullscreen_a(vsquare,screen_a);
            fullscreen_c(vsquare,screen_c);
%             fullscreen_b(xl,3);
        else
            fullscreen_a(hsquare,screen_a)
            fullscreen_b(hsquare,screen_b);
%             fullscreen_c(xr,4);
        end

    elseif app.PatternMatchMode.Value
%         fullscreen2(x,2);
        if stim_img == 1
            fullscreen_a(vsquare,screen_a);
            fullscreen_c(vsquare,screen_c);
            fullscreen_b(hsquare,screen_b);
        else
            fullscreen_a(hsquare,screen_a)
            fullscreen_b(hsquare,screen_b);
            fullscreen_c(vsquare,screen_c);
        end
    elseif app.AudioMode.Value
        if stim_img == 1
            [y,Fs] = audioread('20hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        else
            [y,Fs] = audioread('125hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        end
    elseif app.InitOnlyMode.Value
        if stim_img == 1
            fullscreen_a(vsquare,screen_a);
        else
            fullscreen_a(hsquare,screen_a);
%         fullscreen2(x,2);
        end
    elseif app.ReverseMode.Value
        if stim_img == 1
            fullscreen_a(hsquare,screen_a);
        else
            fullscreen_a(vsquare,screen_a);
%         fullscreen2(x,2);
        end
    elseif app.NewStim.Value
        if stim_img == 1
            fullscreen_a(x_stim,screen_a);
        else
            fullscreen_a(o_stim,screen_a);
        end
    end

elseif lickdetected == 2
%    disp(stim_txt);
    str = 'R';
 %   disp(stim_txt);
    if app.EasyMode.Value
        fullscreen_b(white_background,screen_b);
    elseif app.nostim.Value
        fullscreen_a(white_background,screen_a);
        fullscreen_b(white_background,screen_b);
        fullscreen_c(white_background,screen_c);
        disp('nostim_trial')
    elseif app.FollowLightMode.Value

        if stim_img == 1
            fullscreen_b(vsquare,screen_b);
            fullscreen_a(vsquare,screen_a); 
        else
            fullscreen_b(hsquare,screen_b);
            fullscreen_c(hsquare,screen_c);
%             fullscreen2(xr,2);
        end
%     elseif app.pcontrast50.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r50,2);
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l50,4);
%         end
%     elseif app.pcontrast75.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r75,2);
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l75,4);
%         end
%     elseif app.pcontrast80.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r80,2);
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l80,4);
%         end
%      elseif app.pcontrast85.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r85,2);
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l85,4);
%         end
%     elseif app.pcontrast90.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r90,2);
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l90,4);
%         end
%     elseif app.pcontrast95.Value
%         if stim_img == 1
%             fullscreen_b(r,3);
%             fullscreen2(r95,2); 
%         else
%             fullscreen_b(l,3);
%             fullscreen_c(l95,4);
%         end
    elseif app.PatternMatchMode.Value
%         fullscreen_b(x,3);
        if stim_img == 1
            fullscreen_b(vsquare,screen_b);
            fullscreen_a(vsquare,screen_a);
            fullscreen_c(hsquare,screen_c);
        else
            fullscreen_b(hsquare,screen_b);
            fullscreen_c(hsquare,screen_c);
            fullscreen_a(vsquare,screen_a);
        end
     elseif app.AudioMode.Value
        if stim_img == 1
            [y,Fs] = audioread('20hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        else
            [y,Fs] = audioread('125hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        end
    elseif app.InitOnlyMode.Value
        if stim_img == 1
            fullscreen_b(vsquare,screen_b);
        else
            fullscreen_b(hsquare,screen_b);
        end
    elseif app.ReverseMode.Value
        if stim_img == 1
            fullscreen_b(hsquare,screen_b);
        else
            fullscreen_b(vsquare,screen_b);
        end
    elseif app.NewStim.Value
        if stim_img == 1
            fullscreen_b(x_stim,screen_b);
        else
            fullscreen_b(o_stim,screen_b);
        end
    end
    
else
%    disp(stim_txt);
    str = 'C';
  %  disp(stim_txt);
    if app.EasyMode.Value
        fullscreen_c(white_background,screen_c);
    elseif app.nostim.Value
        fullscreen_a(white_background,screen_a);
        fullscreen_b(white_background,screen_b);
        fullscreen_c(white_background,screen_c);
        disp('nostim_trial')
    elseif app.FollowLightMode.Value

%         fullscreen_c(x,4);
        if stim_img == 1
            fullscreen_c(vsquare,screen_c);
            fullscreen_b(vsquare,screen_b);
%             fullscreen2(xl,2);
        else
            fullscreen_c(hsquare,screen_c);
            fullscreen_a(hsquare,screen_a);
%             fullscreen_b(xr,3);
        end
%     elseif app.pcontrast50.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r50,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l50,2);
%         end
%     elseif app.pcontrast75.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r75,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l75,2);
%         end
%     elseif app.pcontrast80.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r80,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l80,2);
%         end
%     elseif app.pcontrast85.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r85,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l85,2);
%         end
%     elseif app.pcontrast90.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r90,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l90,2);
%         end
%     elseif app.pcontrast95.Value
%         if stim_img == 1
%             fullscreen_c(r,4);
%             fullscreen_b(r95,3);
%         else
%             fullscreen_c(l,4);
%             fullscreen2(l95,2);
%         end
    elseif app.PatternMatchMode.Value
%         fullscreen_c(x,4);
        if stim_img == 1
            fullscreen_c(vsquare,screen_c);
            fullscreen_b(vsquare,screen_b);
            fullscreen_a(hsquare,screen_a);
        else
            fullscreen_c(hsquare,screen_c);
            fullscreen_a(hsquare,screen_a);
            fullscreen_b(vsquare,screen_b);
        end
     elseif app.AudioMode.Value
        if stim_img == 1
            [y,Fs] = audioread('20hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        else
            [y,Fs] = audioread('125hz_short.wav');
            sound(y,Fs);
%             pause(10);
%             clear sound
        end
    elseif app.InitOnlyMode.Value
        if stim_img == 1;
            fullscreen_c(vsquare,screen_c);
        else
            fullscreen_c(hsquare,screen_c);
        end
    elseif app.ReverseMode.Value
        if stim_img == 1;
            fullscreen_c(hsquare,screen_c);
        else
            fullscreen_c(vsquare,screen_c);
        end
    elseif app.NewStim.Value
        if stim_img == 1
            fullscreen_c(x_stim,screen_c);
        else
            fullscreen_c(o_stim,screen_c);
        end
    end
    end

% reward_Callback(app,str);
prevlick = lickdetected;
% lickdetected = 0;