function puntoFijo
    
    % Entrada de la funci�n como funci�n an�nima
    f_str = input('Escriba la funci�n: ', 's');
    fx = inline(f_str);
    x0 = input('Introduzca el valor x0 = ');
    imax = input('Introduzca el n�mero de iteraciones: ');
    maxerror = input('Introduzca el error maximo = ');
    
    % Inicializaci�n
    i = 1;
    
    % Bucle de iteraciones
    while i <= imax
        % Calcular la siguiente aproximaci�n
        x1 = fx(x0);
        
        % Verificar la convergencia
        if abs(x1 - x0) < maxerror
            fprintf('Convergencia alcanzada \nx = %f\tIteraci�n %d.\n', x1,i);
            return;
        end
        
        % Actualizar la aproximaci�n para la siguiente iteraci�n
        x0 = x1;
        
        % Incrementar el contador de iteraciones
        i = i + 1;
    end
    
    % Si no se alcanza la convergencia en el n�mero m�ximo de iteraciones
    disp('El m�todo de punto fijo no convergi� en el n�mero m�ximo de iteraciones.');
end
