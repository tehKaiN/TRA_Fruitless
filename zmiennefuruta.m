% Parametry
M=1;
Il=0.03;
l=0.3;
ml=1;
Ir=0.083;
r=0.5;
g=9.81;
k=1000;

% Obliczenie wartoœci macierzy A
detD = M*(Il*l + ml*l*l)*(Ir*r + M*r*r + ml*l*l);
a1 = (1/detD)*((-M*(M*r*l)+ml*l*r)*(-M*g*l-ml*g*l*0.5));
a2 = (1/detD)*(M*(Ir*r + M*r*r + ml*l*l)*(-M*g*l-ml*g*l*0.5));
a3 = (1/detD)*(M*l*l*(Il*l+l*l+ml*l*l)*M*g);
a4 = (1/detD)*(M*(Il*l+l*l+ml*l*l)*2*M + 2*M*l*(-M*l*l));
a5 = (1/detD)*(-M*M*r*(-M*l*l)-M*(M*r*l+ml*l*r)*(-2*M*l));
a6 = (1/detD)*((Ir*r+M*r*r+ml*l*l)*2*l*M*g+(-2*M*M*r*r*l-2*M*r*r*ml*l)*M*g+(-M*M*r*r*l*l-2*M*r*l*ml*l*r-ml*ml*l*l*r*r)*k+(Ir*r+M*r*r+ml*l*l)*(Il*l+l*l+ml*l*l)*k);
A = [0 1 0 0 0 0;0 0 a1 0 a4 0;0 0 0 1 0 0; 0 0 a2 0 a5 0;0 0 0 0 0 1;0 0 a3 0 a6 0];

% Obliczenie wartoœci wektora B
b1 = (1/detD)*(M*(Il*l+l*l+ml*l*l));
b2 = (1/detD)*(-M*(M*r*l+ml*l*r));
B=[0;b1;0;b2;0;0];

% Obserwator
C=[0 0 1 0 0 0];
D=0;

% Przygotowanie modelu do symulacji
furuta = ss(A,B,C,D);

t = 0:0.1:100;
u = [0*t];
x0 = [0 0 0 0 10 0];
figure(1)

% Symulacja modelu S-S
lsim(furuta,u,t,x0)

K = place (A,B, [-0.9 -0.8 -0.7 -0.6 -0.5 -0.4]);
ster = ss(A-B*K,[],C,0);
figure(2)
[Y,T,x]=lsim(ster,u,t,x0);
plot(T,Y)


% Wspó³czynniki transmitancji
tran = ss2tf(A,B,C,D);