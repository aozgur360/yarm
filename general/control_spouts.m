function control_spouts(s1,s2)
global arduino;
global current_servo_position;
fprintf('%4.3f spouts_%d_%d   \n',toc, (s1),(s2));

fprintf(arduino,'%s',['i' char(s1)]);
fprintf(arduino,'%s',['j' char(s2)]);
current_servo_position = [s1,s2];




