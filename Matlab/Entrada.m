x = ones(1,10);
for n = 2:6
    x = x + 5;
end

for n = 2:0.5:6
    x = x + 5;
end

parfor i = 1:8
   A = i;
end

a = a + b;
a = b / c;
a = a - b;
a = a - (c / (b + d)) * e; 
a = -a;

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

try
    a = notaFunction(5,6);
catch
    warning('Problem using function.  Assigning a value of 0.');
    a = 0;
end

switch n
    case -1
        disp('negative one')
    case 0
        disp('zero')
    case 1
        disp('positive one')
    otherwise
        disp('other value')
end

a = a + b;
a = b / c;
a = a - b;
a = a - (c / (b + d)) * e; 
a = -a;