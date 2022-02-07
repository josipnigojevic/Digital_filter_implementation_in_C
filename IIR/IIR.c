#include <stdio.h>
#include <stdlib.h>
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <windows.h>  
#define DULJINA_POLJA 512
typedef double REAL;
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
		fprintf_s(hFile, "%lf, ", output[i]);
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
	if (Signal[1] == 0) {
		Output_Export(FilteredSignal, DULJINA_POLJA, "impulsni_odziv");
	}
	else {
		Output_Export(FilteredSignal, DULJINA_POLJA, "step_odziv");
	}
}

void main() {
	//******************************DEFINICIJA ULAZNIH SIGNALA****************************************************//
	REAL impuls[DULJINA_POLJA];
	int i;
	impuls[0] = 1;
	for (i = 1;i < DULJINA_POLJA;i++) {
		impuls[i] = 0;
	}
	REAL step[DULJINA_POLJA];
	for (i = 0;i < DULJINA_POLJA;i++) {
		step[i] = 1;
	}
	REAL odziv[DULJINA_POLJA];
	//************************************DEFINICIJA IIR FILTRA**************************************************//
	//karakteristike IIR filtra:
	//Tip: IIR butterworth pojasnopropusni filter
	//Frekvencija uzorkovanja: 40000 [Hz]
	//Sesti red
	//Donja grani�na frekvencija: 8400[Hz]
	//Gornja grani�na frekvencija: 13200[Hz]
	IIR A = {
	.Akoef = {  0.018098933007514465820531057715925271623,
	0,
- 0.054296799022543397461593173147775814869,
0,
 0.054296799022543397461593173147775814869,
 0,
- 0.018098933007514465820531057715925271623 },
	.Bkoef = { 1,
		-0.782955158144867269243150076363235712051,
 1.968448175896390761607790409470908343792,
-0.986921186160320873526075047266203910112,
 1.317087732945881217716532773920334875584,
-0.331778114794525014197290602169232442975,
 0.278059917634546460707412052215659059584 } };

	filter(&A, impuls, odziv);
	filter(&A, step, odziv);

}