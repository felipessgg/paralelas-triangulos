#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> // atof
#include <math.h>   //operaciones aritmeticas
#include <iomanip>  //para dar precision al float
#include <stdio.h>
#include <sstream>

#define GetSize(array_enteros) (sizeof(array_enteros)/sizeof(*(array_enteros)))

using namespace std;

/*
::::ACLARACIONES IMPORTANTES:::
BORRAR PRIMER DATO "9665 3" DEL INICIO DEL ARCHIVO ESPIRAL.MESH

*/
string nod[9665][3] = {};    //ARCHIVO NODOS
string coord[5000][3] = {};  //ARCHIVO COORDENADAS
string resulMesh[19330][3] = {}; //ARCHIVO RESULTADOS.MESH
string resulNode[15000][3] = {}; //ARCHIVO RESULTADOS.NODE
float lstCandidatos[9665][14] = {};
float ptoMedio[2] = {};
float x;

void enLista(string lista[][3], const char *nombreArchivo){
    ifstream archivo(nombreArchivo);
    string linea;

    if(archivo.is_open()){
        int cont=0;
        while(getline(archivo,linea)){
            //Separa cada termino
            for(size_t p=0, q=0, j=0; p!=linea.npos; p=q, j++){
                string num = linea.substr(p+(p!=0),(q=linea.find(' ', p+1))-p-(p!=0));
                lista[cont][j] = num;
            }
            cont++;
        }
    }
    archivo.close();
}

//busca las cordenadas de un nodo en especifico, si n=1 busca X y  si n=2 busca Y.
string buscaCoordenada(string nodo, int n){
    for(int i=0;i<5000;i++){
        if(nodo==coord[i][0]){
            return coord[i][n];
        }
    }
}

float distancia(string a1,string b1, string a2,string b2){
    float d=0, x1=0, y1=0, x2=0, y2=0;
    x1 = (float)atof(a1.c_str());
    x2 = (float)atof(a2.c_str());
    y1 = (float)atof(b1.c_str());
    y2 = (float)atof(b2.c_str());
    d = pow(x2-x1,2)+pow(y2-y1,2);
    d = sqrt(d);
    return d;
}

float calculaAngulo(float d1, float d2, float d3){
    float cose=0,angul=0,arco=0;
    cose= (pow(d1,2) + pow(d2,2) - pow(d3,2))/(2*d1*d2);
    arco=acos(cose);
    angul= arco*180/3.1415926;
    return angul;
}

//retorna 1 si se debe refinar o 0 si no se debe refinar
bool verificaCondicion(float angulo1, float angulo2, float angulo3){
    if ((angulo1<=x) || (angulo2<=x) || (angulo3<=x)){
        return true;
    }else{
        return false;
    }
}

// distancia mas larga
string masLarga(float d1, float d2, float d3){
    string distancia = "";
    if( d1 > d2 and d1 > d3){
        distancia = "d1";
    }
    if( d2 > d1 and d2 > d3){
    	distancia = "d2";
    }
    if( d3 > d1 and d3 > d2){
    	distancia = "d3";
    }
    return distancia;
}

void candidatos(){
    string x1,y1,x2,y2,x3,y3;

    for(int i=0;i<9665;i++){
        //coordenadas de cada nodo
        //nodo 1
        x1 = buscaCoordenada(nod[i][0],1);
        y1 = buscaCoordenada(nod[i][0],2);
        //nodo 2
        x2 = buscaCoordenada(nod[i][1],1);
        y2 = buscaCoordenada(nod[i][1],2);
        //nodo 3
        x3 = buscaCoordenada(nod[i][2],1);
        y3 = buscaCoordenada(nod[i][2],2);

        //lados del triangulo
        float d1 = distancia(x1,y1,x2,y2); // corresponde al angulo alpha
        float d2 = distancia(x2,y2,x3,y3); // corresponde al angulo beta
        float d3 = distancia(x3,y3,x1,y1); // corresponde al angulo gamma

        //angulos del triangulo
        float alpha = calculaAngulo(d2,d3,d1);
        float beta  = calculaAngulo(d3,d1,d2);
        float gamma = calculaAngulo(d1,d2,d3);

        bool esCandidato = verificaCondicion(alpha,beta,gamma);

        lstCandidatos[i][0] = i;    //posicion en archivo espiral.mesh
        lstCandidatos[i][1] = esCandidato; //candidato o no a refinar
        lstCandidatos[i][2] = (float)atof(x1.c_str()); //vertice a
        lstCandidatos[i][3] = (float)atof(y1.c_str()); //vertice a
        lstCandidatos[i][4] = (float)atof(x2.c_str()); //vertice b
        lstCandidatos[i][5] = (float)atof(y2.c_str()); //vertice b
        lstCandidatos[i][6] = (float)atof(x3.c_str()); //vertice c
        lstCandidatos[i][7] = (float)atof(y3.c_str()); //vertice c
        lstCandidatos[i][8] = d1; // distancia 1
        lstCandidatos[i][9] = d2; // distancia 2
        lstCandidatos[i][10] = d3; // distancia 3
        lstCandidatos[i][11] = alpha; // angulo alfa
        lstCandidatos[i][12] = beta; // angulo beta
        lstCandidatos[i][13] = gamma; // angulo gamma
    }
}

