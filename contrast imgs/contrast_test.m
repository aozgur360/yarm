height = 600
width = 1024

xr = zeros(height, width);
xl = zeros(height, width);

column_widthr = 100;
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
column_widthl = 100;
for i = 1:height
    indexl = i - 1;
    roundedl = floor(indexl / column_widthl);
    groupl = mod(roundedl, 2);
    if groupl == 0
        xl(i,:) = zeros(1,width);
    elseif groupl == 1
        xl(i,:) = ones(1,width);
    end
end


% notes:
% load image as variable, or jpg from paint so don't need to resize
% resize image using imresize ('imagename', [1080,1920])
% resize to 600 x 1024 for small screens
% use fullscreen function


%imshow(xr, [0 3])