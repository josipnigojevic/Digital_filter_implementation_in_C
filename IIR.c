#include <stdio.h>
#include <stdlib.h>
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <windows.h>  
#define DULJINA_POLJA 512
typedef short int REAL;
typedef struct {
	REAL Akoef[7];
	REAL Bkoef[7];
} IIR;
//***************************************EXPORT FUNKCIJA**********************************************************//
void Output_Export(REAL* output, int N, const char szFileName[]) {
	FILE* hFile = NULL;
	fopen_s(&hFile, szFileName, "w");
	if (!hFile)
	{
		exit(-1);
		return;
	}
	const int iElementsPerLine = 1;
	int i;
	for (i = 0; i < N; i++)
	{
		fprintf_s(hFile, "%hi, ", output[i]);
		if (i % iElementsPerLine == 0)
		{
			fprintf_s(hFile, "\n");
		}
	}
	fclose(hFile);
}
//**********************************DEFINICIJA FUNKCIJE FILTRIRANJA***********************************************//
void filter(IIR* X, REAL* Signal, REAL* FilteredSignal) {
	int i, j = 6;
	FilteredSignal[0] = X->Akoef[0] * Signal[0];
	FilteredSignal[1] = X->Bkoef[0] * FilteredSignal[0] + X->Akoef[0] * Signal[0] + X->Akoef[1] * Signal[1];
	FilteredSignal[2] = (1 + X->Bkoef[1]) * FilteredSignal[1] + X->Akoef[2] * Signal[2];
	FilteredSignal[3] = (1 + X->Bkoef[2]) * FilteredSignal[2] + X->Akoef[3] * Signal[3];
	FilteredSignal[4] = (1 + X->Bkoef[3]) * FilteredSignal[3] + X->Akoef[4] * Signal[4];
	FilteredSignal[5] = (1 + X->Bkoef[4]) * FilteredSignal[4] + X->Akoef[5] * Signal[5];

	for (i = 6;i < DULJINA_POLJA;i++) {
		FilteredSignal[i] = (1 + X->Bkoef[j - 1]) * FilteredSignal[i - 1] + X->Akoef[j] * Signal[i];
		j++;
		if (j > 6) { j = 0; }
	}
	Output_Export(FilteredSignal, DULJINA_POLJA,"impulsni_odziv");

}

void main() {
	//******************************DEFINICIJA ULAZNIH SIGNALA****************************************************//
	REAL impuls[DULJINA_POLJA];
	int i;
	impuls[0] = 1;
	for (i = 1;i < DULJINA_POLJA;i++) {
		impuls[i] = 0;
	}
	REAL step[DULJINA_POLJA] = { 1 };
	REAL odziv[DULJINA_POLJA];
	//************************************DEFINICIJA IIR FILTRA**************************************************//
	//karakteristike IIR filtra:
	//Tip: IIR butterworth pojasnopropusni filter
	//Frekvencija uzorkovanja: 40000 [Hz]
	//Sesti red
	//Donja granièna frekvencija: 8400[Hz]
	//Gornja granièna frekvencija: 13200[Hz]
	IIR A={
	.Akoef = { 0.0286353001709,
		0,
		-0.0859059005127,
		0,
		0.0859059005127,
		0,
		-0.0286353001709 },
	.Bkoef = { 1,
		0.60914578929460,
		1.64606563196561,
		0.67736112411917,
		1.03408539337512,
		0.21516326623677,
		0.21204593432676 } };

	filter(&A,impuls,odziv);

}