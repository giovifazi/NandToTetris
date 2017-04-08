/** funzioni.c
 *  VM-translator del progetto nand to tetris
 *  versione 1.0
 *	Autore: Giovanni Fazi
 */

 #include "string.h"
 #include "stdio.h"
 #include "stdlib.h"
 #include "ctype.h"

void translateFunc(char line[], FILE *write_file, int counters[])
{
  // return
  if (line[0] == 'r') {
    fprintf(write_file, "//return\n"
                        "@LCL\n"
                        "D=M\n"
                        "@R5\n"
                        "M=D\n"
                        "@3\n"
                        "A=D-A\n"
                        "D=M\n"
                        "@R6\n"
                        "M=D\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "@ARG\n"
                        "A=M\n"
                        "M=D\n"
                        "@ARG\n"
                        "D=M+1\n"
                        "@SP\n"
                        "M=D\n"
                        "@R5\n"
                        "A=M-1\n"
                        "D=M\n"
                        "@ARG\n"
                        "M=D\n"
                        "@R5\n"
                        "D=M\n"
                        "@2\n"
                        "A=D-A\n"
                        "D=M\n"
                        "@LCL\n"
                        "M=D\n"
                        "@R6\n"
                        "A=M\n"
                        "0;JMP\n");

                        return;}

  // call
  if (line[0] == 'c')
  {
    char nome[50], nArg[4];
    int i, k = 0, h = 0;

    // copio il nome
    for(i = 5; line[i] != ' '; i++)
    {
      nome[k] = line[i];
      k++;
    }

    // copio il numero
    i++;
    while(line[i] != '\0')
    {
      nArg[h] = line[i];
      i++;
      h++;
    }
    nArg[h] = '\0';

    // scrittura comandi
    fprintf(write_file, "//call\n"
                        "@RETURN%d\n"
                        "D=A\n"
                        "@SP\n"
                        "A=M\n"
                        "M=D\n"
                        "@SP\n"
                        "M=M+1\n"
                        "@LCL\n"
                        "D=M\n"
                        "@SP\n"
                        "A=M\n"
                        "M=D\n"
                        "@SP\n", counters[3]);
    fprintf(write_file, "M=M+1\n"
                        "@ARG\n"
                        "D=M\n"
                        "@SP\n"
                        "A=M\n"
                        "M=D\n"
                        "@SP\n"
                        "M=M+1\n"
                        "@SP\n"
                        "D=M\n"
                        "@3\n"
                        "D=D-A\n"
                        "@%s\n", nArg);
    fprintf(write_file, "D=D-A\n"
                        "@ARG\n"
                        "M=D\n"
                        "@SP\n"
                        "D=M\n"
                        "@LCL\n"
                        "M=D\n"
                        "@%s\n"
                        "0;JMP\n"
                        "(RETURN%d)\n", nome, counters[3]);

    counters[3]++;
    return;
  }

  // function
  if (line[0] == 'f')
  {
    char nome[50], num[4];
    int i, k = 0, h = 0, conv;

    // copio il nome
    for(i = 9; line[i] != ' '; i++)
    {
      nome[k] = line[i];
      k++;
    }
    nome[k] = '\0';

    // copio il numero
    i++;
    while(line[i] != '\0')
    {
      num[h] = line[i];
      i++;
      h++;
    }
    num[h] = '\0';

    // conversione num da str a intero
    conv = atoi(num);

    // scrittura comandi
    fprintf(write_file, "//function\n"
                        "(%s)\n", nome);

    for(i = 0; i < conv; i++)
      fprintf(write_file, "@SP\n"
                          "M=M+1\n"
                          "A=M-1\n"
                          "M=0\n");
  }


  //printf("|%s|\n", line);
}

