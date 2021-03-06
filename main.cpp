#include <iostream>
#include <locale.h>
#include <conio.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VACIO 999999
#define TAMCLIENTE 1000
#define TAMVECTORF 1000

using namespace std;

struct cliente
{
    int usuarioID = 0;
    int fechaCreacion = 0;
    bool activo = true;
    double totalImporteCompras = 0;
    char eMail[50];
};

struct compra
{
    int compraID;
    char fechaHora[500];
    double monto;
    int usuarioID;
    int nroArticulo;
};

void burbujeoPorID(cliente v[],int tam)
{
    int i,j;
    cliente aux;
    for (i=0;i<tam-1;i++)
    {
        for (j=0;j<tam-1-i;j++)
        {
            if (v[j+1].usuarioID < v[j].usuarioID)
            {
                aux = v[j+1];
                v[j+1] = v[j];
                v[j] = aux;
            }
        }
    }
    return;
}

void inicializarVectorClientes(cliente v[], int tam)
{
    for (int i=0;i<tam;i++)
    {
        v[i].usuarioID = VACIO;
    }
    return;
}

int buscarPosicion(cliente v[], int tam)
{
    int i=0;
    while (i<tam && v[i].usuarioID!=VACIO)
        i++;
    if (i>=tam)
        return -1; // no encontr� posici�n vac�a
    else
        return i;
}

void cargarCliente(cliente v[], char mail[])
{
    cout << endl;
    burbujeoPorID(v,TAMCLIENTE);
    int pos = buscarPosicion(v,TAMCLIENTE);
    if (pos!=-1)
    {
        if (mail!=0)
        {
            v[pos].usuarioID = pos;
            cout << "Ingrese la fecha de hoy en formato AAAAMMDD: ";
            cin >> v[pos].fechaCreacion; //intentar hacerlo autom�tico
            v[pos].activo = true;
            v[pos].totalImporteCompras = 0;
            strcpy(v[pos].eMail,mail);
            cout << "\nUsuario ingresado correctamente. Su ID es " << v[pos].usuarioID << endl;
        }
        else
        {
            v[pos].usuarioID = pos;
            cout << "Ingrese la fecha de hoy en formato AAAAMMDD: ";
            cin >> v[pos].fechaCreacion; //intentar hacerlo autom�tico
            v[pos].activo = true;
            v[pos].totalImporteCompras = 0;
            cout << "Ingrese el eMail del usuario: ";
            fflush(stdin);
            gets(v[pos].eMail);
            cout << "\nUsuario ingresado correctamente. Su ID es " << v[pos].usuarioID << endl;
        }
    }
    else
        cout << "Error: vector lleno." << endl; // Ver si Pablo nos deja hacer lo de los usuarios inactivos.
    return;
}

int busquedaPorMail(cliente v[], char mail[], int tam) //devuelve la posici�n en el vector
{
    int i=0;
    cout << "Ingrese el eMail: ";
    fflush(stdin);
    gets(mail); /** FIJARSE SI PODEMOS HACER QUE CHEQUEE QUE TENGA @ */
    while (i<tam && strcmp(v[i].eMail,mail))
        i++;
    if (i>=tam)
        return -1;
    else
        return i;
}

int busquedaPorID(cliente v[],int tam)
{
    int idBuscado;
    cout << "Ingrese el ID: ";
    cin >> idBuscado;
    burbujeoPorID(v,TAMCLIENTE);
    bool encontrado = false;
    int superior = tam-1;
    int inferior = 0;
    int mitad;
    int i=0;
    while ((inferior<=superior) && (i<tam))
    {
        mitad = (inferior + superior)/2;
        if (v[mitad].usuarioID == idBuscado)
        {
            encontrado = true;
            return mitad;
        }
        if (v[mitad].usuarioID > idBuscado)
        {
            superior = mitad;
            mitad = (superior+inferior)/2;
        }
        if (v[mitad].usuarioID < idBuscado)
        {
            inferior = mitad;
            mitad = (inferior + superior)/2;
        }
        i++;
    }
  return -1;
}
void burbujeoPorImporte(cliente v[],int tam)
{
    int i,j;
    cliente aux;
    for (i=0;i<tam-1;i++)
    {
        for (j=0;j<tam-1-i;j++)
        {
            if (v[j+1].totalImporteCompras > v[j].totalImporteCompras)
            {
                aux = v[j+1];
                v[j+1] = v[j];
                v[j] = aux;
            }
        }
    }
    return;
}

int cantDeClientes(cliente v[],int tam){
	int c=0;
	for (int i=0; i<tam;i++)
	{
		if(v[i].usuarioID!=VACIO)
		{
			c++;
		}
	}
	return c;
}

