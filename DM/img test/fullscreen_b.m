function fullscreen_b(image,device_number)
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

global frame_java2;
global icon_java2;
global device_number_java2;

if ~isequal(device_number_java2, device_number)
    try frame_java2.dispose(); end
    frame_java2 = [];
    device_number_java2 = device_number;
end

if ~isequal(class(frame_java2), 'javax.swing.JFrame')
    frame_java2 = javax.swing.JFrame(gds(device_number).getDefaultConfiguration());
%     frame_java2 = javax.swing.JWindow(gds(device_number).getDefaultConfiguration());
    bounds = frame_java2.getBounds(); 
    frame_java2.setUndecorated(true);
    icon_java2 = javax.swing.ImageIcon(buff_image); 
    label = javax.swing.JLabel(icon_java2); 
    frame_java2.getContentPane.add(label);
    gds(device_number).setFullScreenWindow(frame_java2);
%     frame_java2.setSize( bounds.height, bounds.width );
    frame_java2.setLocation( bounds.x, bounds.y ); 
else
    icon_java2.setImage(buff_image);
end
frame_java2.pack
frame_java2.repaint
frame_java2.show

% notes:
% load image as variable
% resize image using imresize ('imagename', [1080,2920])
% use fullscreen function