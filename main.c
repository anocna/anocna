#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define RENGLON 2
#define COLUMNA 3
#define PBINGOUNO 500
#define PBINGODOS 250
#define PLINEAUNO 100
#define PLINEADOS 50


typedef struct {
    int     casilla[RENGLON][COLUMNA];
    char    nombre[20];                     // a quien pertenece el carton
    int     lin[2];                        //numero de lineas
    int     bin;                        //numero de bingo
    } Carton;

typedef struct {
    Carton  cartonUser[100];       // carton/es del usuario
    int     cantidad;           // numero cartones
    int     modoCarton;         // relleno manual o aleatorio
    int     numBingos;          // numero bingos cantados
    char    premioBingo1[20];        //primer bingo
    char    premioBingo2[20];        //segundo bingo
    char    premioLinea1[20];        //primer linea
    char    premioLinea2[20];        //segunda linea
} Bingo;


void cargarDatos        (Bingo *bingo); // PRE: Una estructura con formato Datos. POST: Carga los ingresados por el usuario.
void ordenamiento       (Carton *carton);
void mostrarCarton      (Carton *carton);
void ordenamientoBurbuja(int v[], int tam);
void llenarConCeros     (Carton *carton);
void desarrollo         (Bingo *bingo);
void verificarLinea     (Bingo *bingo, int *linea);
void verificarBingo     (Bingo *bingo, int *bin);
void multiplicarPuntos  (int bolillas, int *puntos);
void marcarBola         (Bingo *bingo, int num);



