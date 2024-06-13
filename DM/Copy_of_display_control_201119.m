function str = display_control(app)
global state;
global sdata;
global prevlick;
global lickdetected;
global stim;
global stimL;
global stimR;
global background;
global samesidecount;
global jumpcount;
global stim_txt;

state = 1;
app.statenumber.Text = '1';



if rand > app.percent_stim.Value/100
    tempstim = 1;
else
    tempstim = 0;
end

if tempstim == stim
    samesidecount = samesidecount + 1;
    jumpcount = 0;
else
    samesidecount = 1;
    jumpcount = jumpcount + 1;
end

if samesidecount > 3
    samesidecount = 1;
    jumpcount = 1;
    if tempstim == 0
        stim = 1;
    else
        stim = 0;
    end
elseif jumpcount > 3
    jumpcount = 0;
    samesidecount = 2;
    if tempstim == 0
        stim = 1;
    else
        stim = 0;
    end
else
    stim = tempstim;
end












if stim == 1
%     stim_img = app.sfg_R.Value;
    stim_img = 1;
    stim_txt = 'stimR';
else
%     stim_img = app.sfg_L.Value;
    stim_img = 0;
    stim_txt = 'stimL';
end


%spatial frequency generator
width = 1024;
height = 600;
column_width = 100;

% x = zeros(height, width);


xr = zeros(height, width);
xl = zeros(height, width);

column_widthr = app.sfg_R.Value;
for i = 1:width
    indexr = i - 1;
    roundedr = floor(indexr / column_widthr);
    groupr = mod(roundedr, 2);
    if groupr == 0
        xr(:,i) = zeros(height,1);
    elseif groupr == 1
        xr(:,i) = ones(height,1);
    end
end
column_widthl = app.sfg_L.Value;
for i = 1:width
    indexl = i - 1;
    roundedl = floor(indexl / column_widthl);
    groupl = mod(roundedl, 2);
    if groupl == 0
        xl(:,i) = zeros(height,1);
    elseif groupl == 1
        xl(:,i) = ones(height,1);
    end
end






% if stim_img == app.sfg_R.Value
%     column_widthr = app.sfg_R.Value;
%     for i = 1:width
%         index = i - 1;
%         rounded = floor(index / column_width);
%         group = mod(rounded, 2);
%         if group == 0
%             x(:,i) = zeros(height,1);
%         elseif group == 1
%             x(:,i) = ones(height,1);
%         end
%     end
% else
%     column_width = app.sfg_L.Value;
%     for i = 1:width
%         index = i - 1;
%         rounded = floor(index / column_width);
%         group = mod(rounded, 2);
%         if group == 0
%             x(:,i) = zeros(height,1);
%         elseif group == 1
%             x(:,i) = ones(height,1);
%         end
%     end
% end







if lickdetected == 1
%    disp(stim_txt);
    str = 'L';
%    disp(stim_txt);
    if app.EasyMode.Value
        fullscreen2(background,2);
    elseif app.FollowLightMode.Value
%         fullscreen2(x,2);
%         if stim_img == app.sfg_R.Value
        if stim_img == 1
            fullscreen2(xr,2);
            fullscreen4(xr,4);
%             fullscreen3(xl,3);
        else
            fullscreen2(xl,2)
            fullscreen3(xl,3);
%             fullscreen4(xr,4);
        end
    elseif app.PatternMatchMode.Value
%         fullscreen2(x,2);
        if stim_img == 1
            fullscreen2(xr,2);
            fullscreen4(xr,4);
            fullscreen3(xl,3);
        else
            fullscreen2(xl,2)
            fullscreen3(xl,3);
            fullscreen4(xr,4);
        end
    else
        if stim_img == 1
            fullscreen2(xr,2);
        else
            fullscreen2(xl,2);
%         fullscreen2(x,2);
        end
    end

elseif lickdetected == 2
%    disp(stim_txt);
    str = 'R';
 %   disp(stim_txt);
    if app.EasyMode.Value
        fullscreen3(background,3);
    elseif app.FollowLightMode.Value
%         fullscreen3(x,3);
        if stim_img == 1
            fullscreen3(xr,3);
            fullscreen2(xr,2);
%             fullscreen4(xl,4);
        else
            fullscreen3(xl,3);
            fullscreen4(xl,4);
%             fullscreen2(xr,2);
        end
    elseif app.PatternMatchMode.Value
%         fullscreen3(x,3);
        if stim_img == 1
            fullscreen3(xr,3);
            fullscreen2(xr,2);
            fullscreen4(xl,4);
        else
            fullscreen3(xl,3);
            fullscreen4(xl,4);
            fullscreen2(xr,2);
        end
    else
        if stim_img == 1
            fullscreen3(xr,3);
        else
            fullscreen3(xl,3);
        end
    end
else
%    disp(stim_txt);
    str = 'C';
  %  disp(stim_txt);
    if app.EasyMode.Value
        fullscreen4(background,4);
    elseif app.FollowLightMode.Value
%         fullscreen4(x,4);
        if stim_img == 1
            fullscreen4(xr,4);
            fullscreen3(xr,3);
%             fullscreen2(xl,2);
        else
            fullscreen4(xl,4);
            fullscreen2(xl,2);
%             fullscreen3(xr,3);
        end
    elseif app.PatternMatchMode.Value
%         fullscreen4(x,4);
        if stim_img == 1
            fullscreen4(xr,4);
            fullscreen3(xr,3);
            fullscreen2(xl,2);
        else
            fullscreen4(xl,4);
            fullscreen2(xl,2);
            fullscreen3(xr,3);
        end
    else
        if stim_img == 1;
            fullscreen4(xr,4);
        else
            fullscreen4(xl,4);
        end
    end
end

% reward_Callback(app,str);
prevlick = lickdetected;
% lickdetected = 0;