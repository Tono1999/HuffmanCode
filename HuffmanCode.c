/*********************************************************
Aplicacion de arboles binarios haciendo uso del codigo de 
Huffman

By:
Angel Antonio Mendez Hernandez
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 


void contarFrecuenciaLetras(FILE *archivo, int frecuencia[]){
    // Inicializar el array de frecuencia a cero
    for(int i=0; i<26; i++){
        frecuencia[i]=0;
    }
    char bloque[1024]; // Tamaño del bloque a leer
    char caracter;
    unsigned long tamano=sizeof(bloque);
    unsigned long bytes, i;
    while(1){
        bytes=fread(bloque, 1, tamano, archivo);
        if (bytes==0)
            break; //Llegamos al final
        for(i=0; i<bytes; i++) {
            caracter=tolower(bloque[i]); //convertimos a minusculas
            if(isalpha(caracter))
                frecuencia[caracter-'a']++;//Sumamos a la frec correspondiente
        }
    }
}

//Estructura del nodo para el arbol
struct NodoHuffman {
    char letra;
    int frecuencia;
    struct NodoHuffman *izquierda, *derecha;
};

//Para crear nodos
struct NodoHuffman* crearNodo(char letra, int frecuencia){
    struct NodoHuffman* nodo = (struct NodoHuffman*)malloc(sizeof(struct NodoHuffman));
    nodo->letra = letra;//guardar letra
    nodo->frecuencia = frecuencia;//guardar frecuencia
    nodo->izquierda = nodo->derecha = NULL;//hacer que los hijos apunten a null
    return nodo;
}

//Para comparar las frecuencias de los nodos (para cuando haya que ordenar)
int compararNodos(const void* a, const void* b) {
    struct NodoHuffman* nodoA = *(struct NodoHuffman**)a;
    struct NodoHuffman* nodoB = *(struct NodoHuffman**)b;
    return nodoA->frecuencia - nodoB->frecuencia;
}

//Para construir el arbol
struct NodoHuffman* construirArbolHuffman(char letras[], int frecuencias[], int n){
    struct NodoHuffman *izquierda, *derecha, *raiz;
    //creamos las hojas con su respectiva letra y frecuencia
    int i;
    struct NodoHuffman* nodos[n];
    for(i=0; i<n; i++)
        nodos[i]=crearNodo(letras[i], frecuencias[i]);
    //empezamos a unir nodos
    while(n>1){
        //ordenamos los nodos
        qsort(nodos, n, sizeof(struct NodoHuffman*), compararNodos);
        //creamos el nodo combinando las dos frecuencias mas pequenas
        izquierda=nodos[0];
        derecha=nodos[1];
        raiz=crearNodo('*', (izquierda->frecuencia) + (derecha->frecuencia));
        //el * es porque ahi no hay letra jeje
        raiz->izquierda=izquierda;
        raiz->derecha=derecha;
        /*ajustamos las ligas de tal forma que, el nuevo nodo ocupe el primer lugar
        del arrglo sustituyendo a los nodos que le dieron origen*/
        nodos[0] = raiz;
        for (i=1; i<n-1; i++) {
            nodos[i]=nodos[i+1];
        }
        n--;
    }
    return nodos[0];
}

//para imprimir los codigos de cada letra
void imprimirCodigosHuffman(struct NodoHuffman* raiz, int codigo[], int pos){
    //verificamos si hay un hijo izquierdo
    if(raiz->izquierda){
        codigo[pos]=0;
        imprimirCodigosHuffman(raiz->izquierda, codigo, pos+1);
        /*si hay un hijo izquierdo, a la posicion actual del codigo de le asigna 0
        y se llama recursivamente a la funcion en la que estamos, pero ahora con el
        hijo izquierdo y con una posicion adelante del codigo*/
    }
    //verificamos si hay un hijo derecho
    if(raiz->derecha){
        codigo[pos]=1;
        imprimirCodigosHuffman(raiz->derecha, codigo, pos+1);
        /*misma dinamica que con el hijo izquierdo, pero al estar con el hijo derecho
        a la posicion de codigo se le asigna 1*/
    }
    //Si ya no hay hijos
    if(!raiz->izquierda && !raiz->derecha){
        printf("Letra: %c, Frecuencia: %d, Codigo de Huffman: ", raiz->letra, raiz->frecuencia);
        for (int i=0; i<pos; i++) {
            printf("%d", codigo[i]);
        }
        printf("\n");
    }
}

//para liberar la memoria
void liberarArbolHuffman(struct NodoHuffman* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarArbolHuffman(raiz->izquierda);
    liberarArbolHuffman(raiz->derecha);
    free(raiz);
}

int main() {
    int frecuenciaLetras[26];
    FILE *archivo;
    archivo=fopen("Don-Quijote-Ingles.txt", "r");
    //contamos la frecuencia de cada letra
    contarFrecuenciaLetras(archivo, frecuenciaLetras);
    //ya no usamos el archivo, entonces lo cerramos
    fclose(archivo);
    //creamos un arreglo con las letras
    char letras[26];
    for(int i=0; i<26; i++)
        letras[i]='a'+i;
    //construimos el arbol
    struct NodoHuffman* raiz = construirArbolHuffman(letras, frecuenciaLetras, 26);
    //imprimimos
    int codigo[100];
    int pos=0;
    printf("Códigos de Huffman:\n");
    imprimirCodigosHuffman(raiz, codigo, pos);
    // Liberar la memoria del árbol de Huffman
    liberarArbolHuffman(raiz);
    return 0;
}