int listarClientes(cliente v[], int tam)
{
	int c=cantDeClientes(v,tam);
	burbujeoPorImporte(v, tam);
	int fecha,aaaa,mm,dd;
	for(int i=0; i<c ;i++)
	{
		if(v[i].activo==true)
		{
			fecha = v[i].fechaCreacion;
        	aaaa = fecha/10000;
        	mm = (fecha-aaaa*10000)/100;
        	dd = (fecha-aaaa*10000-mm*100);
			cout<<"\nUsuarioID: "<< v[i].usuarioID <<"\nFecha De Creaci�n:"<< dd << "/" << mm << "/" << aaaa
			<<"\nTotal Importe De Compras:"<<v[i].totalImporteCompras<<"\neMail"<<v[i].eMail<<endl;
		}
	}
}

int iniciarCompra(FILE *f,cliente v[],int tam)
{
	compra c;
	f = fopen("procesados.bin","rb+");
	int idCliente;
	char x;
	double montoCompra;
	fseek(f,0,SEEK_END);
	c.compraID = (ftell(f)/sizeof(compra));
	cout << "Ingrese fecha y hora: ";
	fflush(stdin);
	gets(c.fechaHora);
	idCliente = busquedaPorID(v,tam);
	while(idCliente==-1)
	{
		cout << "El cliente ingresado no existe �Desea volver a intentarlo? \n1 - Si \n2 - Volver al men�"<<endl;
		x = getch();
		while (x!='1' && x!='2')
			x=getch();
		if(x=='1')
		{
			idCliente = busquedaPorID(v,tam);
		}
		else
		{
			return 1;
		}
	}
	c.usuarioID = v[idCliente].usuarioID;
	cout << "Ingrese el n�mero de articulo: ";
	cin>> c.nroArticulo;
	cout << "Ingrese el monto: $";
    cin >> montoCompra;
    c.monto = montoCompra;
    fwrite(&c,sizeof(compra),1,f);
    fclose(f);
    v[idCliente].totalImporteCompras += montoCompra;
    return 0;
}

char menu()
{
    int i=0;
    char op;
    cout << "\nElija una opci�n:" << endl;
    cout << "a - Cargar cliente." << endl;
    cout << "b - Desactivar cliente." << endl;
    cout << "c - Buscar cliente" << endl;
    cout << "d - Listar clientes por importe." << endl;
    cout << "e - Iniciar compra." << endl;
    cout << "f - Mostrar compras por usuario." << endl;
    cout << "g - Finalizar jornada.\n" << endl;
    do{

		op = getch();
	}
	while ((op<'a' || op>'g') && (op<'A' || op>'G'));
    return op;
}

int levantarClientes(FILE *f,cliente vec[])
{
    cliente x;
    f = fopen("clientes.bin","rb");
    if (!f)
        return 1;
    int i=0;
    while(fread(&x,sizeof(cliente),1,f))
    {
        vec[i] = x;
        i++;
    }
    cout << "Clientes cargados." << endl;
    return 0;
}

void resultadoDeBusqueda(cliente v[],int pos)
{
    if (pos == -1)
        cout << "Error: no hubo coincidencias." << endl << endl;
    else
    {
        int fecha = v[pos].fechaCreacion;
        int aaaa = fecha/10000;
        int mm = (fecha-aaaa*10000)/100;
        int dd = (fecha-aaaa*10000-mm*100);
        cout << endl;
        if (v[pos].activo)
            cout << "Usuario activo" << endl;
        else
            cout << "Usuario inactivo" << endl;
        cout << "ID: " << v[pos].usuarioID << endl;
        cout << "Fecha: " << dd << "/" << mm << "/" << aaaa << endl;
        cout << "Importe total de compras: $" << v[pos].totalImporteCompras << endl;
        cout << "eMail: " << v[pos].eMail << endl;
        cout << endl;
    }
    return;
}

void desactivarCliente(cliente v[],int tam)
{
    int x;
    int y = busquedaPorID(v,tam);
    if (y==-1)
        cout << "No hubo coincidencias." << endl << endl;
    else
    {
        if (v[y].activo)
        {
            v[y].activo = false;
            cout << "Usuario desactivado." << endl << endl;
        }
        else
        {
            cout << "El usuario ya est� inactivo, �desea activarlo? \n1 - Si. \n2 - No." << endl;
            x = getch();
            if(x!='1' && x!='2')
                {
                    cout << "\nEl valor ingresado no corresponde a ninguna opci�n, vuelva a intentarlo." << endl;
                    x = getch();
                }
                while(x!='1' && x!='2')
                    x = getch();
                if (x == '1')
                {
                    v[y].activo = true;
                    cout << "Usuario activado." << endl << endl;
                }
        }
    }
    return;
}

