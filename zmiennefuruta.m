M=1;
Il=1;
l=1;
ml=1;
Ir=1;
r=1;
g=1;
k=1;

detD = M*(Il*l + ml*l*l)*(Ir*r + M*r*r + ml*l*l);
a1 = (1/detD)*((-M*(M*r*l)+ml*l*r)*(-M*g*l-ml*g*l*0.5));
a2 = (1/detD)*(M*(Ir*r + M*r*r + ml*l*l)*(-M*g*l-ml*g*l*0.5));
a3 = (1/detD)*(M*l*l*(Il*l+l*l+ml*l*l)*M*g);
a4 = (1/detD)*(M*(Il*l+l*l+ml*l*l)*2*M + 2*M*l*(-M*l*l));
a5 = (1/detD)*(-M*M*r*(-M*l*l)-M*(M*r*l+ml*l*r)*(-2*M*l));
a6 = (1/detD)*((Ir*r+M*r*r+ml*l*l)*2*l*M*g+(-2*M*M*r*r*l-2*M*r*r*ml*l)*M*g+(-M*M*r*r*l*l-2*M*r*l*ml*l*r-ml*ml*l*l*r*r)*k+(Ir*r+M*r*r+ml*l*l)*(Il*l+l*l+ml*l*l)*k);
b1 = (1/detD)*(M*(Il*l+l*l+ml*l*l));
b2 = (1/detD)*(-M*(M*r*l+ml*l*r));
