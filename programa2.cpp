#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cmath>
using namespace std;

void leerDatos( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos);
void muestraDatos( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos );
//Funcion Principal
void probabilidad( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos  );

//FUNCIONES AUXILIARES
void leerMatriz(double matriz[50][50], int filas, int columnas);
void potenciaMatriz(double matriz[50][50], int dimension, int potencia);
void muestraMatriz(double matriz[50][50], int filas, int columnas);
void igualaMatrices(double matriz[50][50], double matriz2[50][50], int dimension );

int main(){

    int estados;
    double matriz[50][50];
    int estadoInicial, estadoFinal;
    int periodos, repetir=0;

    do
    {
        system("cls");
        leerDatos( estados , matriz, estadoInicial, estadoFinal, periodos);
        muestraDatos( estados, matriz, estadoInicial, estadoFinal, periodos );
        probabilidad( estados, matriz, estadoInicial, estadoFinal, periodos ); 

        printf("\nDeseas reingresar otro problema, digite 1 para volver a usar, 0 en caso contrario: ");
        scanf("%d", &repetir);
    } while ( repetir == 1);   

    getch();
    return 0;
}

//FUNCIONES PRINCIPALES

void probabilidad( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos ){

    double resultadoFinal; 
    double matrizAux[50][50], matrizAux2[50][50]; //La primera matriz nos ayuda a calcular Pn(x,y) y la segunda nos ayuda con Pn-m(y,y)
    double resultados[1000];
    double pn_xy; //Probabilidad de partir de x y llegar a y en n periodos de tiempo es una variable auxiliar
    int i = 0, j = 0;
    int resultadoExtendido; //Te permite ir viendo las matrices elevadas a la potencia n, es un booleano si es 0 no las muestra si es 1 las muestra
    double suma;

    igualaMatrices(matriz, matrizAux, estados);
    igualaMatrices(matriz, matrizAux2, estados);

    printf("\nSi quieres ver las matrices elevadas a potencias n, digite 1, en caso contrario digite 0: ");
    scanf("%d", &resultadoExtendido);

    resultados[0] = matriz[estadoInicial][estadoFinal];
    printf("\nPx(Ty = 1) = %.3lf" , resultados[0]);

    for( i=2; i <= periodos ; i++ ){
        printf("\n\nCalculando Px(Ty = %d) ", i);
        igualaMatrices(matriz, matrizAux, estados);
        potenciaMatriz(matrizAux, estados, i );
        if ( resultadoExtendido == 1){
            printf("\nMatriz P a la n = %d" , i);
            muestraMatriz(matrizAux, estados,estados);
        }
        printf("\n");
        pn_xy = matrizAux[estadoInicial][estadoFinal];

        suma = 0;
        for( j = 1; j < i ; j++ ){
            igualaMatrices(matriz, matrizAux2, estados);
            potenciaMatriz( matrizAux2 , estados, i-j );
            if ( resultadoExtendido == 1){
                printf("\nMatriz P a la n  = %d" , i-j);
                muestraMatriz(matrizAux2, estados,estados);
            }
            suma += resultados[ j - 1] * matrizAux2[estadoFinal][estadoFinal] ;
        }
        // printf("\nSUMA: %lf" , suma);
        resultados[i-1] = pn_xy - suma;
        printf("\nPx(Ty = %d) = %.3lf" , i , resultados[i-1] );
    }

}

//FUNCIONES DE DATOS
void leerDatos( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos){

    printf("\nDigite el numero de estados que tiene tu proceso Estocastico: "); scanf("%d", &estados);

    printf("\nDigite la Matriz de transicion: ");
    leerMatriz(matriz, estados, estados);
    
    do{
        printf("\nConsidere antes de ingresar datos que el primer estado es el estado cero");
        printf("\nDigite el Estado Inicial: ");scanf("%d", &estadoInicial);
        printf("\nDigite el Estado Final: "); scanf("%d", &estadoFinal);
    }while(estadoInicial >= estados);
    
    do{
        printf("\nDigite el numero de periodos: ");
        scanf("%d", &periodos);
    } while ( periodos <= 1 );
}

void muestraDatos( int &estados, double matriz[50][50], int &estadoInicial, int &estadoFinal, int &periodos ){

    printf("\nTu proceso estocastico cuenta con %d estados, iniciamos en es estado %d y vamos al estado %d en %d periodos de tiempo\nMatriz de Transicion:\n", estados, estadoInicial, estadoFinal, periodos);
    muestraMatriz(matriz, estados, estados);


}

//FUNCIONES AUXILIARES
void muestraMatriz(double matriz[50][50], int filas, int columnas){
    int i,j;
    for (i = 0; i < filas; i++){
        printf("\n");
        for (j = 0; j < columnas; j++){
            printf("  %.3lf  " , matriz[i][j]);
        }
    }
}

void leerMatriz(double matriz[50][50], int filas, int columnas){
    int i=0,j=0, noNegativos = 0; //Esta variable permite que no existan valores negativos en la matriz de transicion
    double auxiliar;
    
    for (i = 0; i < filas; i++){
        printf("\n\nRenglon %d", i+1 );
        noNegativos = 0;
        auxiliar=0; //Esta variable ayua a que la matriz tenga renglones que suman 1

        for (j = 0; j < columnas; j++){
            printf("\nDigite la probabilidad de ir del estado %d al estado %d: " , i,j);
            scanf("%lf", &matriz[i][j]);
            if( matriz[i][j] < 0 ){
                noNegativos = 1;
            }
            auxiliar += matriz[i][j];
        }

        if( auxiliar !=1 || noNegativos == 1){
            printf("\nTU RENGLON NO SUMA 1 o tiene negativos, ESTA MAL ESTE RENGLON VUELVE A DIGITARLO");
            i--;
        }
    }
}

void potenciaMatriz(double matriz[50][50], int dimension, int potencia){

    int a = 0, i = 0, j = 0, k;
    double matrizAux[50][50];
    double matrizAux2[50][50];
    double suma = 0;

    igualaMatrices(matriz, matrizAux, dimension); //Es una copia de la matriz que recibe la funcion
    igualaMatrices(matriz, matrizAux2, dimension); //Es la matriz Pn-1

    for (k = 0; k < potencia - 1; k++)
    {
        for (a = 0; a < dimension; a++) {
            for (int i = 0; i < dimension; i++) {
                suma = 0;
                //printf("\n");
                for (int j = 0; j < dimension; j++) {
                    suma =suma + ( matrizAux2[i][j] * matrizAux[j][a] );
                    //printf("\n %.2lf, %.2lf, %.2lf, multi %.2lf",suma, matrizAux2[i][j], matrizAux[j][a], matrizAux2[i][j] * matrizAux[j][a] );
                }
                //printf("\n\nSUMA %.2lf" , suma);
                matriz[i][a] = suma;
            }
        }
        igualaMatrices(matriz, matrizAux2, dimension);
    }
    

}

void igualaMatrices(double matriz[50][50], double matriz2[50][50], int dimension ){
    //Esta funcion iguala la segunda matriz con los valores de la primera
    int i,j;

    for (i = 0; i < dimension; i++){
        for (j = 0; j < dimension; j++){
            matriz2[i][j] = matriz[i][j];
        }
    }
}