#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define RENGLON 3
#define COLUMNA 5

typedef struct {
    char    nombre[20];
    char    apellido[20];
    char    dni[10];
    int     puntosPc;
    int     puntosUser;
    } Datos;

typedef struct {
    int     casilla[RENGLON][COLUMNA];
    char    id[20];                     // a quien pertenece el carton
    int     num;                        //numero de carton
    int     lin;                        //numero de lineas
    int     col;                        //numero de columnas
    } Carton;

typedef struct {
    Carton cartonUser[3];       // carton/es del usuario
    Carton cartonPc[3];         // carton/es de la pc
    //Carton espejoPc[3];
    //Carton espejoUser[3];
    int     cantidad;           // numero cartones
    int     modoCarton;         // relleno manual o aleatorio
    int     modoJuego;          // generar juego completo o sacar bolillas de a una
    int     numBingos;          // numero bingos cantados
} Bingo;

typedef struct { int *numeros; } Partida; // Guarda los numeros que aparecen durante la partida


void cargarDatos        (Datos *datos); // PRE: Una estructura con formato Datos. POST: Carga los ingresados por el usuario.
void iniciarPartida     (Bingo *bingo); // PRE: Una estructura con formato Bingo. POST: Permite definir el numero de cartones, y modo de juego.
void generarCartones    (Bingo *bingo, Datos *datos);// PRE: Necesita . POST: carga los ingresados por el usuario.
void ordenamiento       (Carton *carton);
void mostrarCarton      (Carton *carton);
void ordenamientoBurbuja(int v[], int tam);
void llenarConCeros     (Bingo *bingo);
void desarrollo         (Bingo *bingo, Datos *datos);
void verificarLinea     (Carton *carton, int *puntos, int *linea,int c);
void verificarColumna   (Carton *carton, int *puntos, int *col,int c);
void verificarBingo     (Carton *carton, int *puntos, int *bin,int c);
void multiplicarPuntos  (int bolillas, int *puntos);
void marcarBola         (Carton *carton, int num,int c);



