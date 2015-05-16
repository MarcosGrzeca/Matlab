x = ones(1,10);
for n = 2:6
    x = x + 5;
end

a = a + b;
a = b / c;
a = a - b;
a = a - c / b + d * e; 

n = 1;
nFactorial = 1;
while (nFactorial < 1e100)
    n = n + 1;
    nFactorial = nFactorial * n;
end

while nFactorial < 1e100
    n = n + 1;
    nFactorial = nFactorial * n;
end

if (((x >= minVal) && (x <= maxVal) || (x == minVal)) || (x ~= minVal))
    b=x;
    fb=fx;
else
    a=x;
    fa=fx;
end
if ((x >= minVal) && (x <= maxVal) || (x == minVal))
    b=x;
    fb=fx;
    a=x;
else
    fa=fx;
end
x = teste(12);

i = 7;
for i=1:r
    k=1;
end