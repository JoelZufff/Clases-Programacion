% Calcular la regresi�n lineal por m�nimos cuadrados
function [a, Y, T] = MC(x, y)
    % Limpiar el espacio de trabajo y la pantalla de comandos
    clear all
    clc;

    % Solicitar al usuario que ingrese los valores de x y y separados por espacio
    x_input = input('Ingrese los valores de x separados por espacio: ', 's');
    y_input = input('Ingrese los valores de y separados por espacio: ', 's');

    % Mostrar una l�nea divisoria para mejorar la legibilidad
    disp('________________________________________________________________________')

    % Convertir los valores ingresados en vectores
    x = str2num(x_input);
    y = str2num(y_input);

    % Calcular la regresi�n lineal por m�nimos cuadrados
    n = length(x);
    sigmaX = sum(x);
    sigmaX2 = sum(x.^2);
    sigmaY = sum(y);
    sigmaXY = sum(x.*y);

    A = [n, sigmaX; sigmaX, sigmaX2];
    B = [sigmaY; sigmaXY];
    a = A\B;

    % Calcular las sumas de los cuadrados para el an�lisis de regresi�n
    Sr = sum((y - a(1) - a(2).*x).^2);
    St = sum((y - sigmaY/n).^2);
    es = sqrt(Sr/(n-2));
    cdet = (St - Sr)/St;
    ccor = sqrt(cdet);

    % Mostrar otra l�nea divisoria
    disp('________________________________________________________________________')

    % Crear una tabla de resultados para mostrar los c�lculos
    E1 = {'Sr'; 'St'; 'Error_estandar_del_estimado';...
          'Coeficiente_de_determinacion'; 'Coeficiente_de_correlacion'};
    E2 = [Sr; St; es; cdet; ccor];
    T = table(E1, E2, 'VariableNames', {'ValoresCalculados','Resultados'});

    % Mostrar los coeficientes de la regresi�n lineal
    disp('________________________________________________________________________')
    disp('Coeficientes de la regresi�n lineal:');
    disp(['a0 (independiente): ', num2str(a(1))]);
    disp(['a1 (pendiente): ', num2str(a(2))]);
    disp('________________________________________________________________________')

    % Mostrar los resultados de la regresi�n lineal para los datos de entrada
    disp('Resultados de la regresi�n lineal para los datos de entrada:');
    Y = a(1) + a(2) * x; % Calcular los valores estimados de y
    disp(['Y: ', num2str(Y)]);
    disp('________________________________________________________________________')

    % Mostrar la tabla de resultados
    disp('Tabla de resultados:');
    disp(T);
    disp('________________________________________________________________________')

    % Graficar los datos y la regresi�n lineal
    scatter(x, y, 'LineWidth', 1, 'MarkerEdgeColor', 'r');
    hold on;

    % Graficar la l�nea de regresi�n
    modelo = poly2sym(flipud(a)); % Crear la ecuaci�n de la l�nea de regresi�n
    f = matlabFunction(modelo); % Convertir la ecuaci�n en una funci�n
    fplot(f, [min(x), max(x)], 'LineWidth', 2, 'Color', 'k'); % Graficar la l�nea de regresi�n en negro

    grid on;
    xlabel('Eje X'); % Etiqueta del eje X
    ylabel('Eje Y'); % Etiqueta del eje Y
    title('Regresi�n Lineal por M�nimos Cuadrados'); % T�tulo del gr�fico

    % Asegurar que la leyenda se ajuste correctamente
    legend('Datos', 'Regresi�n Lineal', 'Location', 'Best');

    % Establecer la posici�n de la figura
    h = gcf;
    h.Position(1:2) = [765, 90];

    % Mostrar los coeficientes de la regresi�n linealizada
    disp('Coeficientes de la regresi�n linealizada:');
    if (num2str(a(1)) >= 0)
        disp(['y = ', num2str(a(2)), ' * x + ', num2str(a(1))]); % Mostrar el formato y = a*x+b
    else
        disp(['y = ', num2str(a(2)), ' * x ', num2str(a(1))]); % Mostrar el formato y = a*x+b   
    end
end
