function init_timers(app)

global stimtimer rtimer;
global stimtimer_up;


rtimer = timer;
rtimer.ExecutionMode ='singleshot';
rtimer.TimerFcn = {@next_state};
rtimer.StartDelay = 2;


% for stim control
stimtimer = timer;
stimtimer.ExecutionMode ='fixedrate';
stimtimer.TimerFcn = {@stim_start,app};
stimtimer.StartDelay = (app.stim_start_delay_CDM.Value);
stimtimer.Period = (app.stim_cycle_duration_CDM.Value);
stimtimer.TasksToExecute =  (app.number_of_stim_cycles_CDM.Value);


% for stim control
stimtimer_up = timer;
stimtimer_up.ExecutionMode ='singleshot';
stimtimer_up.TimerFcn = {@light_off,app};
