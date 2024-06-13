function read_serial(obj,event)
global sdata;
global lickdetected;

bav = get(obj,'BytesAvailable');
if bav > 0
    sdata = fread(obj,bav);    
%      fprintf('%4.3f %s \n',toc,sdata);   %GL edit added \n to string
    if contains(char(sdata'),'lckL')
        lickdetected = 1;
        fprintf('%4.3f lck_spout1 ts \n',toc);   %GL edit added \n to string
    elseif contains(char(sdata'),'lckR')
        lickdetected = 2;
        fprintf('%4.3f lck_spout2 ts \n',toc);   %GL edit added \n to string
    else
        lickdetected = 3;
        fprintf('%4.3f lck_spout3 ts \n',toc);   %GL edit added \n to string
    end
end