int  main (void) {  // FUNCION MAIN()

                    srand(time(NULL)); // planto semilla aleatoriedad

    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("|             >>>>     BIENVENIDOS AL BINGO     <<<<           |\n");
    printf("|                    (                      )                  |\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    Bingo bingo;

    cargarDatos(&bingo);


    // Desarrollo del juego
    desarrollo(&bingo);

    puts("\n");

	system("pause");
    return 0;

}



void cargarDatos(Bingo *bingo){

    int i, j, nuevoNumero, resultadoBusqueda;
    bingo->cantidad=0;

    int seguir=1;

    do{
        printf("Ingrese nombre:  ");
        scanf("%s",bingo->cartonUser[bingo->cantidad].nombre);

        do {
                printf("Seleccione:\n");
                printf("  [1] Carton cualquiera (aleatorio).\n"
                       "  [2] Carton personalizado (eligiendpo los numeros).\n");
                scanf("%d", &bingo->modoCarton);
            } while (bingo->modoCarton != 1 && bingo->modoCarton != 2);


    llenarConCeros(&bingo->cartonUser[bingo->cantidad]);


    if(bingo->modoCarton==1){

            for (i = 0; i<RENGLON; i++){

                for (j = 0; j<COLUMNA; j++ ){

                    do{
                        nuevoNumero = 1 +rand()%90;

                        resultadoBusqueda = busquedaSecuencial(&bingo->cartonUser[bingo->cantidad], nuevoNumero); //nos da la posicion ---> -1

                    }while(resultadoBusqueda!=-1);

                    bingo->cartonUser[bingo->cantidad].casilla[i][j]= nuevoNumero;
                }
            }


           // printf("\n\n se cargo el aleatorio");


    }else{

        printf("\nRellenemos su carton manualmente...\n");

            for (i = 0; i<RENGLON; i++){

                for (j = 0; j<COLUMNA; j++ ){

                     do{
                        do{
                            printf("\nIngrese numero ->  ");
                            scanf("%d",&nuevoNumero);
                            if(nuevoNumero<1 || nuevoNumero>90){
                                printf("Numero fuera del rango permitido.");
                            }
                        }while(nuevoNumero<1 || nuevoNumero>90);

                        resultadoBusqueda = busquedaSecuencial(&bingo->cartonUser[bingo->cantidad], nuevoNumero); //nos da la posicion ---> -1

                        if(resultadoBusqueda!=-1){
                            printf("No es posible repetir numero.");
                        }
                    }while(resultadoBusqueda!=-1);

                    bingo->cartonUser[bingo->cantidad].casilla[i][j]= nuevoNumero;
                }
            }


        }
        ordenamiento (&bingo->cartonUser[bingo->cantidad]);

        printf("\n\n|> Carton de %s <",bingo->cartonUser[bingo->cantidad].nombre);
        mostrarCarton(&bingo->cartonUser[bingo->cantidad]);



        bingo->cantidad+=1;

        do{
            printf("Continuar ingresando? \n > [1] Si \n > [2] No\n");
            scanf(" %d", &seguir);
        } while (seguir != 1 && seguir != 2);

    }while(seguir==1);

    printf("\n\n la cantidad de cartones en juego es de %d\n",bingo->cantidad);


}

int busquedaSecuencial(Carton *carton, int num){

    int pos = -1, i, j;

    for(i=0;i<RENGLON;i++){

        for (j = 0; j<COLUMNA; j++){

            if ( carton->casilla[i][j]==num){ //Lo encontre

                pos = i;

            }
        }
    }

    return pos;
}

void ordenamiento(Carton *carton){

    int x=(RENGLON*COLUMNA);
    int i=0, vec[x], r, c;

    for(r=0;r<RENGLON;r++){
        for(c=0;c<COLUMNA;c++){
            vec[i] = carton->casilla[r][c];
            i++;
        }
    }

    ordenamientoBurbuja(vec,x);

    i=0;
    for(c=0;c<COLUMNA;c++){
        for(r=0;r<RENGLON;r++){
            carton->casilla[r][c] = vec[i];
            i++;
        }
    }

}


void ordenamientoBurbuja(int v[], int tam){

    int aux;

    for ( int i = 0; i<tam; i++){

        for(int j =0; j<tam-1;j++){
            if(v[j]>v[j+1]){
                aux = v[j];
                v[j]=v[j+1];
                v[j+1]=aux;
            }


        }
    }

}


void mostrarCarton(Carton *carton){

    int i, j;

    for(i=0;i<RENGLON;i++){
        printf("|\n----------------------\n");
        for(j=0;j<COLUMNA;j++){
            printf("|  %.2d  ",carton->casilla[i][j]);
        }
    }
    printf("|\n----------------------\n");


}



void llenarConCeros(Carton *carton){

    int i, j;

    carton->bin=0;

    for (i = 0; i<RENGLON; i++){

        for (j = 0; j<COLUMNA; j++ ){
            carton->casilla[i][j]=0;

        }

            carton->lin[i]=0;
    }

}


void desarrollo(Bingo *bingo){

    int nBingo=0, nlinea=0, i=0, j, n, bola=0, bolillas[200],resultadoBusqueda;

    for(n=0;n<90;n++){  //llevo bolillero a cero
        bolillas[n]=0;
    }

    printf("\n\n Comencemos a sacar bolillas...\n");
    system("pause");

    while(nBingo<2){

        do{
            resultadoBusqueda=-1;
            bola = 1 +rand()%90;
            for(n=0;n<i;n++){
                if(bolillas[n]==bola){
                    resultadoBusqueda=n;
                }
            }

        }while(resultadoBusqueda!=-1);


        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n> > > > > > > Ha salido la bola numero: %d \n\n", bola);

        bolillas[i]=bola;
        i++;

        marcarBola(bingo,bola);

        verificarLinea(bingo, &nlinea);

        verificarBingo(bingo,&nBingo);


        printf("Cantidad de Lineas %d\n",nlinea);
        printf("Cantidad de Bingo %d\n",nBingo);


        system("pause");

    }


    printf("\n\n******   GANADORES   *******\n\n");
    printf(" %s GANO $%d con el Primer Bingo\n",bingo->premioBingo1,PBINGOUNO);
    printf(" %s GANO $%d con el Segundo Bingo\n",bingo->premioBingo2,PBINGODOS);
    printf(" %s GANO $%d con la Primera Linea\n",bingo->premioLinea1,PLINEAUNO);
    printf(" %s GANO $%d con la Segunda Linea\n",bingo->premioLinea2,PLINEADOS);


    printf("\n\n Salieron %d bolillas, estas fueron:\n", i);
    for(j=0;j<i;j++){
        printf(" [%2.d]",bolillas[j]);
    }
    printf(".-\n\n");

    printf(
         "\n            =========================================            "
         "\n========================|    G A M E    |========================"
         "\n========================|    O V E R    |========================"
         "\n            =========================================            ");


}



void verificarBingo(Bingo *bingo, int *bin){

    int suma, n;

    for(n=0;n<bingo->cantidad;n++){

    suma=0;

        for ( int i =0; i<RENGLON; i++){

            for ( int j= 0; j<COLUMNA; j++ ){

                //SUMA = SUMA +NUMERO
                suma = suma + bingo->cartonUser[n].casilla[i][j];
            }
        }
        if(suma==0 && bingo->cartonUser[n].bin==0){

            if(*bin==1){
                strcpy(bingo->premioBingo2,bingo->cartonUser[n].nombre);
                printf("\n\n ***************************************************************\n"
                       "*******    %s ha cantado el segundo BINGO, gano $ %d      *******\n"
                       " ***************************************************************\n\n",bingo->cartonUser[n].nombre,PBINGODOS);
                *bin+=1;
                bingo->cartonUser[n].bin+=1;
                printf("\n\n|>  Carton de %s <|",bingo->cartonUser[n].nombre);
                mostrarCarton(&bingo->cartonUser[n]);

            }else if(*bin==0){
                strcpy(bingo->premioBingo1,bingo->cartonUser[n].nombre);
                printf("\n\n ***************************************************************\n"
                       "*******    %s ha cantado el primer BINGO, gano $ %d       *******\n"
                       " ***************************************************************\n\n",bingo->cartonUser[n].nombre,PBINGOUNO);
                *bin+=1;
                bingo->cartonUser[n].bin+=1;
                printf("\n\n|>  Carton de %s <|",bingo->cartonUser[n].nombre);
                mostrarCarton(&bingo->cartonUser[n]);
            }
        }
    }
}

void verificarLinea (Bingo *bingo, int *linea){


    int i, j, n, v;

    for(n=0;n<bingo->cantidad;n++){
        for(i=0;i<RENGLON;i++){
            v=0;
            for(j=0;j<COLUMNA;j++){
                v+=bingo->cartonUser[n].casilla[i][j];
            }

            if(v==0 && bingo->cartonUser[n].lin[i]==0){



                if(*linea==0){

                    strcpy(bingo->premioLinea1,bingo->cartonUser[n].nombre);
                    printf("\n\n*******    %s ah cantado la primer linea, gana $ %d !!   ********\n",bingo->cartonUser[n].nombre,PLINEAUNO);
                    printf("\n\n|>  Carton de %s <|",bingo->cartonUser[n].nombre);
                    mostrarCarton(&bingo->cartonUser[n]);
                    *linea+=1;
                    bingo->cartonUser[n].lin[i]+=1;

                }else if (*linea==1){
                    strcpy(bingo->premioLinea2,bingo->cartonUser[n].nombre);
                    printf("\n\n*******    %s ah cantado la segunda linea, gana $ %d !!   *******\n",bingo->cartonUser[n].nombre,PLINEADOS);
                    printf("\n\n|>  Carton de %s <|",bingo->cartonUser[n].nombre);
                    mostrarCarton(&bingo->cartonUser[n]);
                    *linea+=1;
                    bingo->cartonUser[n].lin[i]+=1;
                }
            }
        }
    }
}


void marcarBola (Bingo *bingo, int num){

    int i, j, n;

    for(n=0;n<bingo->cantidad;n++){

        for(i=0;i<RENGLON;i++){

            for (j = 0; j<COLUMNA; j++){

                if ( bingo->cartonUser[n].casilla[i][j]==num){ //Lo encontre

                    bingo->cartonUser[n].casilla[i][j] = 0;

                    printf("\n\n ~~~~~~~~~  %s ha marcado el %d en su carton \n\n", bingo->cartonUser[n].nombre,num);
                }
            }
        }
    }
}

