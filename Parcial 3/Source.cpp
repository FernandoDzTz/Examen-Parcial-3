#include <windows.h>
#include<conio.h>
#include<stdio.h>
#include<math.h>
#include "resource.h"


int boletos;
char conciertos[10][50] = {"Concierto 1","Concierto 2","Concierto 3","Concierto 4","Concierto 5","Concierto 6","Concierto 7","Concierto 8","Concierto 9","Concierto 10" };
float descuento;
char preciosiva[100];
char precio[100];
float gasto;
char num[20];

HINSTANCE LOGIN;
HINSTANCE EVENTOS;
HINSTANCE Promo;
HINSTANCE COBRO;
HINSTANCE Recibo;

HWND edit1;
HWND edit2;
HWND login;

LRESULT CALLBACK Ceventos(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Cpromo(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Ccobro(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Crecibo(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Clogin(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	DialogBox(LOGIN, MAKEINTRESOURCE(IDD_LOGIN), 0, Clogin);
	return 0;
}

LRESULT CALLBACK Ceventos(HWND handler, UINT message, WPARAM wPARAM, LPARAM lPARAM)
{
	
	switch (message){
	
	case WM_INITDIALOG: 
	
		for (int i = 0; i < 10; i++)
		{
			SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)conciertos[i]);
		}

		break;
	
	case WM_COMMAND:
		switch (LOWORD(wPARAM))
		{
		case IDC_BUTTON1:
			int indice = SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_GETCURSEL, 0, 0);
			if (indice == CB_ERR) {
				
				MessageBox(handler, "Por favor, seleccione un concierto", "Error", MB_OK | MB_ICONERROR);
				break; 
			}


			GetDlgItemText(handler, IDC_EDIT1, num, 20);
			boletos = atoi(num);
			EndDialog(handler, IDD_EVENTOS);

			DialogBox(Promo, MAKEINTRESOURCE(IDD_PROMO), 0, Cpromo);

			break;
		}

	}

	return FALSE;
}

LRESULT CALLBACK Clogin(HWND handler, UINT message, WPARAM wPARAM, LPARAM lPARAM) {
	login = handler;
	edit1 = GetDlgItem(handler, IDC_EDIT1);
	edit2 = GetDlgItem(handler, IDC_EDIT2);
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wPARAM))
		{
		case IDC_BUTTON1:
			char usuario[100];
			char contrasena[100];
			GetWindowText(edit1, usuario, sizeof(usuario));
			GetWindowText(edit2, contrasena, sizeof(contrasena));

			if (strcmp(usuario, "Fernando") == 0 && strcmp(contrasena, "12345") == 0) {
				EndDialog(handler, IDOK);  
				
				DialogBox(EVENTOS, MAKEINTRESOURCE(IDD_EVENTOS), 0, Ceventos);
			}
			else {
				MessageBoxA(handler, "Ingreso incorrecto. Inténtalo de nuevo.", "Error", MB_OK | MB_ICONERROR);
			}
			
			break;
		}
		break;

	}

	return FALSE;
}

LRESULT CALLBACK Crecibo(HWND handler, UINT message, WPARAM wPARAM, LPARAM lPARAM) {

	switch (message)
	{
	case WM_INITDIALOG:

		SetDlgItemText(handler, IDC_stGASTO, precio);
		SetDlgItemText(handler, IDC_stTotal, preciosiva);
		SetDlgItemText(handler, IDC_stBOLETOS, num);
		break;

	case WM_COMMAND:
		switch (LOWORD(wPARAM)) {
		case IDC_BUTTON1:
			DestroyWindow(handler);
			PostQuitMessage(0);
			break;
		}
	}
	return FALSE;

	
}

LRESULT CALLBACK Ccobro(HWND handler, UINT message, WPARAM wPARAM, LPARAM lPARAM){
	switch (message)
	{
	case WM_INITDIALOG:
		float precioSIVA;
		float precioIVA;
		precioSIVA = gasto;
		precioIVA = gasto * 1.16;
		sprintf_s(preciosiva, "%.2f", precioSIVA);
		sprintf_s(precio, "%.2f", precioIVA);
		SetDlgItemText(handler, IDC_stTotal, preciosiva);
		SetDlgItemText(handler, IDC_stCOBRO, precio);
		break;
	case WM_COMMAND:
		switch (LOWORD(wPARAM)){
		case IDC_PAGAR:
			EndDialog(handler, IDD_COBRO);
			DialogBox(Recibo, MAKEINTRESOURCE(IDD_RECIBO), 0, Crecibo);
			break;
		}
	
	}
	return FALSE;
}

LRESULT CALLBACK Cpromo(HWND handler, UINT message, WPARAM wPARAM, LPARAM lPARAM) {
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wPARAM))
		{
		case IDC_BUTTON1:
			int cantidad;
			cantidad = boletos;
			descuento = 800 * .10;
			gasto = (cantidad * 800) - descuento;
			EndDialog(handler, IDD_PROMO);

				DialogBox(COBRO, MAKEINTRESOURCE(IDD_COBRO), 0, Ccobro);

				break;
			case IDC_BUTTON2:
				if ((boletos % 2)==0) {
					int can;
					can = boletos / 2;
					gasto = can * 800;

					EndDialog(handler, IDD_PROMO);

				DialogBox(COBRO, MAKEINTRESOURCE(IDD_COBRO), 0, Ccobro);
				}
				else {
					MessageBox(NULL, "Cantidad no autorizada", "Advertencia", MB_OK | MB_ICONINFORMATION);

					EndDialog(handler, IDD_PROMO);

					DialogBox(Promo, MAKEINTRESOURCE(IDD_PROMO), 0, Cpromo);

				}
				
				break;
		}
	}
	return FALSE;

}


