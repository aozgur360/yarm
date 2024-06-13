function start_Callback(app)
% hObject    handle to start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global run newchar;
global number_of_entries;
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
global number_of_consecutive_identical_spouts;
global sdata;
global randbuf_left randbuf_right  randptr delay_randptr;
global scount;
global light_count;
global reward_denied;
global vlpair;
global runtimer;
global lightval;
global write_index;
global stim_timer_start_delay_active;
global correct_L correct_R bias_ptr bias_buffer_size;
global stim_duration_value;
global stimtimer_up;
global delay_rand_buffer;
global previous_ssd previous_tcsa;
global wrong_spout;
global state1_counter;
global reward_count;

% wrong_spout = 0;  % to be used later for ssd and tcsa


previous_ssd = [];
previous_tcsa = [];

% stim_duration_value = app.stim_duration.Value;

write_index = 1;
% bias parms
% bias_buffer_size = (app.bias_buffer.Value);
correct_L  = zeros(10000,1); % good for 10000 points
correct_R  = zeros(10000,1);
bias_ptr  =  1;

vlpair(1) = 0;
reward_denied = 0;
scount = 0;
state1_counter = 0;
reward_count = 0;
app.state1_count.Text = num2str(state1_counter);
rng('shuffle');

% --------------------------------
% init delay buffers
% delay_rand_buffer =  linspace(app.min_delay.Value, app.max_delay.Value,app.numberofsteps.Value);
% delay_rand_buffer = round(delay_rand_buffer,3); % to 1 ms resolution
% delay_randptr = 1;
% delay_rand_buffer = delay_rand_buffer(randperm(numel(delay_rand_buffer)));
% --------------------------------
% init timers

delete(timerfindall);

% rtimer = timer;
% rtimer.ExecutionMode ='singleshot';
% rtimer.TimerFcn = {@next_state};
% rtimer.StartFcn = {@start_state};
% rtimer.StartDelay = 2;

% for stim control
% stimtimer = timer;
% stimtimer.ExecutionMode ='fixedrate';
% stimtimer.TimerFcn = {@stim_start,app};
% stimtimer.StopFcn = {@stim_stop,app};
% stimtimer.StartDelay = (app.stim_start_delay.Value);
% stimtimer.Period = (app.stim_cycle_duration.Value);
% stimtimer.TasksToExecute =  (app.number_of_stim_cycles.Value);

% for stim control
% stimtimer_up = timer;
% stimtimer_up.ExecutionMode ='singleshot';
% stimtimer_up.TimerFcn = {@light_off,app};

% --------------------------------

randbuf_left = randperm(3);
randbuf_right = 3 + randperm(3);
randptr = 1;

% --------------------------------
% if app.RateDiscrimination.Value 
%     if app.two_rates_only.Value
%         create_2rates_table(app);
%     else
%         create_mixed_rates_table(app);
%     end
% end
% --------------------------------

dirname = app.animalid.Value;
if (exist(dirname,'dir') ~= 7)
   mkdir(dirname);
end

%trigger miniscope recording
% fprintf(arduino,'%c','+');  
% fprintf(arduino,'%c',1);  

% light_Callback(0,7);
% pause(0.1);
% light_Callback(1,7);
% pause(0.1);
% light_Callback(2,7);

% if get(app.retrial_mode,'value')
%     set(app.punishment_active,'value',0);
%     set(app.timeout_punishment,'value',0);
% end
fn = [dirname '/'  datestr(now,'mm_dd_yyyy___HH_MM_SS') ];
diary([fn  '.txt']);

print_settings(app);

% create header containing parameter information
% hhh = fieldnames(app);
% for i = 1:length(hhh)
%    s = app.(hhh{i}); 
%    aaa= get(s);
%    if isfield(aaa,'Style')
%        if strcmp(s.Style,'edit')
%             fprintf('%s     %s \n',aaa.Tag, aaa.Value);
%         end
%    end
% end
% 
% for i = 1:length(hhh)
%    s = app.(hhh{i}); 
%    aaa= get(s);
%    if isfield(aaa,'Style')
%        if strcmp(s.Style,'checkbox')
%              fprintf('%s   %d \n',aaa.Tag, aaa.Value);
%         end
%    end
% end

