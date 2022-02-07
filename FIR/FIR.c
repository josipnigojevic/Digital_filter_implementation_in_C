#include <stdio.h>
#include <stdlib.h>
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <windows.h>  
#define DULJINA_POLJA 512
typedef double REAL;
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
void conv(REAL* A, REAL* B, int lenA, int lenB, int* lenC)
{
	int nconv;
	int i, j, i1;
	REAL tmp;
	REAL* C;

	//allocated convolution array	
	nconv = lenA + lenB - 1;
	C = (REAL*)calloc(nconv, sizeof(REAL));

	//convolution process
	for (i = 0; i < nconv; i++)
	{
		i1 = i;
		tmp = 0.0;
		for (j = 0; j < lenB; j++)
		{
			if (i1 >= 0 && i1 < lenA)
				tmp = tmp + (A[i1] * B[j]);

			i1 = i1 - 1;
			C[i] = tmp;
		}
	}

	//get length of convolution array	

	Output_Export(C, DULJINA_POLJA, "odziv_FIR_step");
}

void main() {

	REAL step[DULJINA_POLJA];
	int i;
	for (i = 0;i < DULJINA_POLJA;i++) {
		step[i] = 1;
	}
	REAL impuls[DULJINA_POLJA];
	impuls[0] = 1;
	for (i = 1;i < DULJINA_POLJA;i++) {
		impuls[i] = 1;
	}
	REAL odziv[DULJINA_POLJA];
	double h[18] = { 0.018099,
0.036198,
0.007857,
0.023322,
0.000305,
0.000707,
0.000472,
0.000464,
0.000927,
0.000201,
0.000598,
0.000008,
0.000018,
0.000012,
0.000012,
0.000024,
0.000005,
0.000015 };
	conv(step, h, DULJINA_POLJA, 18, DULJINA_POLJA);
}