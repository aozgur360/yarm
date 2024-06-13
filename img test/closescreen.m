function closescreen()
%CLOSESCREEN Dispose FULLSCREEN() window
%

global frame_java
global frame_java2
global frame_java3
try frame_java3.dispose();end
try frame_java2.dispose();end
try frame_java.dispose(); end

