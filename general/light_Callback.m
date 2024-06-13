function light_Callback(vl1,vl2)
% hObject    handle to lightr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global arduino;
% global vlpair;

% vlpair = [vl1 vl2];
 
switch vl1
    case 0
    fprintf(arduino,'%c',['r' vl2]);
    case 1
    fprintf(arduino,'%c',['s' vl2]);
    case 2
    fprintf(arduino,'%c',['t' vl2]);
end        
% fprintf(arduino,'%c',vl2);