void translateP(char line[], FILE *write_file, char filename[])
{
  int i = 0, n = 0, k = 0;

  // lavoro in filename
  while(filename[k] != '.')
    k++;

  filename[++k] = '\0';

  //printf("|%s|\n", filename);

  // serve a contenere il numero del comando
  char num[17];

  switch (line[1])
  {
    // Pop
    case 'o':
      switch (line[4])
      {
        // Argument
        case 'a':
          // metto il numero in num
          for(i = 13; line[i] != '\0'; i++)
          {
            if (line[i] == ' ')
            {
              num[n] = '\0';
              break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          fprintf(write_file, "//pop argument\n"
                              "@ARG\n"
                              "D=M\n"
                              "@%s\n"
                              "D=D+A\n"
                              "@R15\n"
                              "M=D\n"
                              "@SP\n"
                              "AM=M-1\n"
                              "D=M\n"
                              "@R15\n"
                              "A=M\n"
                              "M=D\n", num);
        break;

        // Local
        case 'l':
          // metto il numero in num
          for(i = 10; line[i] != '\0'; i++)
          {
            if (line[i] == ' ' || line[i] == '\t')
            {
              num[n] = '\0';
              break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          fprintf(write_file, "//pop Local\n"
                              "@LCL\n"
                              "D=M\n"
                              "@%s\n"
                              "D=D+A\n"
                              "@R15\n"
                              "M=D\n"
                              "@SP\n"
                              "AM=M-1\n"
                              "D=M\n"
                              "@R15\n"
                              "A=M\n"
                              "M=D\n", num);
        break;

        // Static
        case 's':
          // metto il numero in num
          for(i = 11; line[i] != '\0'; i++)
          {
            if (line[i] == ' ')
            {
              num[n] = '\0';
              break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          // creo il nomefile.num
          char var[80];
          var[0] = '\0';

          strcat(var, filename);
          strcat(var, num);

          fprintf(write_file, "//pop static\n"
                              "@SP\n"
                              "AM=M-1\n"
                              "D=M\n"
                              "@%s\n"
                              "M=D\n", var);
        break;
      }
    break;

    // Push
    case 'u':
      switch (line[5])
      {
        // Argument
        case 'a':
          // metto il numero in num
          for(i = 14; line[i] != '\0'; i++)
          {
            if (line[i] == ' ')
            {
                num[n] = '\0';
                break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          fprintf(write_file, "//push argument\n"
                              "@ARG\n"
                              "D=M\n"
                              "@%s\n"
                              "A=D+A\n"
                              "D=M\n"
                              "@SP\n"
                              "A=M\n"
                              "M=D\n"
                              "@SP\n"
                              "M=M+1\n", num);
        break;

        // Local
        case 'l':
          // metto il numero in num
          for(i = 11; line[i] != '\0'; i++)
          {
            if (line[i] == ' ')
            {
                num[n] = '\0';
                break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          fprintf(write_file, "//push local\n"
                              "@LCL\n"
                              "D=M\n"
                              "@%s\n"
                              "A=D+A\n"
                              "D=M\n"
                              "@SP\n"
                              "A=M\n"
                              "M=D\n"
                              "@SP\n"
                              "M=M+1\n", num);
        break;

        // Constant
        case 'c':
          // metto il numero in num
          for(i = 14; line[i] != '\0'; i++)
          {
            if (line[i] == ' ')
            {
              num[n] = '\0';
              break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          fprintf(write_file, "//push constant\n"
                              "@%s\n"
                              "D=A\n"
                              "@SP\n"
                              "A=M\n"
                              "M=D\n"
                              "@SP\n"
                              "M=M+1\n", num);
        break;

        // Static
        case 's':
          // metto il numero in num
          for(i = 12; line[i] != '\0'; i++)
          {
            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          // creo il nomefile.num
          char var[80];
          var[0] = '\0';

          strcat(var, filename);
          strcat(var, num);

          fprintf(write_file, "//push static\n"
                              "@%s\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n", var);
        break;
      }
    break;
  }

  //printf("|%s|\n", line);
}

void translateFlow(char line[], FILE *write_file)
{
  // divido c dal comando
  int i = 0, k = 0;
  char c[100];

  //printf("ho la linea |%s|\n", line);

  // trovo il primo spazio nel comando che è la posizione da cui
  // inizio a copiare c
  while(line[i] != ' ')
  {
    //printf("line[i] = |%c|\n",line[i]);
    i++;
  }

  i++;
  //printf("prima di iniziale la copia |%c|\n", line[i]);

  // copio c finchè non arrivo a \0 o trovo uno spazio o un \n
  for(i; isspace(line[i]) == 0; i++)
  {
    c[k] = line[i];
    //printf("ho copiato |%c| e |%c|\n", line[i], c[k]);
    k++;
  }

  // delimito la stringa c
  c[k] = '\0';

  // label c
  if (line[0] == 'l')
  {
    fprintf(write_file, "//label\n"
                        "(%s)\n", c);

    //printf("|%s|\n", c);
    return;
  }

  // goto c
  if (line[0] == 'g')
    fprintf(write_file, "//goto\n"
                        "@%s\n"
                        "0;JMP\n", c);

  // if-goto c
  if (line[0] == 'i')
    fprintf(write_file, "//if-goto\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "@%s\n"
                        "D;JNE\n", c);

  //printf("|%s| |%s|\n", line, c);
}

void translateArit(char line[], FILE *write_file, int counters[])
{
  // add
  if (line[0] == 'a' && line[1] == 'd' && line[2] == 'd')
    fprintf(write_file, "//add\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "M=D+M\n");

  // sub
  if (line[0] == 's' && line[1] == 'u' && line[2] == 'b')
    fprintf(write_file, "//sub\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "M=M-D\n");

  // and
  if (line[0] == 'a' && line[1] == 'n' && line[2] == 'd')
    fprintf(write_file, "//and\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "M=D&M\n");

  // or
  if (line[0] == 'o' && line[1] == 'r')
    fprintf(write_file, "//or\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "M=D|M\n");

  // neg
  if (line[0] == 'n' && line[1] == 'e' && line[2] == 'g')
    fprintf(write_file, "//neg\n"
                        "@SP\n"
                        "A=M-1\n"
                        "M=-M\n");

  // not
  if (line[0] == 'n' && line[1] == 'o' && line[2] == 't')
    fprintf(write_file, "//not\n"
                        "@SP\n"
                        "A=M-1\n"
                        "M=!M\n");

  // eq
  if (line[0] == 'e' && line[1] == 'q')
  {
    // Converte il counter da int a char
    char conv[6];
    sprintf(conv, "%d", counters[0]);

    fprintf(write_file, "//eq\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "D=M-D\n");
    fprintf(write_file, "@TRUEEQ%s\n"
                        "D;JEQ\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=0\n", conv);
    fprintf(write_file, "@ENDEQ%s\n"
                        "0;JMP\n", conv);
    fprintf(write_file, "(TRUEEQ%s)\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=-1\n", conv);
    fprintf(write_file, "(ENDEQ%s)\n", conv);

    counters[0]++;
  }

  // gt
  if (line[0] == 'g' && line[1] == 't')
  {
    // Converte il counter da int a char
    char conv[6];
    sprintf(conv, "%d", counters[1]);

    fprintf(write_file, "//eq\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "D=M-D\n");
    fprintf(write_file, "@TRUEGT%s\n"
                        "D;JGT\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=0\n", conv);
    fprintf(write_file, "@ENDGT%s\n"
                        "0;JMP\n", conv);
    fprintf(write_file, "(TRUEGT%s)\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=-1\n", conv);
    fprintf(write_file, "(ENDGT%s)\n", conv);

    counters[1]++;
  }

  // lt
  if (line[0] == 'l' && line[1] == 't')
  {
    fprintf(write_file, "//lt\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "A=A-1\n"
                        "D=M-D\n");
    fprintf(write_file, "@TRUELT%d\n"
                        "D;JLT\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=0\n", counters[2]);
    fprintf(write_file, "@ENDLT%d\n"
                        "0;JMP\n", counters[2]);
    fprintf(write_file, "(TRUELT%d)\n"
                        "@SP\n"
                        "A=M\n"
                        "A=A-1\n"
                        "M=-1\n", counters[2]);
    fprintf(write_file, "(ENDLT%d)\n", counters[2]);

    counters[2]++;
  }

  //printf("|%s|\n", line);
}

/* Precondizione: viene passata una stringa che termina con .vm
                  che ha almeno 4 caratteri ma non più di 30

   Postcondizione: viene scritto il nome del file.asm in dotasm
*/
int change_extension(char dotvm[], char dotasm[])
{
  int i, a = strlen(dotvm);

  if (a < 4) {printf("ERROR: name too short\n"); return(0);}
  else
  if (a > 30) {printf("ERROR: name too long\n"); return(0);}
  else
  {
      for(i = 0; i < a-2; i++)
          dotasm[i] = dotvm[i];

      dotasm[i] = '\0';

      //printf("%s\n", dotasm);
      strcat(dotasm, "asm");
      return(1);
  }
}

/* Precondizione: passare un array di caratteri
   Postcondizione: ritorna un numero in base al tipo di comando
   -1 -> ignora
   1  -> pop/push
   2  -> flusso (label, goto...)
   3  -> funzioni
*/
int trimline(char rawline[])
{
  int i, ret, lastSpace = 0;   // salva la posizione dell' ultimo spazio trovato

  // Sostituisco \n con \0
  while (rawline[i] != '\n')
    i++;

  rawline[i] = '\0';

  // Tolgo gli eventuali commenti
  for (i = 0; rawline[i] != '\0'; i++)
  // Se nella stringa trovo uno spazio, registro la sua posizione
    if (rawline[i] == ' ' && rawline[i-1] != ' ' && i > 0)
      lastSpace = i;
    else
  // Se trovo un commento tronco la stringa nella posizione
  // dell' ultimo spazio trovato
    if (rawline[i] == '/')
    {
      rawline[lastSpace] = '\0';
      break;
    }

  // Riconosco il tipo di comando
  // 'p' indica pop e Push
  if (rawline[0] == 'p')
    ret = 1;
  else
  // 'f' per function, 'c' per call, 'r' per return
  if (rawline[0] == 'f' || rawline[0] == 'c' || rawline[0] == 'r')
    ret = 3;
  else
  // label, goto e if-goto hanno un valore lastSpace >= 4
  if (lastSpace >= 4)
    ret = 2;
  // tutto il resto è add, sub, or...
  else
    ret = 0;

  return ret;
}
