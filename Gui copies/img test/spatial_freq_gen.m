
% %add code:
% %when stim is L, n = ... (from gui), get spatial frequency generated
% %image...sfg = getvalue from gui
% %when stim is R, n = ... (from gui)
% %replace stim_img with x


% app.sfg_L.Value

width = 1024;
height = 600;
column_width = 100;

x = zeros(height, width);

if stim_img = app.sfg_L.Value
    column_width = app.sfg_L.Value;
    for i = 1:width
        index = i - 1;
        rounded = floor(index / column_width);
        group = mod(rounded, 2);
        if group == 0
            x(:,i) = zeros(height,1);
        elseif group == 1
            x(:,i) = ones(height,1);
        end
    end
else
    column_width = app.sfg_R.Value;
    for i = 1:width
        index = i - 1;
        rounded = floor(index / column_width);
        group = mod(rounded, 2);
        if group == 0
            x(:,i) = zeros(height,1);
        elseif group == 1
            x(:,i) = ones(height,1);
        end
    end



















function fullscreen2(image,device_number)
%FULLSCREEN Display fullscreen true colour images
%   FULLSCREEN(C,N) displays matlab image matrix C on display number N
%   (which ranges from 1 to number of screens). Image matrix C must be
%   the exact resolution of the output screen since no scaling in
%   implemented. If fullscreen is activated on the same display
%   as the MATLAB window, use ALT-TAB to switch back.
%
%   If FULLSCREEN(C,N) is called the second time, the screen will update
%   with the new image.
%
%   Use CLOSESCREEN() to exit fullscreen.
%
%   Requires Matlab 7.x (uses Java Virtual Machine), and has been tested on
%   Linux and Windows platforms.
%
%   Written by Pithawat Vachiramon
%
%   Update (23/3/09):
%   - Uses temporary bitmap file to speed up drawing process.
%   - Implemeted a fix by Alejandro Camara Iglesias to solve issue with
%   non-exclusive-fullscreen-capable screens.


ge = java.awt.GraphicsEnvironment.getLocalGraphicsEnvironment();
gds = ge.getScreenDevices();
height = gds(device_number).getDisplayMode().getHeight();
width = gds(device_number).getDisplayMode().getWidth();

if ~isequal(size(image,1),height)
    error(['Image must have verticle resolution of ' num2str(height)]);
elseif ~isequal(size(image,2),width)
    error(['Image must have horizontal resolution of ' num2str(width)]);
end

try
    imwrite(image,[tempdir 'display.bmp']);
catch
    error('Image must be compatible with imwrite()');
end

buff_image = javax.imageio.ImageIO.read(java.io.File([tempdir 'display.bmp']));

global frame_java;
global icon_java;
global device_number_java;

if ~isequal(device_number_java, device_number)
    try frame_java.dispose(); end
    frame_java = [];
    device_number_java = device_number;
end

if ~isequal(class(frame_java), 'javax.swing.JFrame')
    frame_java = javax.swing.JFrame(gds(device_number).getDefaultConfiguration());
%     frame_java = javax.swing.JWindow(gds(device_number).getDefaultConfiguration());
    bounds = frame_java.getBounds(); 
    frame_java.setUndecorated(true);
    icon_java = javax.swing.ImageIcon(buff_image); 
    label = javax.swing.JLabel(icon_java); 
    frame_java.getContentPane.add(label);
    gds(device_number).setFullScreenWindow(frame_java);
%     frame_java.setSize( bounds.height, bounds.width );
    frame_java.setLocation( bounds.x, bounds.y ); 
else
    icon_java.setImage(buff_image);
end
frame_java.pack
frame_java.repaint
frame_java.show

% notes:
% load image as variable, or jpg from paint so don't need to resize
% resize image using imresize ('imagename', [1080,1920])
% resize to 600 x 1024 for small screens
% use fullscreen function
end