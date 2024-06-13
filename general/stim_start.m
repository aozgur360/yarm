function stim_start(a,b,app)

global stimtimer stimtimer_up;
global randtable lightval;
global light_count;
% global vl1;
global vlpair;
global randfornext;
global stim_timer_start_delay_active;
global stim_duration_value;
global reward_spout;
global arduino;

stim_timer_start_delay_active = 0;

if app.SideDiscrimination.Value  
% side discrimination

    % ------------------------------------------------------
    if app.Light.Value

        %     light mode
        % extra light off here
        light_Callback(vlpair(1),7);

        light_count = light_count + 1;
        stop(stimtimer_up);
        if app.light2.Value
             light_2_panel(app);
        else
        %     6 panel mode
             if (100*randfornext) < (app.percentage_of_training_trials.Value)
            %         do 2 panel instead of 6 panel
                    light_2_panel( app);
              else
                    vlpair = [randtable(lightval,1),randtable(lightval,2)];
                    if app.allleds.Value
                        light_Callback(vlpair(1),4);  % page 4 all leds on
                    else
                        light_Callback(vlpair(1),vlpair(2));
                    end
        %             fprintf('%4.3f light_on_%d_%d  \n',toc,lightval,light_count);
                    fprintf('%4.3f  light_on_%d_%d  \n',toc,lightval,light_count);
                    fprintf('%4.3f Stim_start\n',toc);
            end
        end
    % ------------------------------------------------------
    elseif app.Tone.Value
        %     tone mode
%         fprintf(arduino,'%c','?');   % left right
        if reward_spout == 0
        %     left spout
%             fprintf(arduino,'%c',0); % left
%             fprintf(arduino,'%c','e'); %  tone 0
            fprintf(arduino,'%c',['?' 0 'e' 1]); %  tone 0
            
            
            
        else
        %     right spout
%             fprintf(arduino,'%c',1); % right 
%             fprintf(arduino,'%c','f'); %  tone 1
            fprintf(arduino,'%c',['?' 1 'f' 1]); %  tone 0
        end
%             fprintf(arduino,'%c',1); %  tone on

    % ------------------------------------------------------
    else 
        %     noise mode
%         fprintf(arduino,'%c','?');   % left right
        if reward_spout == 0
        %     left spout
%             fprintf(arduino,'%c',0); % left
            fprintf(arduino,'%c',['?' 0 '<']); % left
            
        else
        %     right spout
%             fprintf(arduino,'%c',1); % right 
            fprintf(arduino,'%c',['?' 1 '<']); % lright
        end
%         fprintf(arduino,'%c','<'); %  noise on
    end
    % ------------------------------------------------------

    stimtimer_up.StartDelay = stim_duration_value;
    start(stimtimer_up);
    % disp('start stim');
else    % ===============================================================
%     rate discrimination

    start_rate_discrimination(app) ;   

    
end

