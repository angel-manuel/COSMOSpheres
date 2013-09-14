function [ d_state ] = control( t, state )
    x = state(1);
    v = state(2);
    f = 0;
    
    max_f = 0.13;
    
    target = 1.6;
    
    dist = target - x;
    
    t_fren = v / max_f;
    t_fren_dist = sqrt(2*dist/max_f);

    if(t_fren < t_fren_dist)
        f = +max_f;
    else
        f = -max_f;
    end

    d_state = zeros(2, 1);
    d_state(1) = v;
    d_state(2) = f;
end

