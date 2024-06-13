function state_machine(app)

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
global run;
global state;
global arduino;
global rtimer stimtimer
global spout1pos spout2pos;
global lick_countc;
global lick_time_startc;
global lick_countl lick_countr;
global lick_time_startl lick_time_startr;
global lick_time_c   lick_time_r lick_time_l;
global reward_spout;
global sdata;
global light_count;
global reward_denied;
global vlpair;
global lightval;
global stim_timer_start_delay_active;
global wrong_spout;
global state_timer_timeout;
global lick_detected;
global retrial;
global rate;
global prevlick;
global stim;
global lickdetected;
global stimL;
global stimR;
global white_background;
global samesidecount;
global jumpcount;
global stim_txt;
global state1_counter;
global screen_a;
global screen_b;
global screen_c;

%screen_a = 4;
%screen_b = 2;
%screen_c = 3;

screen_a = 1;
screen_b = 2;
screen_c = 3;

rng('shuffle');

state = 0;
run = 1;
retrial = 0;
lickdetected = 0;
jumpcount = 0;
sdata = [];

stim = 0;
samesidecount = 0;


to_state_0(app);

while run
    drawnow;
    switch state
        
        case 0
            
          
            if lickdetected > 0

                disp('begin_trial');
                fprintf('%4.3f ts \n',toc);
                disp("start spout " + lickdetected);
                
%                 if app.FollowLightMode.Value == 1
%                     disp('start_stage2');
%                 end
%                 if app.InitOnlyMode.Value == 1
%                     disp('start_stage100');
%                 end
%                 
%                 disp('lickdetected');
%                 disp('begin_trial');
%                 disp('stimnew');
                
                if app.fanmarbles.Value == 1
%                    load('Y:\Ali O\yarm_rig\yarm\img test\fanmarbles.mat');
                    disp('fanmarbles_trial');
                elseif app.icecream_pizza.Value == 1
                    disp('icecream_pizza_trial');
                else
%                    load('Y:\Ali O\yarm_rig\yarm\img test\imgwks.mat');
                    disp('verticalhorizontal_trial');
                end
                
                
                if app.TimeoutPunishmentCheckBox.Value
                    if app.Eztest.Value == 1
                        disp('Eztest');
                    end
                    str = display_control(app);
                else
                    str = display_control(app);
                    reward_Callback(app,str);
                end
                
      %          disp('begin_trial');
%                 if app.Eztest.Value == 1
%                     disp('Eztest');
%                 end
                if app.FollowLightMode.Value == 1
                    disp('start_stage2');
                end
                if app.InitOnlyMode.Value == 1
                    disp('start_stage100');
                end
                
                disp('lickdetected');
                disp(stim_txt);
%                disp('stimnew');
            end
            
            
% ----------------------------------------------------------------------------------            
        case 1
            if lickdetected ~= prevlick 
%                 fprintf('%d %d/',prevlick, stim);
               
                
                switch prevlick
                    case 1
%                     if app.FollowLightMode.Value == 1
%                         disp('start_stage2');
%                     end
%                     if app.InitOnlyMode.Value == 1
%                         disp('start_stage100')
%                     end
                        
                    if app.EasyMode.Value
                        if lickdetected == 2 || lickdetected == 3
                            fullscreen_a(white_background,screen_a);
                            fullscreen_b(white_background,screen_b);
                            fullscreen_c(white_background,screen_c);
                            str = display_control(app);
                            reward_Callback(app,str);
                        end
                    elseif (lickdetected == 2) && (stim == 0)
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);
                    elseif (lickdetected == 3) && (stim == 1)
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);          
                    else
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        if app.nostim.Value == 1
                            disp('nostim_trial')
                        end
                        if app.TimeoutPunishmentCheckBox.Value
                            disp('timeout punishment')
%                            disp('begin_trial')
                            % light punishment
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',1);  %right
                            pause(app.timeout_punishment.Value);
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',0);  %right
                            lickdetected = 0;
                            clear sound;
                            to_state_0(app);
                        else
                            clear sound;
                            str = display_control(app);
                        end
                    end
                    
                    case 2

                    if app.EasyMode.Value
                        if lickdetected == 3 || lickdetected == 1
                            fullscreen_a(white_background,screen_a);
                            fullscreen_b(white_background,screen_b);
                            fullscreen_c(white_background,screen_c);
                            str = display_control(app);
                            reward_Callback(app,str);
                        end
                    elseif lickdetected == 3 && stim == 0
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);                    
                    elseif lickdetected == 1 && stim == 1
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);                    
                    else
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        if app.nostim.Value == 1
                            disp('nostim_trial')
                        end
                        if app.TimeoutPunishmentCheckBox.Value
                            disp('timeout punishment')
%                            disp('begin_trial')
                            % light punishment
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',1);  %right
                            pause(app.timeout_punishment.Value);
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',0);  %right
                            lickdetected = 0;
                            clear sound;
                            to_state_0(app);
                        else
                            clear sound;
                            str = display_control(app);
                        end
                    end
                        
                    case 3

                    if app.EasyMode.Value
                        if lickdetected == 1 || lickdetected == 2
                            fullscreen_a(white_background,screen_a);
                            fullscreen_b(white_background,screen_b);
                            fullscreen_c(white_background,screen_c);
                            str = display_control(app);
                            reward_Callback(app,str);
                        end
                    elseif lickdetected == 1 && stim == 0
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);                    
                    elseif lickdetected == 2 && stim == 1
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        str = display_control(app);
                        reward_Callback(app,str);                    
                    else
                        fullscreen_a(white_background,screen_a);
                        fullscreen_b(white_background,screen_b);
                        fullscreen_c(white_background,screen_c);
                        clear sound;
                        if app.nostim.Value == 1
                            disp('nostim_trial')
                        end
                        if app.TimeoutPunishmentCheckBox.Value
                            disp('timeout punishment')
%                            disp('begin_trial')
                            % light punishment
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',1);  %right
                            pause(app.timeout_punishment.Value);
                            fprintf(arduino,'%c','@');  %right
                            fprintf(arduino,'%c',0);  %right
                            lickdetected = 0;
                            clear sound;
                            to_state_0(app);
                            
                        else
                            clear sound;
                            str = display_control(app);
                        end
                    end
                end
                        
 
            end
%             lickdetected = 0;

% ----------------------------------------------------------------------------------            
%         case 2 
%             pause(app.timeout_punishment.Value);
%             if lickdetected > 0
%                 disp('lickdetected');
% %                 print lck_spoutX and print stimX
%                 str = display_control(app);
%             end
            
    end
end
            