void puntoMedio(float x1, float y1, float x2, float y2){
    ptoMedio[0] = (x1 + x2)/2 ;
    ptoMedio[1] = (y1 + y2)/2 ;
}

string coordenadasPtoMedio(int posicion){
    string mayor = masLarga(lstCandidatos[posicion][8],lstCandidatos[posicion][9],lstCandidatos[posicion][10]);
    if (mayor == "d1"){
        puntoMedio(lstCandidatos[posicion][2],lstCandidatos[posicion][3],lstCandidatos[posicion][4],lstCandidatos[posicion][5]);
    }
    if (mayor == "d2"){
        puntoMedio(lstCandidatos[posicion][4],lstCandidatos[posicion][5],lstCandidatos[posicion][6],lstCandidatos[posicion][7]);
    }
    if (mayor == "d3"){
        puntoMedio(lstCandidatos[posicion][6],lstCandidatos[posicion][7],lstCandidatos[posicion][2],lstCandidatos[posicion][3]);
    }
    return mayor;
}

int cuentaLineas(const char *nombreArchivo){
    ifstream archivo(nombreArchivo);
    string linea;
    int cont=0;
    if(archivo.is_open()){
        while(getline(archivo,linea)){
            cont++;
        }
    }
    archivo.close();
    return cont;
}

void agregarLinea(const char *nombreArchivo, string linea){
    fstream fileAdd;
    fileAdd.open( nombreArchivo, ios::app);
    fileAdd << linea << endl;
    fileAdd.close();
}

void creaArchivo(){

    FILE *y = fopen("resultados.mesh","w");

    //copia archivo espiral.node en resultados.node
    FILE *file1, *file2;
    int data1 = 0;

    file1 = fopen ( "espiral.node", "r" );
    file2 = fopen ( "resultados.node" , "w" );

    while((data1=fgetc(file1))!=EOF){
        fputc(data1,file2);
    }

    fclose(file1);
    fclose(file2);
    agregarLinea("resultados.node", "");

}

string buscaCoordenada2(string nodo, string lista[][3], int n){
    int largo = cuentaLineas("resultados.node");
    for(int i=0;i<largo;i++){
        if(nodo==lista[i][0]){
            return lista[i][n];
        }
    }
}

