function read_serial_rset(obj,event)
global sdata;
bav = get(obj,'BytesAvailable');

if bav > 0
sdata = fread(obj,bav);    
end