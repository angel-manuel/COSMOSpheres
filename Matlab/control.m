function [ d_state ] = control( t, state )
    x = state(1);
    v = state(2);
    
    max_f = 0.13;
    
    target = 1.6;
    
    dist = target - x;
    
    t_stop = abs(v / max_f);
    dist_stop = (max_f*t_stop^2)/2;
    
    if(2*dist_stop > dist)
        f = -max_f * min(1, t_stop);
    else
        f = +max_f;
    end
    
    if(v < 0) 
        f = -f;
    end

    d_state = zeros(2, 1);
    d_state(1) = v;
    d_state(2) = f;
end

