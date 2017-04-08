/** main.c
 *  VM-translator del progetto nand to tetris
 *  versione 1.0
 *	Autore: Giovanni Fazi
 */

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "funzioni.h"

int main()
{
	// Dichiarazione variabili
	char nomef_in[31], nomef_out[35];
	int nome_corretto;

	// Richiesta del file .vm da tradurre
	do
	{
		printf("Inserisci il nome del file con estensione .vm da tradurre: ");
		scanf("%s", &nomef_in[0]);
		nome_corretto = change_extension(&nomef_in[0], &nomef_out[0]);
	}
	while(nome_corretto == 0);

	// Controllo nome output giusto
	//printf("|%s| -> |%s|\n", nomef_in, nomef_out);

	//-------------------------------------------------------|
	// Apertura e lettura file                               |
	//-------------------------------------------------------|

	FILE *file_open, *file_write;		// puntatori per aprire i files
	char *endf;								// puntatore che rileva la EOF
	char line[101];						// array che tiene la riga letta
	int commandType;						// variabile che tiene il tipo di comando letto
   int counters[4] = {0,0,0,0};		// contatori usati nelle traduzioni
	// 0 = eq counter
	// 1 = gt counter
	// 2 = lt counter
	// 3 = call counter

	file_open = fopen(nomef_in, "r");
	file_write = fopen(nomef_out, "w");

	// Inizializzo il file di scrittura
	fprintf(file_write, "@256\nD=A\n@SP\nM=D\n");

	// Lettura file
	if (file_open == NULL)
	{
		perror("Error while trying to open the file");
    exit(1);
	}
	else
	{
		while (1)
		{
			endf = fgets(line, 100, file_open);	// Lettura riga

			if (endf == NULL) {break;}					// esce se EOF

			if (line[0] != '/' && line[1] != '\n')	// ignora le righe vuote e commenti
			{
				commandType = trimline(&line[0]);

				//printf("%d %s\n", commandType, line);

				switch (commandType)
				{
					case 0:
						translateArit(&line[0], file_write, &counters[0]);
					break;

					case 1:
						translateP(&line[0], file_write, &nomef_in[0]);
					break;

					case 2:
						translateFlow(&line[0], file_write);
					break;

					case 3:
						translateFunc(&line[0], file_write, &counters[0]);
					break;
				}
			}
		}
	}

	fclose(file_open);
	fclose(file_write);
	return 0;
}
