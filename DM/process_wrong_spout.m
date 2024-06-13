function    process_wrong_spout(app)
global lightval;
global correct_L correct_R bias_ptr  reward_spout;
global wrong_spout sdata;

wrong_spout = 1;  % to be used later for ssd and tcsa


fprintf('wrong spout_____________________\n');

if app.light2.Value == 0                  % if in 6 panel mode
       if app.retrial_mode.Value == 0           % not retrial
               if app.punishment_active.Value        % punishment active
                       if app.no_test_punishment.Value && lightval >0 && lightval<7      % if no punish on test panesl
                           to_state_4(app);                                 % no punishment, no reward, new trial
                       else                                                                         % if punish on test panels
                           to_state_3_5(app,1);  % punishment, go to new trial
                       end
               else                                             % punishment not active
                       to_state_1(app);     % new trial
               end
       else                                                 % in retrial mode
               if app.punishment_active.Value            % punishment active
                   if app.no_test_punishment.Value && lightval >0 && lightval<7      % if no punish on test panesl
                           to_state_4(app);                                 % no punishment, no reward, new trial
                   else                                                                         % if punish on test panels
                           to_state_3_5(app,1);  % punishment
                   end
               else                                                 % punishment not active (shouldn't really be used)
                   %                                            to_state_3_5(app,get(app.punishment_active,'value')); % do retrial without punishment (only short timeout)
                   to_state_1(app);  % new trial
               end
       end
elseif app.light2.Value == 1 && app.retrial_mode.Value == 0 % 2 panel, no retrial mode
       if ~app.punishment_active.Value        % if punish not active
               sdata = [];                                   % keep waiting for correct lick.
       else                                              % in punish active
               to_state_3_5(app,1);  % punishment
       end
elseif app.light2.Value == 1 && app.retrial_mode.Value == 1 % 2 panel, retrial mode
       if ~app.punishment_active.Value           % if punish not active
           %                                        to_state_4(app);         % no punishment, no reward go to state 1.
           to_state_3_5(app,0);  % no punishment, no reward, retrial
       else
           to_state_3_5(app,1);  % punishment and retrial
       end
end

%  wrong  side detected
if reward_spout == 0  % left
    correct_L(bias_ptr) =    0;
else
    correct_R(bias_ptr) =    0;
end
% bias_ptr =  1 + mod(bias_ptr,bias_buffer_size);
bias_ptr =  1 + bias_ptr;