int  main (void) {  // FUNCION MAIN()

                    srand(time(NULL)); // planto semilla aleatoriedad

    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("|             >>>>     BIENVENIDOS AL BINGO     <<<<           |\n");
    printf("|                    (Grupo 12 - UNLa - 2020)                  |\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    Bingo bingo;
    Partida partida;
    Datos datos;

    cargarDatos(&datos);

    iniciarPartida(&bingo);

     /* Prepara los cartones */
    generarCartones(&bingo,&datos);

    /* Desarrollo del juego */
    desarrollo(&bingo, &datos);

    puts("\n");
    return 0;

}



void cargarDatos(Datos *datos){

    printf("Ingrese nombre:  ");
    scanf("%s",datos->nombre);
    printf("\nIngrese apellido:  ");
    scanf("%s",datos->apellido);
    printf("\nIngrese DNI:  ");
    scanf("%s",datos->dni);
}



void iniciarPartida (Bingo *bingo){

    int i;

    // Solicita al usuario el numero de cartones a jugar
    do {
            printf("Seleccione el nucmero de cartones a jugar:\n   [1]\n   [2]\n   [3]\n");
            scanf("%d", &bingo->cantidad); printf("\n");

        } while (bingo->cantidad < 1 || bingo->cantidad > 3);


    // Escoge el modo de relleno de cartones en juego

    do {
            printf("Seleccione:\n");
            printf("  [1] Comprar carton cualquiera (aleatorio).\n"
                   "  [2] Comprar carton personalizado (eligiendpo los numeros).\n");
            scanf("%d", &bingo->modoCarton);
        } while (bingo->modoCarton != 1 && bingo->modoCarton != 2);

    do {
            printf("Seleccione modo de Juego:\n");
            printf("  [1] Generar juego completo.\n"
                   "  [2] Sacar bolillas de una a la vez.\n");
            scanf("%d", &bingo->modoJuego);
        } while (bingo->modoCarton != 1 && bingo->modoCarton != 2);



}


void generarCartones (Bingo *bingo, Datos *datos){ //Construye los cartones

    int i, j, n;
    int resultadoBusqueda=0;
    int nuevoNumero=0;

    llenarConCeros(bingo);

    printf(" Cartones en juego:  \n");

    for(n=0;n < (bingo->cantidad);n++){

    //printf("\n");

        for (i = 0; i<RENGLON; i++){
                //printf("\n");

            for (j = 0; j<COLUMNA; j++ ){

                do{
                    nuevoNumero = 1 +rand()%90;

                    resultadoBusqueda = busquedaSecuencial(&bingo->cartonPc[n].casilla, nuevoNumero); //nos da la posicion ---> -1


                }while(resultadoBusqueda!=-1);

                bingo->cartonPc[n].casilla[i][j]= nuevoNumero;
                //bingo->espejoPc[n].casilla[i][j]= nuevoNumero;
            }
        }
 /* Asigna un identificador (id) a cada carton */
        bingo->cartonPc[n].num = n+1;
        strcpy(bingo->cartonPc[n].id,"PC");

        ordenamiento (&bingo->cartonPc[n]);
        //ordenamiento (&bingo->espejoPc[n]);


    }

    //+++++++++++++++ relleno carton segun sea aleatorio o personalizado

    resultadoBusqueda=0;

    nuevoNumero=0;

    for(n=0;n < (bingo->cantidad);n++){

        if(bingo->modoCarton==1){

            for (i = 0; i<RENGLON; i++){

                for (j = 0; j<COLUMNA; j++ ){

                    do{
                        nuevoNumero = 1 +rand()%90;

                        resultadoBusqueda = busquedaSecuencial(&bingo->cartonUser[n].casilla, nuevoNumero); //nos da la posicion ---> -1

                    }while(resultadoBusqueda!=-1);

                    bingo->cartonUser[n].casilla[i][j]= nuevoNumero;
                    //bingo->espejoUser[n].casilla[i][j]= nuevoNumero;
                }
            }
     /* Asigna un identificador (id) a cada carton */
            strcat(bingo->cartonUser[n].id,datos->nombre);
            bingo->cartonUser[n].num=n+1;


            ordenamiento (&bingo->cartonUser[n]);
            //ordenamiento (&bingo->espejoUser[n]);


    }else{

        printf("\nRellenemos su carton manualmente...\n");

            for (i = 0; i<RENGLON; i++){
                    //printf("\n");

                for (j = 0; j<COLUMNA; j++ ){

                     do{
                        do{
                            printf("\nIngrese numero ->  ");
                            scanf("%d",&nuevoNumero);
                            if(nuevoNumero<1 || nuevoNumero>90){
                                printf("Numero fuera del rango permitido.");
                            }
                        }while(nuevoNumero<1 || nuevoNumero>90);

                        resultadoBusqueda = busquedaSecuencial(&bingo->cartonUser[n].casilla, nuevoNumero); //nos da la posicion ---> -1

                        if(resultadoBusqueda!=-1){
                            printf("No es posible repetir numero.");
                        }
                    }while(resultadoBusqueda!=-1);

                    bingo->cartonUser[n].casilla[i][j]= nuevoNumero;
                    //bingo->espejoUser[n].casilla[i][j]= nuevoNumero;
                }
            }
     /* Asigna un identificador (id) a cada carton */
            strcpy(bingo->cartonUser[n].id,datos->nombre);
            bingo->cartonUser[n].num=n+1;


            ordenamiento (&bingo->cartonUser[n]);
            //ordenamiento (&bingo->espejoUser[n]);

        }
    }
    for(n=0;n < (bingo->cantidad);n++){
        printf("\n\n|>      %s - Carton  Numero %d     <", bingo->cartonPc[n].id,bingo->cartonPc[n].num);
        mostrarCarton(&bingo->cartonPc[n]);
        printf("\n\n|>    %s - Carton Numero %d   <",bingo->cartonUser[n].id,bingo->cartonUser[n].num);
        mostrarCarton(&bingo->cartonUser[n]);
    }
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
    int i=0, vec[x], r, c, aux, j;

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
        printf("|\n------------------------------------\n");
        for(j=0;j<COLUMNA;j++){
            printf("|  %.2d  ",carton->casilla[i][j]);
        }
    }
    printf("|\n------------------------------------\n");


}


void llenarConCeros(Bingo *bingo){

    int n, i, j;

    for(n=0;n < (bingo->cantidad);n++){

        for (i = 0; i<RENGLON; i++){

            for (j = 0; j<COLUMNA; j++ ){
                bingo->cartonUser[n].casilla[i][j]=0;
                bingo->cartonPc[n].casilla[i][j]=0;
                //bingo->espejoPc[n].casilla[i][j]=0;
                //bingo->espejoUser[n].casilla[i][j]=0;
            }
        }
        //bingo->espejoPc[n].num=n+1; // le asigno un nuemero identificador al carton espejo
        //bingo->espejoUser[n].num=n+1;

        bingo->cartonPc->lin=0;
        bingo->cartonUser->col=0;
    }
}


