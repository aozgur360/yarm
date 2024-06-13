% --- Executes on button press in reward1.
function reward_Callback(app,str)
% hObject    handle to reward1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global arduino;
global stim_txt;
global reward_count;


switch str
    case 'R'
        fprintf(arduino,'%c','x');  %right
        fprintf('%4.3f reward_spout2 ts \n',toc);
        reward_count = reward_count + 1;
        disp('begin_trial');
        fprintf('%4.3f ts \n',toc);
        disp('start spout 2');
        if app.Eztest.Value == 1
            disp('Eztest');
        end
        if app.fanmarbles.Value == 1
            disp('fanmarbles_trial');
        elseif app.icecream_pizza.Value == 1
            disp('icecream_pizza_trial');
        else
            disp('verticalhorizontal_trial');
        end
        if app.FollowLightMode.Value == 1
            disp('start_stage2');
        end
        if app.InitOnlyMode.Value == 1
            disp('start_stage100');
        end
        disp(stim_txt);

    case 'L'
        fprintf(arduino,'%c','y'); %left
        fprintf('%4.3f reward_spout1 ts \n',toc);
        reward_count = reward_count + 1;
        disp('begin_trial');
        fprintf('%4.3f ts \n',toc);
        disp('start spout 1');
        if app.Eztest.Value == 1
            disp('Eztest');
        end
        if app.fanmarbles.Value == 1
            disp('fanmarbles_trial');
        elseif app.icecream_pizza.Value == 1
            disp('icecream_pizza_trial');
        else
            disp('verticalhorizontal_trial');
        end
        if app.FollowLightMode.Value == 1
            disp('start_stage2');
        end
        if app.InitOnlyMode.Value == 1
            disp('start_stage100');
        end
        disp(stim_txt);
    case 'C'
        fprintf(arduino,'%c','z'); % center
        fprintf('%4.3f reward_spout3 ts \n',toc);
        reward_count = reward_count + 1;
        disp('begin_trial');
        fprintf('%4.3f ts \n',toc);
        disp('start spout 3');
        if app.Eztest.Value == 1
            disp('Eztest');
        end
        if app.fanmarbles.Value == 1
            disp('fanmarbles_trial');
        elseif app.icecream_pizza.Value == 1
            disp('icecream_pizza_trial');
        else
            disp('verticalhorizontal_trial');
        end
        if app.FollowLightMode.Value == 1
            disp('start_stage2');
        end
        if app.InitOnlyMode.Value == 1
            disp('start_stage100');
        end
        disp(stim_txt);
end


