/** main.c
 *  VM-translator del progetto nand to tetris
 *  versione 2.0
 *	Autore: Giovanni Fazi
 *
 *  NOTES: ho notato che alcuni file .vm non vengono letti correttamente
 *         dal programma. Suppongo che l' errore sia nella lettura dei file
 *				 di testo da parte del sistema operativo sul quale si esegue la
 *				 traduzione, in quanto creando un nuovo file di testo (.vm si intende)
 *				 e copiando tutto il contenuto del file .vm con il quale la traduzione
 *			   non risulta corretta, la traduzione sul file .vm "nuovo" risulta
 *				 invece corretta.
 *         Quindi in caso di traduzione errata, può darsi che il problema sia
 *				 il precedente. Il programma è stato scritto e testato su Ubuntu gnome
 *				 versione 64 bit 15.10
 */

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "funzioni.h"
#include "sys/types.h"
#include "dirent.h"

void postAsm(FILE* write_file)
{
	// Dopo la traduzione del file .vm metto le funzioni globali
	// queste funzioni però devono essere eseguite solo su chiamata
	// quindi per evitare errori, faccio saltare il PC alla fine del file

	fprintf(write_file, "@ENDFILE\n"
											"0;JMP\n");

	// Scrivo le variabili globali

	fprintf(write_file, "//definisco funzioni globali\n"
											"(VMGlobal.return)\n"
											// frame = LCL
											"@LCL\n"
											"D=M\n"
											"@R13\n"
											"M=D\n"
											// ret = *(frame-5)
											"@5\n"
											"A=D-A\n"
											"D=M\n"
											"@R14\n"
											"M=D\n"
											// *ARG = pop()
											"@SP\n"
											"AM=M-1\n"
											"D=M\n"
											"@ARG\n"
											"A=M\n"
											"M=D\n"
											// sp = ARG++
											"@ARG\n"
											"D=M+1\n"
											"@SP\n"
											"M=D\n"
											// that = *(frame-1)
											"@R13\n"
											"AM=M-1\n"
											"D=M\n"
											"@THAT\n"
											"M=D\n"
											// this = *(frame-2)
											"@R13\n"
											"AM=M-1\n"
											"D=M\n"
											"@THIS\n"
											"M=D\n"
											// arg = *(frame-3)
											"@R13\n"
											"AM=M-1\n"
											"D=M\n"
											"@ARG\n"
											"M=D\n"
											// lcl = *(frame-4)
											"@R13\n"
											"AM=M-1\n"
											"D=M\n"
											"@LCL\n"
											"M=D\n"
											// goto ret
											"@R14\n"
											"A=M\n"
											"0;JMP\n");

	fprintf(write_file, "(VMGlobal.eq)\n"
                      "@SP\n"
                      "AM=M-1\n"
                      "D=M\n"
                      "A=A-1\n"
                      "D=M-D\n"
   									  "@TRUEEQ\n"
                      "D;JEQ\n"
                      "@SP\n"
                      "A=M\n"
                      "A=A-1\n"
                      "M=0\n"
											"@R14\n"       /////
											"A=M\n"
                      "0;JMP\n"      /////
                      "(TRUEEQ)\n"
                      "@SP\n"
                      "A=M\n"
                      "A=A-1\n"
                      "M=-1\n"
											"@R14\n"       /////
											"A=M\n"
											"0;JMP\n");    /////

	fprintf(write_file, "(VMGlobal.gt)\n"
	                    "@SP\n"
	                    "AM=M-1\n"
	                    "D=M\n"
	                    "A=A-1\n"
	                    "D=M-D\n"
	 									  "@TRUEGT\n"
	                    "D;JGT\n"
	                    "@SP\n"
	                    "A=M\n"
	                    "A=A-1\n"
	                    "M=0\n"
											"@R14\n"       /////
											"A=M\n"
	                    "0;JMP\n"      /////
	                    "(TRUEGT)\n"
	                    "@SP\n"
	                    "A=M\n"
	                    "A=A-1\n"
	                    "M=-1\n"
											"@R14\n"       /////
											"A=M\n"
											"0;JMP\n");    /////

	fprintf(write_file, "(VMGlobal.lt)\n"
	                    "@SP\n"
	                    "AM=M-1\n"
	                    "D=M\n"
	                    "A=A-1\n"
	                    "D=M-D\n"
	 									  "@TRUELT\n"
	                    "D;JLT\n"
	                    "@SP\n"
	                    "A=M\n"
	                    "A=A-1\n"
	                    "M=0\n"
											"@R14\n"       /////
											"A=M\n"
	                    "0;JMP\n"      /////
	                    "(TRUELT)\n"
	                    "@SP\n"
	                    "A=M\n"
	                    "A=A-1\n"
	                    "M=-1\n"
											"@R14\n"       /////
											"A=M\n"
											"0;JMP\n");    /////

	// scrittura comandi                   counter[3] è il conta calls
  fprintf(write_file, "//call\n"
											"(VMGlobal.call)\n"
                      // push return address
                      "@SP\n"
                      "M=M+1\n"
                      "A=M-1\n"
                      "M=D\n"
                      // push LCL
                      "@LCL\n"
                      "D=M\n"
                      "@SP\n"
                      "M=M+1\n"
                      "A=M-1\n"
                      "M=D\n"
 											// push ARG
                      "@ARG\n"
                      "D=M\n"
                      "@SP\n"
                      "M=M+1\n"
                      "A=M-1\n"
                      "M=D\n"
                      // push THIS
                      "@THIS\n"
                      "D=M\n"
                      "@SP\n"
                      "M=M+1\n"
                      "A=M-1\n"
                      "M=D\n"
                      // push THAT
                      "@THAT\n"
                      "D=M\n"
                      "@SP\n"
                      "M=M+1\n"
                      "A=M-1\n"
                      "M=D\n"
                      // ARG = SP-nArg-5
                      "@SP\n"
                      "D=M\n"
                      "@5\n"
                      "D=D-A\n"
                      "@R13\n"
                      "D=D-M\n"
                      "@ARG\n"
                      "M=D\n"
                      // LCL = SP
                      "@SP\n"
                      "D=M\n"
                      "@LCL\n"
                      "M=D\n"
                      // goto nomefunzione
                      "@R14\n"
											"A=M\n"
                      "0;JMP\n");

		// Loop infinito per chiudere l' esecuzione
		fprintf(write_file, "(ENDFILE)\n"
												"@ENDFILE\n"
												"0;JMP\n");
}

