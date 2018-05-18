clear

% x and y matrixes
xMatrix = [265 0 0 0 0 0 0 0 0; 
    0 400 0 0 0 0 0 0 0;
    0 0 500 0 0 0 0 0 0;
    0 0 0 700 0 0 0 0 0;
    0 0 0 0 950 0 0 0 0; 
    0 0 0 0 0 1360 0 0 0;
    0 0 0 0 0 0 2080 0 0;
    0 0 0 0 0 0 0 2450 0;
    0 0 0 0 0 0 0 0 2940];

yMatrix = [1025; 1400; 1710; 2080; 2425; 2760; 3005; 2850; 2675];

% x and y points
x = [265 400 500 700 950 1360 2080 2450 2940]; 
y = [1025 1400 1710 2080 2425 2760 3005 2850 2675];
size = length(x); %Size of x matrix
% Initialize A and B matrixes
AMatrix = zeros(3*size - 3, 3*size - 3);
BMatrix = zeros(3*size - 3, 1);

temp = 1; %Counter
temp2 = size; %Counter

%% First Part

%Coefficients of the equation Ax^2+B*x+C;
for i = 1:size-1
    for j=1:3 % 3 Values to be stored in A matrix
    AMatrix(temp, (j-1)*(size-1)+i) = x(i)^(3-j);
    AMatrix(temp2, (j-1)*(size-1)+i) = x(i + 1)^(3-j);
    end
    BMatrix(temp) = y(i); % Assign y values to B matrix
    BMatrix(temp2) = y(i + 1);%Write next values to the B matrixx
    temp = temp + 1; %incrementation.
    temp2 = temp2 + 1; %incrementation.
end

%Put the final values of coeficients to the A matrix
for i = 1:(size-2)
   for j=0:1
       for k=0:1
           if j==0
              AMatrix(temp2, j*(size-1)+i+k) = 2*x(i+1)*((-1)^k);
           else
              AMatrix(temp2, j*(size-1)+i+k) = (-1)^k;
           end
       end
   end
    temp2 = temp2 + 1; %incrementation.
end

%Adding values to be able to plot, it must be in same length
AMatrix(temp2, 1) = 1;
BMatrix(temp2, 1) = 0;

%linsolve method to find coeficients
coeficients = linsolve(AMatrix, BMatrix);

%Coefficient matrices of Ax^2+B*x+c
A_cof = coeficients(1:size-1);
B_cof = coeficients(size:2*size-2);
C_cof = coeficients(2*size-1:end);

% to maximaze the window to see clear
figure('Name', 'Quadratic Spline Interpolation', 'units', 'normalized', 'outerposition', [0 0 1 1]); 
subplot(1,2,1), plot(x(2:end), y(2:end), '*'); hold on

result = linspace(x(2), x(3),300); %Linspace Method
plot(result, A_cof(2)*result.^2 + B_cof(2)*result + C_cof(2)); hold on %Plot the quadratic equation function
result = linspace(x(3), x(4),300); %Linspace Method
plot(result, A_cof(3)*result.^2 + B_cof(3)*result + C_cof(3)); hold on %Plot the quadratic equation function
result = linspace(x(4), x(5),300); %Linspace Method
plot(result, A_cof(4)*result.^2 + B_cof(4)*result + C_cof(4)); hold on %Plot the quadratic equation function
result = linspace(x(5), x(6),300); %Linspace Method
plot(result, A_cof(5)*result.^2 + B_cof(5)*result + C_cof(5)); hold on %Plot the quadratic equation function
result = linspace(x(6), x(7),300); %Linspace Method
plot(result, A_cof(6)*result.^2 + B_cof(6)*result + C_cof(6)); hold on %Plot the quadratic equation function
result = linspace(x(7), x(8),300); %Linspace Method
plot(result, A_cof(7)*result.^2 + B_cof(7)*result + C_cof(7)); hold on %Plot the quadratic equation function
result = linspace(x(8), x(9),300); %Linspace Method
plot(result, A_cof(8)*result.^2 + B_cof(8)*result + C_cof(8)); hold on %Plot the quadratic equation function

title('Quadratic Spline');
xlabel('Microstrain'); ylabel('Stress (psi)');
xlim([0 3000]); %Limit of x
ylim([0 3500]); %Limit of y



%% Second Part 

xx = 265: 1 : 2940;
ww = 3.8679 : 0.9099;

p = 1/(1+9000*10^3); % p value to be use in csaps function
yy = csaps(x,y,p,xx); % smoothing function csaps
subplot(1,2,2), plot(x,y,'ok'); hold on; % (x,y) pairs coordinate
subplot(1,2,2), plot(xx, yy, 'k'); hold off; % Actual data plot
xlabel('Microstrain'); ylabel('Stress (psi)'); xlim([0 3000]); ylim([0 3500]); %Figure's size and names
title('Actual Plot');