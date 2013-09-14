function [ d_state ] = control( t, state )
    x = state(1);
    v = state(2);
    f = 0;
    
    max_f = 1;
    
    target = 100;
    
    dist = target - x;
    dx0 = v^2/(2*max_f);
    dt0 = sqrt(2*dx0/max_f);
    half_dx = dx0 + dist / 2;
    half_dt = sqrt(2*half_dx/max_f);
    
    if(dt0 < (half_dt - 0.1))
        f = +max_f;
    else
        f = -max_f;
    end

    d_state = zeros(2, 1);
    d_state(1) = v;
    d_state(2) = f;
end

