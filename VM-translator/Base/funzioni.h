/** funzioni.h
 *  VM-translator del progetto nand to tetris
 *  versione 1.0
 *	Autore: Giovanni Fazi
 */

 #include "string.h"
 #include "stdio.h"
 #include "stdlib.h"

int change_extension(char dotvm[], char dotasm[]);

int trimline(char rawline[]);

void translateArit(char line[], FILE *write_file, int counters[]);

void translateFlow(char line[], FILE *write_file);

void translateP(char line[], FILE *write_file, char filename[]);

void translateFunc(char line[], FILE *write_file, int counters[]);