void initAsm(FILE* write_file)
{
	// Inizializza SP e chiama sys.init
	fprintf(write_file, "@256\n"
											"D=A\n"
											"@SP\n"
											"M=D\n"
											"//call Sys.init\n"
                      // push return address
                      "@RETURN-1\n"
                      "D=A\n"
                      "@SP\n"
                      "A=M\n"
                      "M=D\n"
                      "@SP\n"
                      "M=M+1\n"
                      // push LCL
                      "@LCL\n"
                      "D=M\n"
                      "@SP\n"
                      "A=M\n"
                      "M=D\n"
                      "@SP\n"
											"M=M+1\n"
                      // push ARG
                      "@ARG\n"
                      "D=M\n"
                      "@SP\n"
                      "A=M\n"
                      "M=D\n"
                      "@SP\n"
                      "M=M+1\n"
                      // push THIS
                      "@THIS\n"
                      "D=M\n"
                      "@SP\n"
                      "A=M\n"
                      "M=D\n"
                      "@SP\n"
                      "M=M+1\n"
                      // push THAT
                      "@THAT\n"
                      "D=M\n"
                      "@SP\n"
                      "A=M\n"
                      "M=D\n"
                      "@SP\n"
                      "M=M+1\n"
                      // ARG = SP-nArg-5
                      "@SP\n"
                      "D=M\n"
                      "@5\n"
                      "D=D-A\n"
                      "@ARG\n"
                      "M=D\n"
                      // LCL = SP
                      "@SP\n"
                      "D=M\n"
                      "@LCL\n"
                      "M=D\n"
                      // goto nomefunzione
                      "@Sys.init\n"
                      "0;JMP\n"
                      "(RETURN-1)\n");
}

