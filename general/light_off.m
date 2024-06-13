function light_off(~,~,app)

global reward_spout;
global arduino;
global stimtimer_up;
global randtable lightval;
global vlpair;


stop(stimtimer_up);
fprintf('%4.3f light off %d -------   lightval = %d    vlpair  = %d  %d\n',toc,reward_spout,lightval,vlpair(1), vlpair(2))

% if get(handles.light2,'value') || str2double(get(handles.prc,'string'))
%     % lights off
%     if reward_spout == 0  % left
%         light_Callback(0,7);
%     else  % right 
%         light_Callback(hObject, eventdata, handles,2,7);
%     end
% 
% else
%     light_Callback(hObject, eventdata, handles,randtable(lightval,1),7);
% end
if app.Light.Value

     light_Callback(vlpair(1),7);

elseif app.Tone.Value
    %     tone mode
    disp('tone off');
        if reward_spout == 0
            fprintf(arduino,'%c',['e' 0]); %  tone 0 
        else
            fprintf(arduino,'%c',['f' 0]); %  tone 1 
        end
%          fprintf(arduino,'%c',0); %  tone  off
else 
    %     noise mode
            fprintf(arduino,'%c','>'); %  noise off
end

% if stimcount == 0
% %  delete(stimtimer);
%      stop(stimtimer);
%     return
% end

% stimtimer_up.ExecutionMode ='singleshot';
% stimtimer_up.TimerFcn = {@stim_start};
% tm = str2double(mapp.stim_cycle_duration.Value) - str2double(mapp.stim_duration.Value);
% stimtimer_up.StartDelay = tm;
% start(stimtimer_up);
% disp('light off  \\\\\\');
