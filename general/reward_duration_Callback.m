
function reward_duration_Callback(app)
% hObject    handle to reward_duration (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of reward_duration as text
%        str2double(get(hObject,'String')) returns contents of reward_duration as a double
global arduino;
fprintf(arduino,'%c','q');
rd = app.reward_duration.Value;
rd = uint8(rd);
fprintf(arduino,'%s',char(rd));

