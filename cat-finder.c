#include <stdio.h>
#include <stdlib.h>
#define PLIN 1
#define GOL 0
#define NESET 8
#define MULTE 9

int* bitii(int *poz, int *lung, int nelem, int nbiti)
{
	int i, j;
	static int b[100];
	for (i=0; i<nbiti; i++)
		b[i] = 0;
	for (i=0; i<nelem; i++)
		for (j=0; j<lung[i]; j++)
			b[poz[i]+j] = 1;
	return b;
}
int* generare(int *lung, int nelem, int *aflat, int nbiti)
{
	int poz[100];
	static int sigur[100];
	int limita = nbiti - lung[nelem-1];
	int i, j, k;

	/* completez initial */
	for (i=0, j=0; i<nelem; j+=lung[i++]+1)
		poz[i] = j;
	for (i=0; i<nbiti; i++)
		sigur[i] = NESET;

	for (;;)
	{
		/* prelucrez. daca sirul este valid, setez ce e nou */
		int *sir = bitii(poz, lung, nelem, nbiti);
		int valid=1;
		for (i=0; i<nbiti; i++)
			if (aflat[i] != MULTE && aflat[i] != sir[i])
				{ valid=0; break; }
		if (valid)
		{
			for (i=0; i<nbiti; i++)
				if (sigur[i] == NESET)
					sigur[i] = sir[i];
				else if (sigur[i] != MULTE && sigur[i] != sir[i])
					sigur[i] = MULTE;
		}

		/* daca marit, depaseste */
		if (poz[nelem-1]+1 > limita)
		{
			/* caut primul (de la coada) care poate fi marit) */
			for (i=nelem-2; i>=0; i--)
				if (poz[i]+lung[i]+1 < poz[i+1]) break;
			
			/* daca mai mic ca 0, am terminat generarea */
			if (i<0) break;

			/* maresc si completez restul */
			poz[i]++;
			for (j=i+1; j<nelem; j++)
				poz[j] = poz[j-1] + lung[j-1] + 1;
		}
		/* altfel mareste ultimul */
		else poz[nelem-1]++;
	}
	return sigur;
}
void afisare(int mat[100][100], int n, int m)
{
	int i, j;
	printf("/");   for (i=0; i<m; i++) printf("--");   printf("\\\n");
	for (i=0; i<n; i++)
	{
		putchar('|');
		for (j=0; j<m; j++)
			if (mat[i][j] == 1) printf("##");
			else if (mat[i][j] == GOL) printf("  ");
			else printf("..");
		puts("|");
	}
	printf("\\");   for (i=0; i<m; i++) printf("--");   printf("/\n");
}
int main(void)
{
	int corectlin[100][20], corectcol[100][20], nclin[100], nccol[100];
	int linii=0, coloane=0;
	FILE* cit;
	char linie[300];

	/* citire corectlin.txt */
	cit = fopen("corectlin.txt", "r");
	if (!cit) { perror("nu exista fisierul corectlin.txt\n"); exit(1); }
	while (fgets(linie, sizeof(linie), cit))
	{
		nclin[linii] = 0;
		char *start = linie;
		int plus;
		while (sscanf(start, "%d%n", &corectlin[linii][nclin[linii]], &plus) == 1)
		{
			start += plus;
			nclin[linii]++;
		}
		linii++;
	}
	fclose(cit);

	/* citire corectcol.txt */
	cit = fopen("corectcol.txt", "r");
	if (!cit) { perror("nu exista fisierul corectcol.txt\n"); exit(1); }
	while (fgets(linie, sizeof(linie), cit))
	{
		nccol[coloane] = 0;
		char *start = linie;
		int plus;
		while (sscanf(start, "%d%n", &corectcol[coloane][nccol[coloane]], &plus) == 1)
		{
			start += plus;
			nccol[coloane]++;
		}
		coloane++;
	}
	fclose(cit);

	/* construire matrice */
	int matrice[100][100], i, j;
	for (i=0; i<linii; i++)
		for (j=0; j<coloane; j++)
			matrice[i][j] = MULTE;


	int corect[100] = {5, 15};
	int selcol[100];
	int* sigurul;
	int mod;
	int modificat;
	for (mod=0; mod<10; mod++)
	{
		modificat=0;
		for (i=0; i<linii; i++)
		{
			sigurul = generare(corectlin[i], nclin[i], matrice[i], coloane);
			for (j=0; j<coloane; j++)
				if (matrice[i][j]==MULTE && sigurul[j]!=MULTE)
				{
					matrice[i][j] = sigurul[j];
					modificat=1;
				}
		}
		afisare(matrice, linii, coloane);
		if (!modificat) break;

		modificat=0;
		for (i=0; i<coloane; i++)
		{
			for (j=0; j<linii; j++)
				selcol[j] = matrice[j][i];
			sigurul = generare(corectcol[i], nccol[i], selcol, linii);
			for (j=0; j<linii; j++)
				if (matrice[j][i]==MULTE && sigurul[j]!=MULTE)
				{
					matrice[j][i] = sigurul[j];
					modificat=1;
				}
		}
		afisare(matrice, linii, coloane);
		if (!modificat) break;
	}

	printf("Sfarsit\n");
	return 0;
}