fprintf(' \n \n');
fprintf('-------------------------------------------------- ');
fprintf(' \n \n');

% spout1pos.rest = str2double(app.sp1rest.Value);
% spout1pos.ext = str2double(app.sp1ext.Value);
% spout2pos.init = 0;
% spout2pos.rest = str2double(app.sp2rest.Value);
% spout2pos.ext = str2double(app.sp2ext.Value);

fprintf(arduino,'%c',['q' round(str2double(app.r_pulse_C.Value))]);
pause(0.05);
fprintf(arduino,'%c',['v' round(str2double(app.r_pulse_L.Value))]);
pause(0.05);
fprintf(arduino,'%c',['w' round(str2double(app.r_pulse_R.Value))]);
pause(0.05);
% fprintf(arduino,'%c',['u' round((app.airpuff_duration.Value))]);

% ---------------------------------------------------
% initialize light panel
% fprintf(arduino,'%c',['}'  round(app.DM_brightness_volume.Value) 1]);   % do  initialize panels
% pause(1.5);
% ---------------------------------------------------
% initialize volume and tone freq
% fprintf(arduino,'%c','a');
% audio_volume = app.DM_brightness_volume.Value;
% fprintf(arduino,'%c',audio_volume);
% fprintf('audio volume = %d \n',audio_volume);
%     
% %     tone freq
% tf = app.DM_tone_freq.Value;
% btl = mod(tf,256); 
% bth = floor(tf/256);
% fprintf(arduino,'%c',['=' btl bth]);
% ---------------------------------------------------


% for state control
% rtimer.StartDelay = 2;

% for experiment duration
%for miniscope/webcam trigger
fprintf(arduino,'%c','+');  
fprintf(arduino,'%c',1);  
runtimer = timer;
runtimer.ExecutionMode ='singleshot';
% runtimer.TimerFcn = {@stoprun,app};
runtimer.TimerFcn = {@stoprun};
runtimer.StartDelay = (app.ex_duration.Value);

run = 1;
app.start.Text  = 'Stop';
% app.start.Enable  = 'off';

bav = get(arduino,'BytesAvailable');
if bav > 0
    a = fread(arduino,bav);    
    fprintf('%s',a)
end

enint_Callback( app);
newchar = 0;
number_of_entries = 0;

state = '0';
app.statenumber.Text  = state;
% start(rtimer);
diary on;

number_of_consecutive_identical_spouts = 0;
% fprintf('%4.3f state 0\n',toc);

bav = get(arduino,'BytesAvailable');
if bav > 0
    sdata = fread(arduino,bav);    
    fprintf('%4.3f %s  \n',toc,sdata);
    sdata = [];
end

start(runtimer);
reward_spout = 0;
vlpair = [0 5];

% -------------------------
tic;

% to_state_0(app);
state_machine(app);
% -------------------------

stop(timerfindall);
% noise off
fprintf(arduino,'%c','>');
% fprintf('%4.3f noise_stop \n',toc);
pause(0.05);
% retract spouts at the end
% control_spouts(0,0);
% stop(stimtimer);
% light_off(0,0,app);          % GL edit 7.4.19 not perfect! (error if stopped before lights)
reward_denied = 0;                                  % GL edit 7.6.19

app.start.Text = 'Start';
state = '0';
app.statenumber.Text = state;
delete(timerfindall);

fprintf('\n\n');
fprintf('edit: ');
for i = 1:write_index-1
    fprintf('%4.3f  %s \n',notestime{i},char(notes(i)));
end
   
diary off;

% all lights off
% light_Callback(0,7);
% pause(0.1);
% light_Callback(1,7);
% pause(0.1);
% light_Callback(2,7);
