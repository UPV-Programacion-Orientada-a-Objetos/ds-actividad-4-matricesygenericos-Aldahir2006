#include <iostream>
#include <cmath>

template <typename T>
class Campo2D {
private:
    T **_datos;
    int _filas;
    int _columnas;

    void liberarMemoria() {
        if (_datos) {
            for (int i = 0; i < _filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

    void asignarMemoria(int f, int c) {
        _filas = f;
        _columnas = c;
        _datos = new T*[f];
        for (int i = 0; i < f; ++i) {
            _datos[i] = new T[c];
        }
    }

public:
    Campo2D(int f, int c) : _datos(nullptr), _filas(0), _columnas(0) {
        asignarMemoria(f, c);
    }

    ~Campo2D() {
        std::cout << "Destructor invocado. Liberando memoria de la Matriz 2D..." << std::endl;
        liberarMemoria();
    }

    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            _datos[f][c] = valor;
        } else {
            std::cout << "Error: índice fuera de rango." << std::endl;
        }
    }

    T getValor(int f, int c) const {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas)
            return _datos[f][c];
        std::cout << "Error: índice fuera de rango." << std::endl;
        return T();
    }

    void mostrar() const {
        for (int i = 0; i < _filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < _columnas; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    void redimensionar(int nuevaF, int nuevaC) {
        std::cout << "Redimensionando Grid a " << nuevaF << "x" << nuevaC << "..." << std::endl;
        T **nuevaMatriz = new T*[nuevaF];
        for (int i = 0; i < nuevaF; ++i)
            nuevaMatriz[i] = new T[nuevaC];

        int minF = (nuevaF < _filas) ? nuevaF : _filas;
        int minC = (nuevaC < _columnas) ? nuevaC : _columnas;

        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                nuevaMatriz[i][j] = _datos[i][j];

        liberarMemoria();
        _datos = nuevaMatriz;
        _filas = nuevaF;
        _columnas = nuevaC;

        std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
    }

    T calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC) {
        if (inicioF < 0 || finF >= _filas || inicioC < 0 || finC >= _columnas) {
            std::cout << "Error: límites fuera de rango." << std::endl;
            return T();
        }

        T suma = 0;
        int contador = 0;

        for (int i = inicioF; i < finF; ++i) {
            for (int j = inicioC; j < finC; ++j) {
                if (i + 1 <= finF && j + 1 <= finC) {
                    T grad = (std::abs(_datos[i+1][j] - _datos[i][j]) + std::abs(_datos[i][j+1] - _datos[i][j])) / 2.0;
                    suma += grad;
                    contador++;
                }
            }
        }

        return (contador > 0) ? (suma / contador) : 0;
    }
};

int main() {
    std::cout << "--- Simulador Genérico de Campo 2D ---" << std::endl;

    Campo2D<float> campo(3, 3);

    std::cout << "Inicializando Campo Gravitatorio (Tipo FLOAT)" << std::endl;
    std::cout << "Creando Grid (FLOAT) de 3x3..." << std::endl;

    float valores[3][3] = {
        {10.0, 8.0, 5.0},
        {12.0, 9.0, 6.0},
        {15.0, 11.0, 7.0}
    };

    std::cout << "Estableciendo valores iniciales..." << std::endl;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            campo.setValor(i, j, valores[i][j]);

    std::cout << "Grid Actual (Paso 0):" << std::endl;
    campo.mostrar();

    std::cout << "\nOpción: Calcular Gradiente Promedio" << std::endl;
    int inicioF = 0, finF = 2, inicioC = 0, finC = 2;
    std::cout << "Ingrese Fila Inicial: " << inicioF << ", Fila Final: " << finF << std::endl;
    std::cout << "Ingrese Columna Inicial: " << inicioC << ", Columna Final: " << finC << std::endl;

    std::cout << "Calculando Gradiente Promedio en la región ["
              << inicioF << "," << finF << "]x[" << inicioC << "," << finC << "]..." << std::endl;

    float gradiente = campo.calcularGradientePromedio(inicioF, finF, inicioC, finC);
    std::cout << "Gradiente Promedio calculado: " << gradiente << " unidades/metro." << std::endl;

    std::cout << "\nOpción: Redimensionar" << std::endl;
    campo.redimensionar(4, 4);
    campo.mostrar();

    std::cout << "\nOpción: Redimensionar (A una dimensión menor)" << std::endl;
    campo.redimensionar(2, 2);
    campo.mostrar();

    std::cout << "\nOpción: Salir" << std::endl;
    // Destructor de mundos invocara automáticamente al salir de myndo.

    return 0;
}

