
% Entrada de la funci�n como funci�n an�nima
f_str = input('Escriba la funci�n: ', 's');
f = inline(f_str);

% Valores iniciales y criterio de convergencia
x1 = input('Introduzca el valor x0 = ');
x2 = input('Introduzca el valor x1 = ');
maxerror = input('Introduzca el error maximo = ');

% Gr�fico de la funci�n
ezplot(f);
grid on;

% Inicializaci�n de variables
ea = 100;
i = 0;

fprintf('Iteraci�n\tRa�z\n');

% Bucle de iteraciones
while ea > maxerror
    xi = x2 - ((f(x2) * (x1 - x2)) / (f(x1) - f(x2)));
    ea = abs((xi - x2) / xi);
    
    % Impresi�n de resultados
    fprintf('%d\t\t%8.4f\n', i, xi);
    
    % Actualizaci�n de valores para la siguiente iteraci�n
    x1 = x2;
    x2 = xi;
    i = i + 1;
end

fprintf('\nLa ra�z de la funci�n: %8.4f\nCalculada en %d iteraciones\n', xi, i);