#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


int main()
{
	char PT[4];
	char CTS[5];
	char RTS[5] ;
	char ACK_IN[3] ;
	char ACK_OUT[3] ;
	int TRAMA[1]; // trama de alarma
	int ID_NODE = 10; // ID del nodo actual
	int ID_NODE_IN = 20; // ID del nodo escuchado
	int contador_p = 0; // contador del primer estado de espera
	int contador_s = 0; // contador del segundo estado de espera
	int contador_t = 0; // contador del tercer estado de espera
	int contador_c = 0; // contador del cuarto estado de espera

	enum state { primer_estado, segundo_estado, tercer_estado, cuarto_estado };
	enum state current_state = primer_estado;

	while (current_state != cuarto_estado)
	{
		switch (current_state)
		{
		case primer_estado:
		{
			if (PT != 0)
			{
				if (CTS != 0 or RTS != 0) // si CTS o RTS estan activos
				{
					if (contador_p == 3)
					{
						// Hibernación
						contador_p = 0;
					}
					else
					sleep(10); // tiempo de espera para trasmisión
					current_state = primer_estado;
					contador_p = contador_p + 1;
					cout << "Espera \n";
				}
				if (ID_NODE_IN < (ID_NODE-1)) // si el ID del PT escuchado es menor al ID del nodo reconfiguracion
				{
					ID_NODE = 30; // Cambio de nodo si se escucha uno menor

					cout << "cambio de nodo \n";
					cout << ID_NODE;
					cout << "\n";

				}
				else
				{
					if (ID_NODE_IN == ID_NODE - 1)
					{
						ID_NODE = 20; // Nodo actual
						current_state = segundo_estado;
						contador_p = 0;
					}

					RTS = 1;
					cout << " Nodo actual no cambio \n";
					cout << ID_NODE;
					cout << "\n";
				}
				 // cambio al siguiente estado
			}
			else
			{
				// regreso al estado de descubrimiento de red no borra memoria de alarmas
			}

			break;
		}

		case segundo_estado:

		{

			sleep(rand() % 20); // espera un tiempo aleatorio
			if (RTS != 0 or CTS !=0)
			{
				if (contador_s == 3)
				{
					// regreso al estado de hibernación
					cout << "estado de hibernación\n";
					//Hibernar();
					contador_s = 0;

					cout << contador_s;
					cout << "\n";
					sleep(200);
					RTS = 0;
					current_state = segundo_estado;
				}
				else
				{
					contador_s = contador_s + 1;
					current_state = primer_estado;

					cout << "contador\n";
					cout << contador_s;
					cout << "\n";
					sleep(2000);
				}
			}
			else
			{
				RTS = 17393; // envio de trama RTS
				contador_s = 0;
				current_state = tercer_estado;

				cout << "cambio de estado \n";
				CTS = 1;
			}
			break;
		}

		case tercer_estado:

		{
			if (CTS != 0)
			{
				current_state = cuarto_estado;
				contador_t = 0;
				cout << "cambio a estado 4\n";
			}
			else if (CTS == 0)
			{
				if (contador_t == 3)
				{
					// Pasa a modo Hibernación
					contador_t = 0;
				}
				else
				{
					contador_t = contador_t + 1;
					current_state = primer_estado;
				}
			}

			break;
		}

		case cuarto_estado:

		{
			TRAMA = 1527;
			ACK_OUT = 1232;
			sleep(10);
			if (ACK_IN == ACK_OUT)
			{
				TRAMA = 0000;
				// pasa a hibernación
			}
			else
			{
				if (contador_c == 3)
				{
					// paso a hibernación
					contador_c = 0;
				}
				else
				{
					current_state = cuarto_estado;
					contador_c = contador_c + 1;
				}
			}


			break;
		}
		}

	}
    return 0;
}

int Hibernar()
{
	cout << "Hibernando\n";
	return 0;
}
