function to_state_4(app)
global state rtimer;
global reward_spout   % GL edit 9.3.19
global correct_L correct_R bias_ptr bias_buffer_size;

state  = '4';
app.statenumber.Text = state;
stop(rtimer);
rtimer.StartDelay = app.wait24.Value;   % GL edit 190903: wait for GUI set time
start(rtimer);

% correct side
if reward_spout == 0  % left
    correct_L(bias_ptr) =    1;
else
    correct_R(bias_ptr) =    1;
end
bias_ptr =  1 + bias_ptr;

fprintf('%4.3f to_state_4\n',toc);
