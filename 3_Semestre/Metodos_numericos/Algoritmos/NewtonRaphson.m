% M�todo de Newton-Raphson para encontrar ra�ces de funciones

% Definir la variable simb�lica
syms x;

% Ingresar la funci�n y su derivada
fx = input('Introduzca f(x) = ');
dfx = diff(fx);

% Ingresar el valor inicial, n�mero de iteraciones y error m�ximo
x0 = input('x0 =  ');
i = input('Introduzca el n�mero de iteraciones: ');
maxerror = input('Introduzca el error m�ximo: ');

% Bucle de iteraciones del m�todo de Newton-Raphson
for k = 1:i
    % Calcular la siguiente aproximaci�n
    x1 = x0 - subs(fx, x0) / subs(dfx, x0);
    
    % Mostrar informaci�n sobre la iteraci�n actual
    fprintf('Iteraci�n %d: x%d = %f\n', k, k, x1);
    
    % Verificar la convergencia
    if abs(x1 - x0) < maxerror
        fprintf('Convergencia alcanzada. x%d = %f es una aproximaci�n de una ra�z\n', k, x1);
        return
    end
    
    % Actualizar el valor inicial para la pr�xima iteraci�n
    x0 = x1;
end
