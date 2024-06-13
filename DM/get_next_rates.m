 function [next_spout] = get_next_rates(app) 
global rate_pair;
global rtable reward_spout;
global randbuf randptr;
global rate;

if rand  >  (app.per_R.Value)/100
   next_spout = 0;    % left
else
   next_spout = 1;    % right
end

if app.two_rates_only.Value
%     tone_pair
    [n,~] =  size(rate_pair);
        idx = ceil(n*rand);  % choose one pair
        
        if app.higher_rate_correct.Value
            if next_spout ==0  % left
                rate.r = rate_pair(idx,1);
                rate.l = rate_pair(idx,2);
            else
                rate.r = rate_pair(idx,2);
                rate.l = rate_pair(idx,1);
            end
        else
            if next_spout  ==0 % left
                rate.r = rate_pair(idx,2);
                rate.l = rate_pair(idx,1);
            else
                rate.r = rate_pair(idx,1);
                rate.l = rate_pair(idx,2);
            end
        end
        
else   % ---------------------------------------------------------------
 %     mixed rates  case
 
      if  rand < app.percentage_of_training_trials.Value/100
            %       training trial case
            qq = rtable{1};
            h = max(qq(1,:));
            l = min(qq(1,:));

            if app.higher_rate_correct.Value
                if next_spout  == 0 % left
                    rate.l = h;
                    rate.r = l;
                else
                    rate.l = l;
                    rate.r = h;
                end
            else
                if next_spout  == 0 % left
                    rate.l = l;
                    rate.r = h;
                else
                    rate.l = h;
                    rate.r = l;
                end
            end
      else    % non training case
                idx  =randbuf(randptr)
%                update pointers for future use
                  randptr = 1 + mod(randptr,length(randbuf));
                  if randptr == 1
                         randbuf = randperm(length(randbuf));
                  end
                  
%                pick on difficulty level
                  idx = idx + 1;  % do not use 4:1  or  1:4  (training mode rates)
                  qq = rtable{idx};
                  [n,c] = size(qq);  % n possible rate pairs at this difficulty level
%                   pick one pair of n
                  k = randperm(n);
                  i = k(1);                  
                  rate.l = qq(i,1);
                  rate.r = qq(i,2);
                  
                  if rate.l > rate.r
                       if app.higher_rate_correct.Value
                               next_spout = 0;    % left
                       else
                               next_spout = 1;    % right
                       end
                  else
                       if app.higher_rate_correct.Value
                               next_spout = 1;    % right
                        else
                               next_spout = 0;    % left
                        end
                  end
      end
end
% 
% 

fprintf('%4.3f  blink rates  l = %d    r = %d   \n',toc,rate.l,rate.r);