void mostrarCompras(FILE *fcompras,cliente v[],int tam)
{
    int id = busquedaPorID(v,tam);
    char x;
    compra vcompras[TAMVECTORF];
    while (id==-1)
    {
		cout << "El cliente ingresado no existe �Desea volver a intentarlo? \n1 - Si \n2 - Volver al men�"<<endl;
		x = getch();
		while (x!='1' && x!='2')
			x=getch();
		if(x=='1')
		{
			id = busquedaPorID(v,tam);
		}
		else
		{
			return;
		}
	}
	fcompras = fopen("procesados.bin","rb");
	fseek(fcompras,0,SEEK_END);
    int cantDeCompras = (ftell(fcompras)/sizeof(compra));
    compra aux;
    int j = 0;
    for (int i=0;i<cantDeCompras;i++)
    {
        if (vcompras[i].compraID == id)
        {
            cout << "Compra N� " << j+1 << endl;
            cout << "ID: " << vcompras[i].compraID << endl;
            cout << "Fecha y hora: " << vcompras[i].fechaHora << endl;
            cout << "N�mero de art�culo: " << vcompras[i].nroArticulo << endl;
            cout << "Monto: $" << vcompras[i].monto << endl << endl;
            j++;
        }
    }
    if (j==0)
    {
        cout << "El cliente no tiene compras." << endl;
    }
    fclose(fcompras);
    return;
}

void CargarClientesParaPrueba(FILE *f,cliente v[])
{
    v[0].usuarioID = 0;
    v[0].fechaCreacion = 20200905;
    v[0].activo = true;
    v[0].totalImporteCompras = 0;
    fflush(stdin);
    strcpy(v[0].eMail,"cliente0@prueba.com");
    v[1].usuarioID = 1;
    v[1].fechaCreacion = 20200905;
    v[1].activo = true;
    v[1].totalImporteCompras = 15;
    fflush(stdin);
    strcpy(v[1].eMail,"cliente1@prueba.com");
    v[2].usuarioID = 2;
    v[2].fechaCreacion = 20200905;
    v[2].activo = true;
    v[2].totalImporteCompras = 30;
    fflush(stdin);
    strcpy(v[2].eMail,"cliente2@prueba.com");
    //---------------------------------//
    f = fopen("clientes.bin","wb");
    if(f)
    fwrite(v,sizeof(cliente),3,f);
    fclose(f);
    return;
}

int main()
{
    setlocale(LC_ALL,"");
    FILE *fclientes;
    FILE *fprocesados;
    cliente vectorClientes[TAMCLIENTE];
    inicializarVectorClientes(vectorClientes,TAMCLIENTE);
    if(levantarClientes(fclientes,vectorClientes))
    {
    	cout << "Error: no se pudo abrir el archivo. Cierre el programa y vuelva a ejecutarlo." << endl;
    	return 1;
    }
    CargarClientesParaPrueba(fclientes,vectorClientes);
	int posicion;
    char mail[50] = "0";
    char op = menu();
    while (op!='h')
    {
        switch(op)
        {
        case 'a':
        case 'A':
            cargarCliente(vectorClientes,0);
            break;
        case 'b':
        case 'B':
            desactivarCliente(vectorClientes,TAMCLIENTE);
            break;
        case 'c':
        case 'C':
            char x;
            cout << "�Desea buscar por eMail o por ID de cliente?\n1 - eMail.\n2 - ID." << endl;
            x = getch();
            if(x!='1' && x!='2')
            {
                cout << "\nEl valor ingresado no corresponde a ninguna opci�n, vuelva a intentarlo." << endl;
                x = getch();
            }
            while(x!='1' && x!='2')
                x = getch();
            if(x=='1')
                {
                    posicion = busquedaPorMail(vectorClientes,mail,TAMCLIENTE);
                    resultadoDeBusqueda(vectorClientes,posicion);
                    if (posicion == -1)
                    {
                        cout << "�Quiere ingresar a un nuevo cliente con esa direcci�n de eMail?\n1 - Si.\n2 - No." << endl;
                        x = getch();
                        if(x!='1' && x!='2')
                        {
                            cout << "\nEl valor ingresado no corresponde a ninguna opci�n, vuelva a intentarlo." << endl;
                            x = getch();
                        }
                        while(x!='1' && x!='2')
                            x = getch();
                    }
                    if (x=='1')
                        cargarCliente(vectorClientes,mail);
                }
            else
            {
                posicion = busquedaPorID(vectorClientes,TAMCLIENTE);
                resultadoDeBusqueda(vectorClientes,posicion);
            }
            break;
        case 'd':
        case 'D':
            listarClientes(vectorClientes,TAMCLIENTE);
            break;
        case 'e':
        case 'E':
            if (!iniciarCompra(fprocesados,vectorClientes,TAMCLIENTE))
                cout << "Compra cargada correctamente. " << endl;
            break;
        case 'f':
        case 'F':
            mostrarCompras(fprocesados,vectorClientes,TAMCLIENTE);
            break;
        }
        cout << "\nPulse cualquier tecla para continuar... " << endl;
    	getch();
        op = menu();
    }
        //FINALIZAR JORNADA
}
