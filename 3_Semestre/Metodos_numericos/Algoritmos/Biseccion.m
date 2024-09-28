%% Ingreso de datos
fx_str = input('Escriba la funci�n: ', 's');
fx = inline(fx_str);
a = input('Limite Inferior = ');
b = input('Limite Superior = ');
n = input('Numero de iteraciones = ');

% Identificamos si la raiz esta en un valor intermedio
if sign(fx(a)) * sign(fx(b)) > 0
    error('No se cumple el teorema del valor intermedio');
end

%% M�todo de Bisecci�n
cont = 0; % Contador de iteraciones

while cont < n
    m = (a + b) / 2;
    
    if sign(fx(a)) * sign(fx(m)) < 0 % Si hay un cambio de signo en la funci�n en fx(a)*fx(m)
        b = m;
    elseif sign(fx(m)) * sign(fx(b)) < 0 % Si hay un cambio de signo en la funci�n en fx(m)*fx(b)
        a = m;
    else
        fprintf('\n La raiz es %f\n\n', m);
        return; % Termina si encuentra la ra�z exacta
    end
    
    cont = cont + 1;
    
    fprintf('\nIteracion Numero %i', cont);
end

%% Impresi�n de resultados
fprintf('\n La raiz es = %f\n', m);
