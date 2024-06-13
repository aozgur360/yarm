
clear all;


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


pause(1)
%a = arduino();
onArray = {};
offArray = {};
fprintf(arduino,'%c','+');  
fprintf(arduino,'%c',1);  
tic
for i = 1:20
    pause(30)
%     writeDigitalPin(arduino,'D6',1);
    fprintf(arduino,'%c','@');  %right
    fprintf(arduino,'%c',1);  %right
    onArray=[onArray,toc];
    pause(5)
%     writeDigitalPin(arduino,'D6',0);
    fprintf(arduino,'%c','@');  %right
    fprintf(arduino,'%c',0);  %right
    offArray=[offArray,toc];
    i
end
fprintf(arduino,'%c','+');  
fprintf(arduino,'%c',0);  

fclose(arduino);