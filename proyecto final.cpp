#include<bits/stdc++.h>
#include<windows.h>
#include<cstdlib>  // Necesario para system()
#include<conio.h>  // Para usar getch()
#include<ctime>
#include<iomanip>
#include<sstream>

using namespace std;

const int TAM=50;
const int espacio = 4;
const int MAX_PUNTAJES = 100;

// ---------------------- ESTRUCTURAS ----------------------

typedef struct {
    string titulo;
    int filas;
    int columnas;
    int numPalabras=0;
    int dificultad;
    char sopa[TAM][TAM];
} Juego;

typedef struct {
    int ci;
    string nombre;
    string fechaHora;
    int puntaje;
} Usuario;


// ---------------------- UTILIDADES ----------------------
void gotoxy(int x, int y){
    COORD coord; coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
		
void setColor(WORD color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void continuar(){
    cout << "Presiona una tecla para seguir...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string obtenerFechaHora() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream ossFecha, ossHora;
    ossFecha << put_time(&tm, "%Y-%m-%d");
    ossHora << put_time(&tm, "%H:%M:%S");

    return ossFecha.str() + "//" + ossHora.str();
}


// ---------------------- ENTRADA Y DATOS ----------------------
void ingresoJuego(Juego &juego){
    cout << "Título:"; cin >> juego.titulo;
    cout << "Cantidad Columnas: "; cin >> juego.columnas;
    cout << "Cantidad Filas: "; cin >> juego.filas;
    cout << "Cantidad de Palabras: "; cin >> juego.numPalabras;
    cout << "Dificultad (1-5): "; cin >> juego.dificultad;
}

void imprimirIngresoJuego(Juego juego){
    cout << "Título:" << juego.titulo << endl;
    cout << "Cantidad Columnas: " << juego.columnas << endl;
    cout << "Cantidad Filas: " << juego.filas << endl;
    cout << "Cantidad de Palabras: " << juego.numPalabras << endl;
    cout << "Dificultad (1-5): " << juego.dificultad << endl;
}


// ---------------------- INTERFAZ VISUAL ----------------------
void letrero1(){
    int x = 0, y = 0;
    gotoxy(x, y);     cout << "*************************************************";
    gotoxy(x, y + 1); cout << "*    Presione # para guardar juego              *";
    gotoxy(x, y + 2); cout << "*    Presione * para completar espacios al azar *";
    gotoxy(x, y + 3); cout << "*************************************************";
    gotoxy(0, y + 5);
}

void dibujarSopa(Juego juego){
    for(int r = 0; r < juego.filas; ++r){
        for(int c = 0; c < juego.columnas; ++c){
            gotoxy(c * 2, r + 8 + espacio); 
            cout << juego.sopa[r][c];
        }
    }
}


// ---------------------- DECLARACIÓN DE FUNCIONES ----------------------
void crearTabla(int pares[], int listaEstados[], const vector<string> &palabras);

void crearTabla2(int pares[], int listaEstados[], vector<string> &palabras);

void crearSopa(Juego &juego, vector<string> palabras, int pares[TAM], char direccion[TAM],
               int listaEstados[TAM]);
               
void pedirPalabras(Juego &juego, int &fila, int &col, char direcciones[TAM], vector<string> &palabras,
                   int pares[TAM], int listaEstados[TAM]);
                   
void Colocar_Sopa_Texto(Juego juego, int Num_Palabras,
                        vector<string> palabras, int pares[TAM], string Nombre_Archivo, char direcciones[TAM]);
                        
void completar_y_Guardar(Juego &juego, int pares[TAM], int listaEstados[TAM], vector<string> &palabras,
                         int &filap, int &colp, char direcciones[TAM]);
                         
void cargar_Juego(string elegido);

void guardarUsuario();

void verPuntajes();

void vaciar_todo(Juego &juego, int &filap, int &colp,
                 char direcciones[TAM], int pares[TAM], int listaEstados[TAM], vector<string> &palabras);
                 
void menu();


// ---------------------- DEFINICIONES COMPLETAS DE FUNCIONES ----------------------

void crearTabla(int pares[], int listaEstados[], const vector<string> &palabras){
    system("cls");
    cout << "Palabra\tFila\tCol\tEstado" << endl;
    cout << "--------------------------------" << endl;
    for(int i = 0; i < palabras.size(); ++i){
        int fila = pares[2*i];
        int col  = pares[2*i + 1];
        
        string estado;
        if(listaEstados[i] == 1){
            estado = "OK";
        }else{
            estado = "MAL";
        }
        cout << palabras[i] << '\t'
             << fila           << '\t'
             << col            << '\t'
             << estado         << endl;
    }
    cout << "--------------------------------" << endl;
    continuar();
}

void crearTabla2(int pares[], int listaEstados[], vector<string> &palabras){
	
	int x = 2, y = 5;
    gotoxy(x, y);
    
    cout << "Palabra\tFila\t\tCol\t\tEstado" << endl;
    cout << "-------------------------------------------------" << endl;
    for(int i = 0; i <palabras.size(); ++i){
        int fila = pares[2*i];
        int col  = pares[2*i + 1];
        
        string estado;
        if(listaEstados[i] == 1){
            estado = "OK";
        }else{
            estado = "MAL";
        }
        cout << palabras[i] << '\t' << '\t'
             << fila           << '\t' << '\t'
             << col            << '\t' << '\t'
             << estado         << endl;
    }
    cout << "------------------------------------------------" << endl;
}

void crearSopa(Juego &juego, vector<string> palabras, int pares[TAM], char direccion[TAM],
               int listaEstados[TAM]){
    system("cls");
    int numPalabras = palabras.size();
    int dueño[50][50];

    // Inicializar matrices
    for(int r = 0; r < juego.filas; r++){
        for(int c = 0; c < juego.columnas; c++){
            juego.sopa[r][c] = '.';
            dueño[r][c] = -1;
        }
    }

    // Procesar cada palabra
    for(int i = 0; i < numPalabras; i++){
        int fila = pares[2*i] - 1;
        int col  = pares[2*i+1] - 1;
        int dr = 0, dc = 0;

        if(direccion[i] == 'h'){
			dc = 1;
		}else if(direccion[i] == 'v'){
			dr = 1;
		}

        bool posicionAceptada = false;
        int longitud = palabras[i].length();
        char resp;

        while(!posicionAceptada){
            // 1) Dibuja la sopa actual
            for(int r = 0; r < juego.filas; r++){
                for(int c = 0; c < juego.columnas; c++){
                    gotoxy(c * 2, r);
                    cout << juego.sopa[r][c];
                }
            }

            // 2) Comprueba conflicto provisional
            bool conflictProv = false;
            for(int k = 0; k < longitud; k++){
                int rr = fila + dr * k;
                int cc = col  + dc * k;
                if(rr < 0 || rr >= juego.filas || cc < 0 || cc >= juego.columnas ||
                    (juego.sopa[rr][cc] != '.' && juego.sopa[rr][cc] != palabras[i][k])){
                    conflictProv = true;
                    break;
                }
            }

            // 3) Dibuja la palabra provisional (verde si no hay conflicto, rojo si lo hay)
            setColor(conflictProv ? (FOREGROUND_RED | FOREGROUND_INTENSITY) : 0x000A);
            for(int k = 0; k < longitud; k++){
                int rr = fila + dr * k;
                int cc = col + dc * k;
                if(rr >= 0 && rr < juego.filas && cc >= 0 && cc < juego.columnas){
                    gotoxy(cc * 2, rr);
                    cout << palabras[i][k];
                }
            }
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // 4) Preguntar si se acepta la posición
            gotoxy(0, juego.filas + 2);
            cout << "¿Aceptar '" << palabras[i] << "' en (" << fila + 1 << "," << col + 1 << ")? (s/n): ";
            cin >> resp;

            if(resp == 's'){
                posicionAceptada = true;
            }else{
                gotoxy(0, juego.filas + 3);
                cout << "Nueva fila inicio: ";
                cin >> pares[2 * i];
                cout << "Nueva columna inicio: ";
                cin >> pares[2 * i + 1];
                fila = pares[2 * i] - 1;
                col  = pares[2 * i + 1] - 1;
            }
        }

        // Verificar conflicto definitivo y asignar estado
        bool conflict = false;
        for(int k =0; k <longitud; k++){
            int rr = fila + dr * k;
            int cc = col  + dc * k;
            if(rr < 0 || rr >= juego.filas || cc < 0 || cc >= juego.columnas ||
                (juego.sopa[rr][cc] != '.' && juego.sopa[rr][cc] != palabras[i][k])){
                conflict = true;
            }
        }
        listaEstados[i] = conflict ? 0 : 1;

        // Colocar letras definitivas sin sobrescribir palabras previas
        for(int k = 0; k < longitud; k++){
            int rr = fila + dr * k;
            int cc = col  + dc * k;
            if(rr >= 0 && rr < juego.filas && cc >= 0 && cc < juego.columnas && dueño[rr][cc] == -1){
                juego.sopa[rr][cc] = palabras[i][k];
                dueño[rr][cc] = i;
            }
        }
    }

    // Dibujo final de la sopa
    for(int r =0; r < juego.filas; r++){
        for(int c = 0; c < juego.columnas; c++){
            gotoxy(c* 2, r);
            cout << juego.sopa[r][c];
        }
    }

    gotoxy(0, juego.filas + numPalabras + 4);
    system("pause");
}

void pedirPalabras(Juego &juego, int &fila, int &col, char direcciones[TAM],
                   vector<string> &palabras, int pares[TAM], int listaEstados[TAM]){
    
    int i = 0;
    int cont = 0;
    int cont2 = 0;
    int numPalabras = juego.numPalabras;

    while(numPalabras--){
        cout << "Palabra " << i++ << ": ";
        string ayuda;
        cin >> ayuda;
        palabras.push_back(ayuda);

        cout << "Fila: ";
        cin >> pares[cont++];
        cout << "Columna: ";
        cin >> pares[cont++];
        cout << "Dirección(h-v): ";
        cin >> direcciones[cont2++];

        // Actualiza la sopa y muestra la tabla
        crearSopa(juego, palabras, pares, direcciones, listaEstados);
        crearTabla(pares, listaEstados, palabras);
    }
}

void Colocar_Sopa_Texto(Juego juego, vector<string> palabras, int pares[TAM],
                        string Nombre_Archivo, char direcciones[TAM]){

    fstream guardar;
    guardar.open(Nombre_Archivo, ios::out);

    guardar << juego.dificultad << endl;
    guardar << juego.numPalabras << endl;
    guardar << juego.filas << endl;
    guardar << juego.columnas << endl;

    // Escribir palabras con su información
    for(int i =0; i < palabras.size(); ++i){
        guardar << palabras[i] << " "
                << pares[2 * i] << " "
                << pares[2 * i + 1] << " "
                << direcciones[i] << endl;
    }

    // Escribir la sopa
    for(int r =0; r < juego.filas;++r){
        for(int c = 0; c < juego.columnas; ++c){
            guardar << juego.sopa[r][c];
        }
        guardar << endl;
    }

    guardar.close();
}


void completar_y_Guardar(Juego &juego, int pares[TAM], int listaEstados[TAM], vector<string> &palabras,
                         int &filap, int &colp, char direcciones[TAM]){
							 
	system("cls");
    letrero1();
    crearTabla2(pares, listaEstados, palabras);
    dibujarSopa(juego);
    // continuar();
    
    while(true){
		
		char opcion = getch();
		
		if(opcion == '*'){
			system("cls");
			
			for(int r =0; r < juego.filas; ++r){
				for(int c = 0; c < juego.columnas; ++c){
					if(juego.sopa[r][c] == '.'){
						juego.sopa[r][c] = 'A' + rand() % 26;
					}
				}
			letrero1();
			crearTabla2(pares, listaEstados, palabras);
			dibujarSopa(juego);
			
			}
	
		}else if(opcion=='#'){
			fstream arch;
			string dificultad_texto;
			string dificultad_num = to_string(juego.dificultad);
			
			int CantJuegos;
			int CantJuegosParaNombreTexto = 0;
			arch.open("CantJuegos.txt", ios::in);
			
			ofstream archTemp("TempCantJuegos.txt");
			
			while(arch >> dificultad_texto >> CantJuegos){
				
				if(juego.dificultad == 1 && dificultad_texto == "uno"){
					CantJuegos++;
					CantJuegosParaNombreTexto = CantJuegos;
					
				}
				if(juego.dificultad == 2 && dificultad_texto == "dos"){
					CantJuegos++;
					CantJuegosParaNombreTexto = CantJuegos;
					
				}
				if(juego.dificultad == 3 && dificultad_texto == "tres"){
					CantJuegos++;
					CantJuegosParaNombreTexto = CantJuegos;
					
				}
				if(juego.dificultad == 4 && dificultad_texto == "cuatro"){
					CantJuegos++;
					CantJuegosParaNombreTexto = CantJuegos;
					
				}
				if(juego.dificultad == 5 && dificultad_texto == "cinco"){
					CantJuegos++;
					CantJuegosParaNombreTexto = CantJuegos;
					
				}
							
				archTemp << dificultad_texto << " " << CantJuegos << endl;
			}
			
			arch.close();
			archTemp.close();
			
			remove("CantJuegos.txt");
			rename("TempCantJuegos.txt", "CantJuegos.txt");
			
			string agregar_num = to_string(CantJuegosParaNombreTexto);
			
			string Nombre_Archivo = juego.titulo+" "+dificultad_num+" _"+agregar_num+".txt";
			
			Colocar_Sopa_Texto(juego, palabras, pares, Nombre_Archivo, 
			direcciones);
			
		    fstream nombres_juegos;
			nombres_juegos.open("nombres_juegos.txt", ios::out|ios::app);
			
			nombres_juegos << Nombre_Archivo << endl;
			
			nombres_juegos.close();
			
		}else if(opcion==27){
			
			break;
			vaciar_todo(juego, filap, colp, 
			direcciones, pares, listaEstados, palabras);
			
			menu();
			
		}
    
	}
}


void cargar_Juego(string elegido){
	
    fstream archivo;
    archivo.open(elegido, ios::in);

    Juego juego;
    archivo >> juego.dificultad >> juego.numPalabras;
    archivo >> juego.filas >> juego.columnas;

    
    vector<string> palabras;
    palabras.reserve(juego.numPalabras);
    int pares[TAM];              // pares[2*i] = fila, pares[2*i+1] = columna
    char direcciones[TAM];       // direcciones[i] = 'h' o 'v'

    for(int i= 0; i < juego.numPalabras; ++i){
        string palabra;
        int fila, columna;
        char dir;
        archivo >> palabra >> fila >> columna >> dir;

        palabras.push_back(palabra);
        pares[2*i]   = fila;
        pares[2*i + 1] = columna;
        direcciones[i] = dir;
    }

    // Leer la sopa de letras
    for(int i = 0; i < juego.filas; ++i){
        for(int j = 0; j < juego.columnas;++j){
            archivo >> juego.sopa[i][j];
        }
    }

    archivo.close();

    vector<string> estados(juego.numPalabras, "");

    while(true){
        // Salida con tecla ESC
        if(_kbhit()){
            int tecla = getch();
            if (tecla == 27) return;
        }

        // Mostrar info
        cout << "Dificultad: " << juego.dificultad << endl;
        cout << "Número de palabras: " << juego.numPalabras << endl;
        for(int i = 0; i < juego.numPalabras; ++i){
			string estado = estados[i].empty() ? "-" : estados[i];
			
            cout << "Palabra: " << palabras[i]
                 << "  Fila: " << pares[2*i]
                 << "  Col: "  << pares[2*i + 1]
                 << "  Dir: "  << direcciones[i]
                 << "  Estado: " << estado << endl;;
        }

        // Mostrar sopa con resaltado
        cout << "\nSopa de letras con palabras encontradas en verde:\n";
        for(int r = 0; r < juego.filas; ++r){
            for(int c = 0; c < juego.columnas; ++c){
                bool esVerde = false;

                for(int i =0; i < juego.numPalabras; ++i){
                    if(estados[i] !="OK") continue;

                    int f = pares[2*i] - 1;
                    int col = pares[2*i + 1] - 1;
                    int dr = (direcciones[i] =='v') ? 1 : 0;
                    int dc = (direcciones[i]== 'h') ? 1 : 0;

                    int len = palabras[i].size();
                    for(int k = 0; k < len; ++k){
                        int rr = f + dr * k;
                        int cc = col + dc * k;
                        
                        if(rr == r && cc == c){
                            esVerde = true;
                            break;
                        }
                    }
                    if (esVerde) break;
                }

                setColor(esVerde ? 0x000A : (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
                cout << juego.sopa[r][c] << " ";
            }
            cout << endl;
        }
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


        cout << "\nIngresa el índice de la palabra a buscar (0 a " << juego.numPalabras - 1
             << "), o -1 para terminar: ";
        int idx;
        cin >> idx;

        if (idx == -1) break;
        if(idx < 0 || idx >= juego.numPalabras){
            cout << "Índice fuera de rango. Intenta de nuevo.\n";
            continue;
        }

        // Solicitar coordenadas y dirección
        cout << "Fila para \"" << palabras[idx] << "\": ";
        int filaUsu;
        cin >> filaUsu;

        cout << "Columna para \"" << palabras[idx] << "\": ";
        int colUsu;
        cin >> colUsu;

        cout << "Dirección (h/v): ";
        char dirUsu;
        cin >> dirUsu;

        // Validar respuesta
        if(filaUsu == pares[2*idx] &&
            colUsu == pares[2*idx + 1] &&
            dirUsu == direcciones[idx]){
            estados[idx] = "OK";
            cout << "¡Correcto! Palabra encontrada.\n";
        }else{
            estados[idx] = "MAL";
            cout << "Incorrecto. Estado marcado como MAL.\n";
        }
		
		        // Verificar si todas las palabras fueron encontradas
        bool todasEncontradas = true;
        for(auto estado : estados){
            if(estado != "OK"){
                todasEncontradas = false;
                break;
            }
        }

        if(todasEncontradas){
            break;
        }

		
        system("cls");
    }
    
    Sleep(200);
    system("cls");
    
     for(int r = 0; r < juego.filas; ++r){
        for(int c = 0; c < juego.columnas; ++c){
            bool esVerde = false;

            for(int i = 0; i < juego.numPalabras; ++i){
                if (estados[i] != "OK") continue;

                int f = pares[2*i] - 1;
                int col = pares[2*i + 1] - 1;
                int dr = (direcciones[i] == 'v') ? 1 : 0;
                int dc = (direcciones[i] == 'h') ? 1 : 0;

                int len = palabras[i].size();
                for(int k = 0; k < len; ++k){
                    int rr = f + dr * k;
                    int cc = col + dc * k;
                    if(rr == r && cc == c){
                        esVerde = true;
                        break;
                    }
                }
                if (esVerde) break;
            }

            setColor(esVerde ? 0x000A : (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
            cout << juego.sopa[r][c] << " ";
        }
        cout << endl;
    }
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    guardarUsuario();
}

void guardarUsuario(){
    Usuario usuario;
    cout << "Ingrese CI: "; cin >> usuario.ci;
    cout << "Ingrese usuario: "; cin >> usuario.nombre;
    usuario.puntaje = 100;
    usuario.fechaHora = obtenerFechaHora();

    fstream arch("puntajes.txt", ios::in);
    fstream archTemp("temp.txt", ios::out);

    if(!arch){
        archTemp << usuario.ci << " " << usuario.nombre << " " << usuario.fechaHora << " " << usuario.puntaje << endl;
        archTemp.close();
        rename("temp.txt", "puntajes.txt");
        cout << "Datos guardados correctamente." << endl;
        return;
    }

    Usuario usuarioArch;
    bool encontrado = false;

    while(arch >> usuarioArch.ci >> usuarioArch.nombre >> usuarioArch.fechaHora >> usuarioArch.puntaje){
        if(usuarioArch.ci == usuario.ci && usuarioArch.nombre == usuario.nombre){
            usuarioArch.puntaje += usuario.puntaje; 
            usuarioArch.fechaHora = usuario.fechaHora;
            encontrado = true;
        }
        archTemp << usuarioArch.ci << " " << usuarioArch.nombre << " " << usuarioArch.fechaHora << " " << usuarioArch.puntaje << endl;
    }
    
    
    if(!encontrado){
        archTemp << usuario.ci << " " << usuario.nombre << " " << usuario.fechaHora << " " << usuario.puntaje << endl;
    }

    arch.close();
    archTemp.close();

    
    remove("puntajes.txt");
    rename("temp.txt", "puntajes.txt");

    cout << "Datos guardados correctamente." << endl;
    
}

void verPuntajes(){
    Usuario usuarios[MAX_PUNTAJES];
    int total = 0;

    ifstream arch("puntajes.txt");
    if(!arch.is_open()){
        cout << "No se pudo abrir el archivo de puntajes.\n";
        return;
    }

    // Leer archivo
    while(total < MAX_PUNTAJES && arch >> usuarios[total].ci >> usuarios[total].nombre >> usuarios[total].fechaHora >> usuarios[total].puntaje){
        total++;
    }
    arch.close();

    int opcion = -1;

    while(opcion!= 0){
		
		cout << endl;
    cout << "╔══════════════════════════════════════════════╗" << endl;
    cout << "║     📊  M E N Ú   D E   P U N T A J E S      ║" << endl;
    cout << "╠══════════════════════════════════════════════╣" << endl;
    cout << "║  1. 🔢 Ver puntajes ordenados por valor      ║" << endl;
    cout << "║  2. 🗓️  Ver puntajes ordenados por fecha     ║" << endl;
    cout << "║  3. 👤 Ver puntajes por jugador/usuario      ║" << endl;
    cout << "║  0. 🔙 Volver al menú principal              ║" << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;
    cout << endl;
    cout << "👉 Ingrese una opción (0-3): ";
        cin >> opcion;

        if(opcion == 1){
            // Ordenar por puntaje descendente
            int i = 0;
            while(i <total - 1){
                int j = 0;
                while(j <total - i- 1){
                    if (usuarios[j].puntaje < usuarios[j + 1].puntaje) {
                        swap(usuarios[j], usuarios[j + 1]);
                    }
                    j++;
                }
                i++;
            }

            cout << "\n--- Puntajes por valor ---\n";
            i = 0;
            while(i < total){
                cout << usuarios[i].ci << " " << usuarios[i].nombre << " " << usuarios[i].fechaHora << " " << usuarios[i].puntaje << endl;
                i++;
            }

        } else if (opcion == 2) {
            // Ordenar por fecha ascendente
            int i = 0;
            while(i < total- 1){
                int j = 0;
                while(j <total - i- 1){
                    if (usuarios[j].fechaHora > usuarios[j + 1].fechaHora) {
                        swap(usuarios[j], usuarios[j + 1]);
                    }
                    j++;
                }
                i++;
            }

            cout << "\n--- Puntajes por fecha ---\n";
            i = 0;
            while(i< total){
                cout << usuarios[i].ci << " " << usuarios[i].nombre << " " << usuarios[i].fechaHora << " " << usuarios[i].puntaje << endl;
                i++;
            }

        }else if(opcion ==3){
            string buscar;
            cout << "Ingrese nombre de usuario: ";
            cin >> buscar;

            bool encontrado = false;
            int i = 0;
            cout << "\n--- Puntajes de " << buscar << " ---\n";
            while(i < total){
                if (usuarios[i].nombre == buscar) {
                    cout << usuarios[i].ci << " " << usuarios[i].nombre << " " << usuarios[i].fechaHora << " " << usuarios[i].puntaje << endl;
                    encontrado = true;
                }
                i++;
            }

            if(!encontrado){
                cout << "No se encontraron puntajes para ese usuario.\n";
            }
        }
    }
}


void vaciar_todo(Juego &juego, int &filap, int &colp,
                 char direcciones[TAM], int pares[TAM], int listaEstados[TAM], vector<string> &palabras){
	
	juego.filas = 0;
	juego.columnas = 0;
	juego.numPalabras = 0;
	juego.dificultad = 0;
	juego.titulo = "";
	filap = 0;
	colp = 0;
	for(int i=0; i<TAM; ++i){
		direcciones[i]= ' ';
		pares[i]=0;
		listaEstados[i]=0;
		if(i < palabras.size()){
			palabras[i]=" ";
		}
	}
	for(int i=0; i<TAM; ++i){
		for(int j=0; j<TAM; ++j){
			juego.sopa[i][j]=' ';
		}
	}
	palabras.clear();
}

void menu(){
	
    cout << "╔════════════════════════════════════════════════╗" << endl;
    cout << "║         🧠  S O P A   D E   L E T R A S   	 ║" << endl;
    cout << "║            Menú Principal          	  	 ║" << endl;
    cout << "╠════════════════════════════════════════════════╣" << endl;
    cout << "║  1. 🎮 Crear nuevo juego                 	 ║" << endl;
    cout << "║  2. ▶️  Jugar                            	 ║" << endl;
    cout << "║  3. 📊 Ver puntuaciones                   	 ║" << endl;
    cout << "║  4. 🎲 Jugar partida aleatoria             	 ║" << endl;
    cout << "╚════════════════════════════════════════════════╝" << endl;
    cout << endl;
    cout << "👉 Ingrese una opción (1-4): ";
    
    int opcion; cin >> opcion;
     
    
    if(opcion == 1){
	
		Juego juego;
		ingresoJuego(juego);
		
		int filap, colp;
		char direcciones[TAM]={};
		int pares[TAM]={ }; // fila y col de las palabras;
		int listaEstados[TAM]={ };
		vector<string> palabras;
		
		pedirPalabras(juego, filap, colp, direcciones, palabras, pares, listaEstados);
		
		completar_y_Guardar(juego, pares, listaEstados, palabras, filap, colp, direcciones);
		
		vaciar_todo(juego, filap, colp, direcciones, pares, listaEstados, palabras);
	
	}else if(opcion == 2){
		
		system("cls");
		
		cout << "Escoge el num de partida" << endl;
		
		fstream nombres_juegos;
		
		nombres_juegos.open("nombres_juegos.txt", ios::in);

		vector<string> lista;
		string nombre;
		int i=0;

		while(getline(nombres_juegos, nombre)){
			lista.push_back(nombre);
			cout << i << ": " << nombre << endl;
			i++;
		}

		nombres_juegos.close();

		int num; cin >> num;

		if(num >= 0 && num < lista.size()){
			string elegido = lista[num];

			cargar_Juego(elegido);
			
		}else{
			cout << "Número inválido." << endl;
		}
				
			
	}else if(opcion == 3){
		verPuntajes();
		
	}else if(opcion == 4){
		
		system("cls");
		
        fstream nombres_juegos;
        nombres_juegos.open("nombres_juegos.txt", ios::in);

        vector<string> lista;
        string nombre;

        while(getline(nombres_juegos, nombre)){
            lista.push_back(nombre);
        }

        nombres_juegos.close();

        if(lista.empty()){
            cout << "No hay juegos guardados aún." << endl;
        }else{
            srand(time(NULL));
            int aleatorio = rand()% lista.size();

            cout << "Cargando partida aleatoria: "<< lista[aleatorio] << endl;
            cargar_Juego(lista[aleatorio]);
        }
    }

}


int main(){
    system("chcp 65001");
    
    while(true){
	
		menu();
		
		int tecla = getch();
        if (tecla == 27) break; // 27 es el código ASCII de ESC
        system("cls");
	
	}   
    
	
}
