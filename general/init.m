function init(app)
global state;
global arduino;
global spout1pos spout2pos;
global light_value;
global previous_spout;
global rest_pause;
global randtable;
global lightval;
global vobj preview_active;
global write_index;
global reward_denied;
global sdata;
global audio_volume;
global run;

run = 0;

audio_volume = 0;
sdata  ={'test'};
reward_denied = 0;
write_index = 1;

% set_backgrounds(app);

tic;

d = uiprogressdlg(app.figure1,'Title','Please Wait', 'Message','Opening the application');
% imaqreset;

preview_active = 0;
% randtable = [0 0 ; 0 1; 1 0; 1 1 ; 1 2; 1 3; 2 0 ; 2 1];
randtable = [0 0 ; 0 1; 1 0; 1 1; 2 0 ; 2 1];

lightval = 1;
rest_pause = 1.0;
previous_spout = 0;

light_value = 3;

% spout1pos.init = 0;
% spout1pos.rest = str2double(app.sp1rest.Value);
% spout1pos.ext = str2double(app.sp1ext.Value);
% spout2pos.init = 0;
% spout2pos.rest = str2double(app.sp2rest.Value);
% spout2pos.ext = str2double(app.sp2ext.Value);

state = '0';
delete(instrfindall);

devs = fcom();
[r,c] = size(devs);

for i = 1:r
    [str,n] =  devs{i,:};
    if contains(string(str),'Arduino')
        break;
    end
end

comstr = ['COM' num2str(n)]; % ,'Mega'];
arduino=serial(comstr,'BaudRate',115200); % create serial communication object on port COM4
arduino.BytesAvailableFcnCount = 4;
arduino.BytesAvailableFcnMode = 'byte';
fopen(arduino); % initiate arduino communication

% wait until data from arduino
while 1
    bav = get(arduino,'BytesAvailable');
    if bav == 6
        break
    end
end

sdata = fread(arduino,bav);    
% make sure 'Rset' received
if ~contains(char(sdata'),'Rset')
    return;
end

% start serial reads in background
arduino.BytesAvailableFcn = {'read_serial'};

if isempty(app.animalid.Value)
        app.animalid.Value =  'animalid';
end

fprintf(' %s \n',sdata);   %GL edit added \n to string

% initialize the reward pulse and air puff durations
fprintf(arduino,'%c','q');
fprintf(arduino,'%c',round(str2double(app.r_pulse_C.Value)));
pause(0.05);
fprintf(arduino,'%c','v');
fprintf(arduino,'%c',round(str2double(app.r_pulse_L.Value)));
pause(0.05);
fprintf(arduino,'%c','w');
fprintf(arduino,'%c',round(str2double(app.r_pulse_R.Value)));
pause(0.05);
% fprintf(arduino,'%c','u');
% fprintf(arduino,'%c',round((app.airpuff_duration.Value)));
close(d);

% init_camera(hObject, eventdata, handles);
