I1 = 1;
I2 = 1;
I3 = 1;
cs1 = 50000;
cs2 = 50000;
Rt1 = 50;
Rt2 = 50;
Rt3 = 50;


a5=(I1*I2*I3)/(cs1*cs2);
a3=((I1*(Rt2*Rt3+I3*cs2+I2*cs2+I3*cs1)+Rt1*(I2*Rt3+I3*Rt2)+cs1*I2*I3))/(cs1*cs2);
a4=((I1*(I1*Rt3+I3*Rt2)+Rt1*I2*I3))/(cs1*cs2);
a2=(I1*(Rt3*cs1+Rt2*cs2+Rt3*cs2)+Rt1*(Rt2*Rt3+I3*cs2+I2*cs2+I3*cs1)+cs1*(I2*Rt3+I3*Rt2))/(cs1*cs2);
a1=((I1*cs1*cs2+Rt1*(Rt3*cs1+Rt2*cs2+Rt3*cs2)+cs1*(Rt1+Rt2+I3*cs2+I3*cs2+I2*cs2+I3*cs1)))/(cs1*cs2);
a0=((Rt1*cs1*cs2+cs1*(Rt2*cs2+cs1*(Rt2*cs2+Rt3*cs2))))/(cs1*cs2);

% a3 = I1*I2;
% a2 = (I2*Rt1 + I1*Rt2);
% a1 = (Rt1*Rt2 + I2 + I1*cs1);
% a0 = (Rt2 + Rt1*cs1);

