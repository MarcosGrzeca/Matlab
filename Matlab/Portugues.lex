algoritmo 'compiladores'

var
x, minVal, maxVal, b, fb, fx, a, fa, i, r, k : real

inicio
x<-10;
minVal<-14;
maxVal<-50;

SE (((x>=minVal) e (x<=maxVal) ou (x = minVal)) ou (x <> minVal)) ENTAO
	b<-x;
fb<-fx;
SENAO
	a<-x;
fa<-fx;
FIMSE

SE ((x>=minVal) e (x<=maxVal) ou (x = minVal)) ENTAO
	b<-x;
fb<-fx;
a<-x;
SENAO
	fa<-fx;
FIMSE
x<-teste(12);
i<-7;
PARA i<-1:r
	k<-1;
FIMPARA

fimalgoritmo