/*	Precondizione: nomef_in è il nome senza estensione del file .vm da aprire
 *								 path è la path della cartella invocata dal terminale
 *								 counters contiene un array con i contatori per la traduzione
 *
 *  Postcondizione: scrive nel file out.asm la traduzione del file nomef_in.vm
*/
void translate(char nomef_in[], char path[], int counters[])
{
	FILE *file_open, *file_write;		// puntatori per aprire i files
	char pathIn[600], pathOut[600];	// conterranno la path + nomefile di in e out
	char *endf;											// puntatore che rileva la EOF
	char line[101];									// array che tiene la riga letta
	int commandType;								// variabile che tiene il tipo di comando letto
	char funcname[50];							// nome dell' ultima funzione trovata

	funcname[0] = '\0';							// inizializzo funcname

	sprintf(pathIn, "%s/%s.vm", path, nomef_in);
	sprintf(pathOut, "%s/out.asm", path);

	file_open = fopen(pathIn, "r");
	file_write = fopen(pathOut, "a");	// !scrittura in appendice!

	while (1)
	{
		endf = fgets(line, 100, file_open);	// Lettura riga

		if (endf == NULL) {break;}					// esce se EOF

		if (line[0] != '/' && line[1] != '\n')	// ignora le righe vuote e commenti
		{
			commandType = trimline(&line[0]);

			// se ho letto una funzione, metto il nome in funcname
			if (commandType == 3 && line[0] == 'f')
			{
				int t = 0;

				for (int i = 9; line[i] != ' '; i++)
				{
					funcname[t] = line[i];
					t++;
				}

				funcname[t] = '\0';
				//printf("ho la funzione |%s|\n", funcname);
			}
			else
			// se leggo return cancello il nome della funzione
			if (commandType == 3 && line[0] == 'r')
			{
				funcname[0] == '\0';
			}

			switch (commandType)
			{
				case 0:
					translateArit(&line[0], file_write, &counters[0]);
				break;

				case 1:
					translateP(&line[0], file_write, &nomef_in[0]);
				break;

				case 2:
					translateFlow(&line[0], file_write, &funcname[0]);
				break;

				case 3:
					translateFunc(&line[0], file_write, &counters[0]);
				break;
			}
		}
	}

	fclose(file_open);
	fclose(file_write);
	return;
}

/* Precondizione: il programma deve essere invocato da terminale con la path
 * 								della directory (es: ./VMtranslator /home/pippo/test).
 *								La directory deve contenere più di un file .vm e i file devono
 *								essere privi di errori, inoltre il programma non gestisce
 *								eventuali codifiche di testo speciali dei sistemi operativi
 *
 * Postcondizione: nella directory invocata, viene generato un file chiamato
 *								 out.asm
 *
*/
int main(int argc, char *argv[])
{
	int counters[4] = {0,0,0,0};	// array per i vari counters eq,gt,lt,call
	char vmfilename[50];					// contiene il nome del file .vm senza estensione
	DIR *directory;								// la directory da aprire
	struct dirent *file;					// legge i file nella directory

	// Crea subito il file out.asm inizializzato col Sys.init
	char asmOutPath[600];
	sprintf(asmOutPath, "%s/out.asm", argv[1]);
	FILE *w_init = fopen(asmOutPath, "w");
	initAsm(w_init);
	fclose(w_init);

	// Apertura directory passata come argomento dal terminale
	directory = opendir(argv[1]);

	// Scorre tutti i file nella directory
	while ((file = readdir(directory)) != NULL)
	{
		// Se trova un file .vm
		if (strstr(file->d_name, ".vm") != NULL)
		{
			// in vmfilename metto il nome del file .vm e tolgo l' estensione
			strcpy(vmfilename, file->d_name);
			vmfilename[strlen(vmfilename)-3] = '\0';

			// chiamo la funzione traduci sul file
			translate(vmfilename, argv[1], &counters[0]);
		}
	}

	// Una volta finiti i file .vm scrivo le funzioni globali
	sprintf(asmOutPath, "%sout.asm", argv[1]);
	w_init = fopen(asmOutPath, "a");
	postAsm(w_init);
	fclose(w_init);

	// poi chiudo la directory e il programma
	closedir(directory);
	return 0;
}
