function [next_spout] = get_next_spout(app) 

global number_of_consecutive_identical_spouts;
global number_of_consecutive_alternating_spouts;
global previous_spout;
global randbuf_left  randbuf_right randptr randtable lightval;
global randfornext;
global override_next_spout next_spout_value;
global correct_L correct_R bias_ptr bias_buffer_size;


if app.light2.Value
        if rand  >  (app.per_R.Value)/100
           next_spout = 0;    % left
        else
           next_spout = 1;    % right
        end

        if app.override.Value == 0
            if next_spout == previous_spout
                number_of_consecutive_alternating_spouts  =  0;    
                number_of_consecutive_identical_spouts = number_of_consecutive_identical_spouts  + 1;
                if number_of_consecutive_identical_spouts >= 2
                    number_of_consecutive_identical_spouts  =  0;
                    next_spout = xor(next_spout,1);
                end
            else
                number_of_consecutive_identical_spouts  =  0;
                number_of_consecutive_alternating_spouts = number_of_consecutive_alternating_spouts + 1;
                if number_of_consecutive_alternating_spouts >= 4
                    number_of_consecutive_alternating_spouts  =  0;
                    number_of_consecutive_identical_spouts = number_of_consecutive_identical_spouts  + 1;
                    next_spout = xor(next_spout,1);
                end
            end
        end 
        
        if next_spout == 0
            lightval = 0;
        else
            lightval = 7;
        end
            
else  % 6 panel case ----------------------------------------------------------------------------------------------------------------
   
%    6 panel case

    
    randfornext = rand;
    
    % percentage decided here
    
    %     6 panel mode
     if (100*randfornext) < (app.percentage_of_training_trials.Value)
    %         do 2 panel instead of 6 panel
    
       if rand  >  (app.per_R.Value)/100
           next_spout = 0;    % left
        else
           next_spout = 1;    % right
        end

        if app.override.Value == 0
            if next_spout == previous_spout
                number_of_consecutive_alternating_spouts  =  0;    
                number_of_consecutive_identical_spouts = number_of_consecutive_identical_spouts  + 1;
                if number_of_consecutive_identical_spouts >= 2
                    number_of_consecutive_identical_spouts  =  0;
                    next_spout = xor(next_spout,1);
                end
            else
                number_of_consecutive_identical_spouts  =  0;
                number_of_consecutive_alternating_spouts = number_of_consecutive_alternating_spouts + 1;
                if number_of_consecutive_alternating_spouts >= 4
                    number_of_consecutive_alternating_spouts  =  0;
                    number_of_consecutive_identical_spouts = number_of_consecutive_identical_spouts  + 1;
                    next_spout = xor(next_spout,1);
                end
            end
        end 
        
%         if next_spout == 0
%             lightval = 0;
%         else
%             lightval = 7;
%         end
    
     else
         
            idx = ceil(randptr/2);

            if mod(randptr,2) == 0  % even, odd
                lightval = randbuf_left(idx);    %odd
            else
                lightval = randbuf_right(idx);  % even
            end
        %     lightval = randbuf(randptr);

           if lightval < 4
                next_spout = 0; % left
            else
                next_spout = 1;  % right
            end

            randptr = 1 + mod(randptr,6);
            if randptr == 1
                randbuf_left = randperm(3);
                randbuf_right = 3 + randperm(3);
            end
         
         % do nothing
     end
    

end   % ---------------------------------------------------------------------------------------------------------------------------------
 
% see if bias correction active
if app.biason.Value
    %  override_next_spout next_spout_value;
    % do only if enough data
    if bias_ptr > bias_buffer_size
        correct_lefts = sum(correct_L((bias_ptr -bias_buffer_size+1 ):bias_ptr ));
        correct_rights =  sum(correct_R((bias_ptr -bias_buffer_size+1 ):bias_ptr ));
        
%         if the difference exceeds the limit then force
%         see if too different
        if (correct_lefts - correct_rights) > (app.bias_diff.Value)
%             too many more correct lefts, force  a right
            next_spout = 1;
            fprintf('!!!! right forced !!!!!!!!!!!!!!!!')
        elseif (correct_rights - correct_lefts) > (app.bias_diff.Value)
%             too many more correct rights, force  a left
            next_spout = 0;
            fprintf('!!!! left forced !!!!!!!!!!!!!!!!')
        end
    end
end

previous_spout = next_spout;

% fprintf('next spout =========%d========== \n',lightval);