void desarrollo(Bingo *bingo, Datos *datos){

    int nBingo=0, nlinea=0, ncolunma=0, i=0, j, n, bola=0, bolillas[90],resultadoBusqueda;


    for(n=0;n<90;n++){
            bolillas[n]=0;
    }

    datos->puntosPc=0;   // iniciamos el puntaje a 0.
    datos->puntosUser=0;

    printf("\n   Comencemos a sacar bolillas...\n");
    system("pause");

    while(nBingo!=1){

        do{
            resultadoBusqueda=-1;
            bola = 1 +rand()%90;
            for(n=0;n<i;n++){
                if(bolillas[n]==bola){
                    resultadoBusqueda=n;
                }
            }

        }while(resultadoBusqueda!=-1);


        printf("\n>>> Ha salido la bola numero: %d \n", bola);
        bolillas[i]=bola;
        i++;

        marcarBola(bingo->cartonUser,bola,bingo->cantidad);
        marcarBola(bingo->cartonPc,bola,bingo->cantidad);

        verificarLinea(bingo->cartonUser,&datos->puntosUser,&nlinea,bingo->cantidad);
        verificarLinea(bingo->cartonPc,&datos->puntosPc,&nlinea,bingo->cantidad);

        verificarColumna(bingo->cartonUser,&datos->puntosUser,&ncolunma,bingo->cantidad);
        verificarColumna(bingo->cartonPc,&datos->puntosPc,&ncolunma,bingo->cantidad);

        verificarBingo(bingo->cartonUser,&datos->puntosUser,&nBingo, bingo->cantidad);
        verificarBingo(bingo->cartonPc,&datos->puntosPc,&nBingo,bingo->cantidad);


        for(n=0;n < (bingo->cantidad);n++){
            printf("\n\n|>      %s - Carton  Numero %d     <",bingo->cartonPc[n].id,bingo->cartonPc[n].num);
            mostrarCarton(&bingo->cartonPc[n]);
            printf("\n\n|>    %s - Carton Numero %d   <",bingo->cartonUser[n].id,bingo->cartonUser[n].num);
            mostrarCarton(&bingo->cartonUser[n]);
            printf("Cantidad de Lineas %d\n",nlinea);
            printf("Cantidad de Columnas %d \n",ncolunma);
            printf("Cantidad de Bingo %d\n\n",nBingo);

        }

        if(bingo->modoJuego==2){
            system("pause");
        }
    }

    multiplicarPuntos(i,&datos->puntosUser);
    multiplicarPuntos(i,&datos->puntosPc);

    printf("\n******   PUNTAJES   *******\n\n");
    printf("Total de puntos de %s %s --> %d\n",datos->nombre,datos->apellido,datos->puntosUser);
    printf("Total de puntos de PC --> %d\n",datos->puntosPc);

    printf("\n\n Salieron %d bolillas, estas fueron:", i);
    for(j=0;j<i;j++){
        printf("  [%d]",bolillas[j]);
    }
    printf(".-");

}

void multiplicarPuntos(int bolillas, int *puntos){

    if(bolillas<30){

        *puntos*=2;
    }else{

        if(bolillas<50){

            *puntos*=1.7;
        }else{

            if(bolillas<70){

                *puntos*=1.5;
            }
        }
    }


}

void verificarBingo(Carton *carton,int *puntos, int *bin, int c){

    int suma=0, n;

    for(n=0;n<c;n++){

        for ( int i =0; i<RENGLON; i++){

            for ( int j= 0; j<COLUMNA; j++ ){

                //SUMA = SUMA +NUMERO
                suma = suma + carton[n].casilla[i][j];
            }
        }
        if(suma==0){

            printf("\n\n========    G A M E    ========\n========    O V E R    ========\n\n");
            printf("------>  %s ha completado el BINGO con en carton numero %d",carton[n].id,carton[n].num);
            *puntos+=70;
            *bin=1;
        }
    }

}

void verificarLinea (Carton *carton, int *puntos, int *linea,int c){

    int v, n, i, j;

    for(n=0;n<c;n++){

        for ( i =0; i<RENGLON; i++){
            //iniciar vector en cero
            v = 0;

            for ( j= 0; j<COLUMNA; j++ ){

                //SUMA = SUMA +NUMERO
                v += carton[n].casilla[i][j];
            }

            if(v==0 && carton[n].lin<3){

                *linea=*linea+1;
                carton[n].lin++;

                if(*linea==1){
                    *puntos+=20;
                    printf("--> %s ha sacado linea con en carton %d, por ser la primer linea gana 20 Puntos!\n",carton[n].id,carton[n].num);
                }

            }
        }
    }

}

void verificarColumna (Carton *carton, int *puntos, int *col, int c){

    int v, n, i, j;

    for(n=0;n<c;n++){

        for ( i =0; i<COLUMNA; i++){

            v=0;
            for ( j= 0; j<RENGLON; j++ ){

                //SUMA = SUMA +NUMERO
                v += carton[n].casilla[j][i];
            }
            if(v==0 && carton[n].col<5){

                *col=*col+1;
                carton[n].col++;

                if(*col==1){
                    *puntos+=10;
                    printf("--> %s ha sacado columna con en carton %d, por ser la primer columna gana 10 Puntos!\n",carton[n].id,carton[n].num);
                }

            }

        }
    }
}

void marcarBola (Carton *carton, int num, int c){

    int i, j, n;

    for(n=0;n<c;n++){

        for(i=0;i<RENGLON;i++){

            for (j = 0; j<COLUMNA; j++){

                if ( carton->casilla[i][j]==num){ //Lo encontre

                    carton->casilla[i][j] = 0;

                    printf("\n %s ha marcado el numero %d en el carton %d \n", carton[n].id,num,carton[n].num);
                }
            }
        }
    }
}
