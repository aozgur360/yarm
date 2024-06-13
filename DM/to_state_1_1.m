function to_state_1_1(app)
global state rtimer sdata stimtimer;
global stim_timer_start_delay_active lick_detected light_countc reward_spout;
global retrial;
global rate;
state  = '1_1';
app.statenumber.Text = state;
stop(rtimer);
fprintf('%4.3f to_state_1_1 \n',toc);

stop(stimtimer);
light_countc  = 0;

% only update spout if not retrial  ( instead of retrial mode)
if ~retrial
      
    if app.SideDiscrimination.Value
    %         side discrimination
        reward_spout = get_next_spout(app) ;
    else
    %         rate discrimination
            reward_spout = get_next_rates(app);
    end
end
 del = start_stimulus_with_delay(app);
 tm = del  - app.tcsa_lick_time_window.Value;
 
 if tm > 0
        licktimer = timer;
        licktimer.ExecutionMode ='singleshot';
        licktimer.TimerFcn = {@clear_licks};
        start(licktimer);
 end

stim_timer_start_delay_active = 1;
sdata = [];     %  clear licks
lick_detected = 0;

