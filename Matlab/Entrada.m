switch n
    case -1
        x = 5;
		k = k+1;
    case 0
		x = 1
    case 1
        x = 2;
    otherwise
        x = 5;
        switch n
            case -1
                x = 5;
                k = k+1;
            case 0
                x = 1
            case 1
                x = 2;
            otherwise
                x = 5;
        end
end

switch n
    case -1
        x = 5;
    case 0
        x = 5;
    case 1
        x = 5;
end

if ((x >= minVal) && (x <= maxVal))
    x=x+1;
elseif (x > maxVal)
    x=x+1;
else
    x=x+1;
end