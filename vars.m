I1 = 1;
I2 = 1;
I3 = 1;
I4 = 1;
Rt1 = 1;
Rt2 = 1;
Rt3 = 1;
Cs = 1;
m = 1;
fi = 1;
Rx = 1;
Pn = 1;

m0 = (I1*I2*I3*I4)/(Rt1*Rt2*Rt3*Cs);
m1 = ((I2*I3*I4)/(Rt2*Rt3*Cs))+(((Rt3*I4+Rt2*I1*I3*(I2+I4))/(Rt1*Rt2*Rt3*Cs)));
m2 = (((I1+I3)*(I2*Rt2+I4*(Rt2+Rt3)))/(Rt2*Rt3*Cs))+((I1*I3*(Rt2+Rt3))/(Rt1*Rt3*Cs));
m3 = (((I1+I3)*(Rt2+Rt3))/(Rt3*Cs))+((I1*I2*I4)/(Rt1*Rt2*Rt3))+((I1*I2*I3)/(Rt1*Rt2*Rt3))-((I1*I3*Rt2)/(Rt1*Rt3*Cs));
m4 = ((2*I1*I2+I2*I3+I2*I4)/(Rt2*Rt3))+((I1*(I3+I4)*(Rt2+Rt3))/(Rt1*Rt2*Rt3))-((Rt2*(I1+I3))/(Rt3*Cs));
m5 = (((Rt3+Rt2)*(2*I1+I3+I4))/(Rt3*Rt2))+I2;
m6 = 1;
m7 = (2*pi*fi)/(Pn*m*Rx);