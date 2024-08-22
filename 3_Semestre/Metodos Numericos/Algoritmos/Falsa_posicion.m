%% Ingreso de datos y declaracion de variables
syms x;

% Entrada de datos
fx = input('Ingrese la funci�n f(x): ');
xl = input('Ingrese el valor inferior: ');
xu = input('Ingrese el valor superior: ');

% Entrada de error estimado e iteraciones
errorest = input('Ingrese el error estimado: ');

disp('--------------------------------------------------------');

% Para la gr�fica
xio = xl - 0.5;
xuo = xu + 0.5;

% Variables para la gr�fica
vxr = [];
vfxr = [];

% Evaluaci�n de fxl y fxu
fxl = subs(fx, x, xl);
fxu = subs(fx, x, xu);

%met = fxl * fxu;

%% Comprobacion de compatibilidad
%if met >= 0
 %   error('No cumple con las condiciones para llevarse a cabo.');
%end

%% Bucle principal de Falsa Poscion

erroraprox = 100;
i = 0;
xro = 0;

while (erroraprox > errorest)
    i = i + 1;
    xr = xu - (fxu * (xl - xu)) / (fxl - fxu);
    erroraprox = abs((xr - xro) / xr);
    xro = xr;

    % Mostrar informaci�n de la iteraci�n
    fprintf('Iteraci�n %i: \n', i)
    fprintf('xl = %2.6f   xu = %2.6f  xr = %2.6f \n', xl, xu, xr)
    fprintf('Ea = %.6f \n', erroraprox)
    disp('--------------------------------------------------------');

    % Evaluar fxr y almacenar valores para la gr�fica
    fxr = subs(fx, x, xr);
    vxr(i) = xr;
    vfxr(i) = fxr;

    % Actualizar xl y xu
    if (fxl * fxr) < 0
        xu = xr;
    elseif (fxl * fxr) > 0
        xl = xr;
    end

    % Actualizar fxl y fxu
    fxl = subs(fx, x, xl);
    fxu = subs(fx, x, xu);
end

%% Graficado
ezplot(fx, [xio, xuo])
hold on
plot(vxr, vfxr, 'm*', 'LineWidth', 2)
hold on
stem(xr, fxr, 'filled')
xlabel('Eje x');
ylabel('Eje y');