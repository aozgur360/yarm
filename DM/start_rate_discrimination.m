function start_rate_discrimination(app)    

global arduino rate;
%     start rate discrimination

    left_half_cycles = round(2*app.stim_duration_rate_disc.Value * rate.l);
    right_half_cycles = round(2*app.stim_duration_rate_disc.Value * rate.r);
    left_halfcycle_in_ms = round(0.5*1000/rate.l);
    right_halfcycle_in_ms = round(0.5*1000/rate.r);
    lhcl =  mod(left_halfcycle_in_ms,256); 
    lhch =  floor(left_halfcycle_in_ms/256); 
    rhcl =  mod(right_halfcycle_in_ms,256); 
    rhch =  floor(right_halfcycle_in_ms/256); 
    
    if app.Light.Value
%         light stimulus
            single_panel_L = 0;
            single_panel_R = 0;

            if app.single_panel.Value
                if rate.l > rate.r
                    if app.higher_rate_correct.Value
                        single_panel_R = 1;
                    else
                        single_panel_L = 1;
                    end
                else
                    if app.higher_rate_correct.Value
                            single_panel_L = 1;
                    else
                            single_panel_R = 1;
                end
                end
            end
    else
%         tone stimulus
            single_panel_L = 10;   %to indicate tone
            single_panel_R = 10;
    end

%     odd number of half cycles
    if  mod(left_half_cycles,2)  ==  0
        left_half_cycles = left_half_cycles -1;
    end
    if  mod(right_half_cycles,2)  ==  0
        right_half_cycles = right_half_cycles -1;
    end
    
    fprintf(arduino,'%c',['b' lhcl lhch  rhcl rhch  left_half_cycles  right_half_cycles single_panel_L  single_panel_R]);