int main()
{
    creaArchivo();
    std::cout << std::fixed;
    std::cout << std::setprecision(6);
    cout<<"RECUERDA!!"<<endl;
    cout<<"BORRAR PRIMER DATO '9665 3' DEL INICIO DEL ARCHIVO ESPIRAL.MESH\n"<<endl;
    cout<<"Ingrese angulo de refinamiento: ";
    cin>>x;
    enLista(nod,"espiral.mesh");
    enLista(coord,"espiral.node");

    //enlista los candidatos a refinar
    candidatos();

    for(int i=0;i<9665;i++){
        if (lstCandidatos[i][1]==1){
            //cout<<"#####INICIOO######"<<endl;
            string mayor = coordenadasPtoMedio(i); // ve que lado se partió
            int numNodo = cuentaLineas("resultados.node") + 1; //cuenta cuantas coordenadas hay y suma 1 para guardar la proxima

            //pasando numero numNodo a string
            std::stringstream numToString;
            numToString << numNodo;

            //cout<<"SI ES CANDIDATO CON "<<mayor<<" PARA FILA "<< i <<endl;

            if (mayor == "d1"){
                //se intercambian nodos 1 y 2
                string triangulo1 = nod[i][0]+" "+numToString.str()+" "+nod[i][2];
                string triangulo2 = numToString.str()+" "+nod[i][1]+" "+nod[i][2];

                agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);
            }
            if (mayor == "d2"){
                //se intercambian nodos 2 y 3
                string triangulo1 = nod[i][0]+" "+numToString.str()+" "+nod[i][2];
                string triangulo2 = nod[i][0]+" "+nod[i][1]+" "+numToString.str();

                agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);
            }
            if (mayor == "d3"){
                //se intercambian nodos 3 y 1
                string triangulo1 = numToString.str()+" "+nod[i][1]+" "+nod[i][2];
                string triangulo2 = nod[i][0]+" "+nod[i][1]+" "+numToString.str();

                agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);
            }
            /*
            cout<<nod[i][0]<<endl;
            cout<<nod[i][1]<<endl;
            cout<<nod[i][2]<<endl;
            cout<< numToString.str() <<endl;

            cout << ptoMedio[0] <<endl;
            cout << ptoMedio[1] <<endl;
            */
            //pasando numero numNodo a string
            std::stringstream ptoMedio0ToString;
            char bufferX[15];
            sprintf(bufferX, "%.6f", ptoMedio[0]); //numero formateado a 6 decimales
            ptoMedio0ToString << bufferX;

            //pasando numero numNodo a string
            std::stringstream ptoMedio1ToString;
            char bufferY[15];
            sprintf(bufferY, "%.6f", ptoMedio[1]); //numero formateado a 6 decimales
            ptoMedio1ToString << bufferY;

            //se agrega al nuevo archivo
            string linea = numToString.str()+" "+ptoMedio0ToString.str()+" "+ptoMedio1ToString.str();
            agregarLinea("resultados.node",linea);
            cout<<"FILA "<< i+1 << " REFINADA" <<endl;

        }else if(lstCandidatos[i][1]==0){
            cout<<"FILA "<< i+1 << " NO SE REFINA" <<endl;
            /*
            cout<<"NO ES CANDIDATO PARA FILA "<< i <<endl; //FILA DEL ARCHIVO ESPIRAL.MESH (FILA+1)
            cout<<nod[i][0]<<endl;
            cout<<nod[i][1]<<endl;
            cout<<nod[i][2]<<endl;
            */
            string triangulo = nod[i][0]+" "+nod[i][1]+" "+nod[i][2];
            agregarLinea("resultados.mesh",triangulo);
        }
    }

    cout<<"COMIENZO ANALISIS DE CONFORMIDADES"<<endl;
    enLista(resulMesh,"resultados.mesh");
    enLista(resulNode,"resultados.node");

    int largoResulMesh = cuentaLineas("resultados.mesh");

    string x1,y1,x2,y2,x3,y3;
    float nx1,ny1,nx2,ny2,nx3,ny3;

    system("pause");

    FILE *z = fopen("resultados.mesh","w"); //borrando contenido del archivo

    for(int k=0;k<largoResulMesh;k++){
        //nodo 1
        x1 = buscaCoordenada2(resulMesh[k][0],resulNode,1);
        nx1 = (float)atof(x1.c_str());
        y1 = buscaCoordenada2(resulMesh[k][0],resulNode,2);
        ny1 = (float)atof(y1.c_str());
        //nodo 2
        x2 = buscaCoordenada2(resulMesh[k][1],resulNode,1);
        nx2 = (float)atof(x2.c_str());
        y2 = buscaCoordenada2(resulMesh[k][1],resulNode,2);
        ny2 = (float)atof(y2.c_str());
        //nodo 3
        x3 = buscaCoordenada2(resulMesh[k][2],resulNode,1);
        nx3 = (float)atof(x3.c_str());
        y3 = buscaCoordenada2(resulMesh[k][2],resulNode,2);
        ny3 = (float)atof(y3.c_str());

        //ANALIZANDO PRIMER PUNTO MEDIO CON COORDENADAS DEL RESULTADOS.NODE
        puntoMedio(nx1,ny1,nx2,ny2); //primer punto medio del triangulo
        int largo1 = GetSize(resulNode);
        string iguales1 = "NADA1";

        for (int x=0;x<largo1;x++){
            float coord1 = (float)atof(resulNode[x][1].c_str());
            float coord2 = (float)atof(resulNode[x][2].c_str());

            if(ptoMedio[0]==coord1){
                if(ptoMedio[1]==coord2){
                    iguales1 = "IGUALES1";
                    cout<<iguales1<<" - FILA "<<k<<endl; //filas de archivo resultados.mesh
                    cout<<"("<<resulNode[x][1]<<" , "<<resulNode[x][2]<<")"<<endl;

                    //agregando nuevos triangulos
                    string triangulo1 = resulMesh[k][0]+" "+resulNode[x][0]+" "+resulMesh[k][2];
                    string triangulo2 = resulNode[x][0]+" "+resulMesh[k][1]+" "+resulMesh[k][2];

                    agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);

                }
            }
        }

        //ANALIZANDO SEGUNDO PUNTO MEDIO CON COORDENADAS DEL RESULTADOS.NODE
        puntoMedio(nx2,ny2,nx3,ny3); //segundo punto medio del triangulo
        string iguales2 = "NADA2";
        int largo2 = GetSize(resulNode);

        for (int x=0;x<largo2;x++){
            float coord1 = (float)atof(resulNode[x][1].c_str());
            float coord2 = (float)atof(resulNode[x][2].c_str());

            if(ptoMedio[0]==coord1){
                if(ptoMedio[1]==coord2){
                    iguales2 = "IGUALES2";
                    cout<<iguales2<<" - FILA "<<k<<endl; //filas de archivo resultados.mesh
                    cout<<"("<<resulNode[x][1]<<" , "<<resulNode[x][2]<<")"<<endl;

                    //agregando nuevos triangulos
                    string triangulo1 = resulMesh[k][0]+" "+resulNode[x][0]+" "+resulMesh[k][2];
                    string triangulo2 = resulMesh[k][0]+" "+resulMesh[k][1]+" "+resulNode[x][0];

                    agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);


                }
            }
        }

        //ANALIZANDO TERCER PUNTO MEDIO CON COORDENADAS DEL RESULTADOS.NODE
        puntoMedio(nx3,ny3,nx1,ny1); //tercer punto medio del triangulo
        int largo3 = GetSize(resulNode);
        string iguales3 = "NADA3";

        for (int x=0;x<largo3;x++){
            float coord1 = (float)atof(resulNode[x][1].c_str());
            float coord2 = (float)atof(resulNode[x][2].c_str());

            if(ptoMedio[0]==coord1){
                if(ptoMedio[1]==coord2){
                    iguales3 = "IGUALES3";
                    cout<<iguales3<<" - FILA "<<k<<endl; //filas de archivo resultados.mesh
                    cout<<"("<<resulNode[x][1]<<" , "<<resulNode[x][2]<<")"<<endl;

                    //agregando nuevos triangulos
                    string triangulo1 = resulMesh[k][0]+" "+resulMesh[k][1]+" "+resulNode[x][0];
                    string triangulo2 = resulNode[x][0]+" "+resulMesh[k][1]+" "+resulMesh[k][2];

                    agregarLinea("resultados.mesh",triangulo1+"\n"+triangulo2);

                }
            }
        }

        //SI NINGUNO DE LOS PUNTOS MEDIOS ES IGUAL A ALGUN NODO, GUARDA LOS 3 NODOS ORIGINALES DEL ARCHIVO
        if ((iguales1 == "NADA1")&&(iguales2 == "NADA2")&&(iguales3 == "NADA3")){
            //agregando triangulo sin modificaciones
            string triangulo = resulMesh[k][0]+" "+resulMesh[k][1]+" "+resulMesh[k][2];
            agregarLinea("resultados.mesh",triangulo);
        }

    }

    cout<<endl;
    cout<<"FIN DE LA EJECUCION"<<endl;
    return 0;
}
