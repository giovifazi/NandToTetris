/** funzioni.c
 *  VM-translator del progetto nand to tetris
 *  versione 2.0
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
                        "@VMGlobal.return\n"
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
    nome[k] = '\0';

    // copio il numero
    i++;
    while(line[i] != '\0')
    {
      nArg[h] = line[i];
      i++;
      h++;
    }
    nArg[h] = '\0';

    // Se la call ha 0 argument da pushare
    if (nArg[0] == '0')
    {
      fprintf(write_file, "//call\n"
                          "@%s\n"              // la posizione di jump a funzione la metto in R14
                          "D=A\n"
                          "@R14\n"
                          "M=D\n"
                          "@R13\n"            // il numero argomenti lo metto in R13
                          "M=0\n"
                          "@RETURN_CALL%d\n"  // la posizione di ritorno la metto in D
                          "D=A\n"
                          "@VMGlobal.call\n"  // chiamo la call globale
                          "0;JMP\n"
                          "(RETURN_CALL%d)\n", nome, counters[3], counters[3]);
    }
    // Se la call ha solo 1 argument da pushare
    else if (nArg[0] == '1' && isdigit(nArg[1]) == 0)
    {
      fprintf(write_file, "//call\n"
                          "@%s\n"              // la posizione di jump a funzione la metto in R14
                          "D=A\n"
                          "@R14\n"
                          "M=D\n"
                          "@R13\n"            // il numero argomenti lo metto in R13
                          "M=1\n"
                          "@RETURN_CALL%d\n"  // la posizione di ritorno la metto in D
                          "D=A\n"
                          "@VMGlobal.call\n"  // chiamo la call globale
                          "0;JMP\n"
                          "(RETURN_CALL%d)\n", nome, counters[3], counters[3]);
    }
    else
    fprintf(write_file, "//call\n"
                        "@%s\n"              // la posizione di jump a funzione la metto in R14
                        "D=A\n"
                        "@R14\n"
                        "M=D\n"
                        "@%s\n"              // il numero argomenti lo metto in R13
                        "D=A\n"
                        "@R13\n"
                        "M=D\n"
                        "@RETURN_CALL%d\n"  // la posizione di ritorno la metto in D
                        "D=A\n"
                        "@VMGlobal.call\n"  // chiamo la call globale
                        "0;JMP\n"
                        "(RETURN_CALL%d)\n", nome, nArg, counters[3], counters[3]);

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

  // Casi POP
  if (line[1] == 'o')
  {
      // Argument
      if (line[4] == 'a' && line[5] == 'r' && line[6] == 'g' && line[7] == 'u')
      {
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

          // Ottimizzazione per caso pop 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//pop argument 0\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@ARG\n"
                                "A=M\n"
                                "M=D\n");
          }
          // Ottimizzazione per caso pop 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//pop argument 1\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@ARG\n"
                                "A=M+1\n"
                                "M=D\n");
          }
          else
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
          return;
      }

      // Local
      if (line[4] == 'l' && line[5] == 'o' && line[6] == 'c' && line[7] == 'a')
      {
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

          // Ottimizzazione per caso pop 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//pop Local\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@LCL\n"
                                "A=M\n"
                                "M=D\n");
          }
          // Ottimizzazione per caso pop 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//pop Local\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@LCL\n"
                                "A=M+1\n"
                                "M=D\n");
          }
          else
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
          return;
        }

        // Static
        if (line[4] == 's' && line[5] == 't' && line[6] == 'a' && line[7] == 't')
        {
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
          strcat(var, ".");
          strcat(var, num);

          fprintf(write_file, "//pop static\n"
                              "@SP\n"
                              "AM=M-1\n"
                              "D=M\n"
                              "@%s\n"
                              "M=D\n", var);
        return;
        }

        // Temp
        if (line[4] == 't' && line[5] == 'e' && line[6] == 'm' && line[7] == 'p')
        {
          char cnum[2];
          int conv;

          // Estrae il numero dal comando
          cnum[0] = line[9];
          cnum[1] = '\0';

          // Converte il numero in intero e aggiunge 5
          conv = atoi(cnum);
          conv += 5;

          fprintf(write_file, "//pop temp\n"
                              "@SP\n"
                              "AM=M-1\n"
                              "D=M\n"
                              "@R%d\n"
                              "M=D\n", conv);

          return;
        }

        // Pointer
        if (line[4] == 'p' && line[5] == 'o' && line[6] == 'i' && line[7] == 'n')
        {
          if (line[12] == '0')
          // pop pointer 0 è per this
            fprintf(write_file, "//pop pointer\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THIS\n"
                                "M=D\n");
          else
          // pop pointer 1 è per that
            fprintf(write_file, "//pop pointer\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THAT\n"
                                "M=D\n");
          return;
        }

        // This
        if (line[4] == 't' && line[5] == 'h' && line[6] == 'i' && line[7] == 's')
        {
          // metto il numero in num
          for(i = 9; line[i] != '\0'; i++)
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

          // Ottimizzazione pop 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//pop this 0\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THIS\n"
                                "A=M\n"
                                "M=D\n");
          }
          // Ottimizzazione pop 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//pop this 1\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THIS\n"
                                "A=M+1\n"
                                "M=D\n");
          }
          else
          fprintf(write_file, "//pop this\n"
                              "@THIS\n"
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
          return;
        }

        // That
        if (line[4] == 't' && line[5] == 'h' && line[6] == 'a' && line[7] == 't')
        {
          // metto il numero in num
          for(i = 9; line[i] != '\0'; i++)
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

          // Ottimizzazione pop 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//pop that\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THAT\n"
                                "A=M\n"
                                "M=D\n");
          }
          // Ottimizzazione pop 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//pop that\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@THAT\n"
                                "A=M+1\n"
                                "M=D\n");
          }
          else
          fprintf(write_file, "//pop that\n"
                              "@THAT\n"
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
          return;
        }
      }
    else
    /////////////////////////////////////////////////////// ^^^^ ////////////////////////////////////////////////
    /////////////////////////////////////////////////////// POP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////// ---- ////////////////////////////////////////////////
    /////////////////////////////////////////////////////// PUSH ////////////////////////////////////////////////
    /////////////////////////////////////////////////////// vvvv ////////////////////////////////////////////////
    {
      // Argument
      if (line[5] == 'a' && line[6] == 'r' && line[7] == 'g' && line[8] == 'u')
      {
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

          // Ottimizzazione per push argument 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//push argument\n"
                                "@ARG\n"
                                "A=M\n"
                                "D=M\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=D\n");
          }
          // Ottimizzazione per push argument 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//push argument\n"
                                "@ARG\n"
                                "A=M+1\n"
                                "D=M\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=D\n");
          }
          else
          // Caso per push argument n dove n > 1
          fprintf(write_file, "//push argument\n"
                              "@ARG\n"
                              "D=M\n"
                              "@%s\n"
                              "A=D+A\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n", num);
        return;
      }

      // Local
      if (line[5] == 'l' && line[6] == 'o' && line[7] == 'c' && line[8] == 'a')
      {
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

          // Ottimizzazione per push local 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//push local\n"
                                "@LCL\n"
                                "A=M\n"
                                "D=M\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=D\n");
          }
          // Ottimizzazione per push local 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//push local\n"
                                "@LCL\n"
                                "D=M\n"
                                "A=D+1\n"
                                "D=M\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=D\n");
          }
          else
          // Caso per push local n dove n > 1
          fprintf(write_file, "//push local\n"
                              "@LCL\n"
                              "D=M\n"
                              "@%s\n"
                              "A=D+A\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n", num);
        return;
      }

      // Constant
      if (line[5] == 'c' && line[6] == 'o' && line[7] == 'n' && line[8] == 's')
      {
          // metto il numero in num
          for(i = 14; line[i] != '\0'; i++)   //isspace(line[i]) == 0
          {
            if (isspace(line[i]) != 0) // line[i] == ' '
            {
              num[n] = '\0';
              break;
            }

            num[n] = line[i];
            n++;
          }

          num[n] = '\0';

          // Ottimizzazione per push constant 0
          if (num[0] == '0')
          {
            fprintf(write_file, "//push constant 0\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=0\n");
          }
          // Ottimizzazione per push constant 1
          else if (num[0] == '1' && isdigit(num[1]) == 0)
          {
            fprintf(write_file, "//push constant 1\n"
                                "@SP\n"
                                "M=M+1\n"
                                "A=M-1\n"
                                "M=1\n");
          }
          else
          // push constant x dove x > 1
          fprintf(write_file, "//push constant\n"
                              "@%s\n"
                              "D=A\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n", num);
        return;
      }

      // Static
      if (line[5] == 's' && line[6] == 't' && line[7] == 'a' && line[8] == 't')
      {
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
          strcat(var, ".");
          strcat(var, num);

          fprintf(write_file, "//push static\n"
                              "@%s\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n", var);
        return;
      }

      // Temp
      if (line[5] == 't' && line[6] == 'e' && line[7] == 'm' && line[8] == 'p')
      {
        char cnum[2];
        int conv;

        // Estrae il numero dal comando
        cnum[0] = line[10];
        cnum[1] = '\0';

        // Converte il numero in intero e aggiunge 5
        conv = atoi(cnum);
        conv += 5;

        fprintf(write_file, "//push temp\n"
                            "@R%d\n"
                            "D=M\n"
                            "@SP\n"
                            "M=M+1\n"
                            "A=M-1\n"
                            "M=D\n", conv);

        return;
      }

      // Pointer
      if (line[5] == 'p' && line[6] == 'o' && line[7] == 'i' && line[8] == 'n')
      {
        if (line[13] == '0')
          fprintf(write_file, "//push pointer 0\n"
                              "@THIS\n");
        else
          fprintf(write_file, "//push pointer 1\n"
                              "@THAT\n");

        fprintf(write_file, "D=M\n"
                            "@SP\n"
                            "M=M+1\n"
                            "A=M-1\n"
                            "M=D\n");
        return;
      }

      // This
      if (line[5] == 't' && line[6] == 'h' && line[7] == 'i' && line[8] == 's')
      {
        // estraggo il numero dal comando
        char num[6];
        int i = 10, w = 0;

        while (isdigit(line[i])) {
          num[w] = line[i];
          w++;
          i++;
        }

        num[w] = '\0';

        // Ottimizzazione per push this 0
        if (num[0] == '0')
        {
          fprintf(write_file, "//push this 0\n"
                              "@THIS\n"
                              "A=M\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n");
        }
        // Ottimizzazione per push this 1
        else if (num[0] == '1' && isdigit(num[1]) == 0)
        {
          fprintf(write_file, "//push this 1\n"
                              "@THIS\n"
                              "A=M+1\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n");
        }
        else
        fprintf(write_file, "//push this\n"
                            "@THIS\n"
                            "D=M\n"
                            "@%s\n"
                            "A=D+A\n"
                            "D=M\n"
                            "@SP\n"
                            "M=M+1\n"
                            "A=M-1\n"
                            "M=D\n", num);
        return;
      }

      // That
      if (line[5] == 't' && line[6] == 'h' && line[7] == 'a' && line[8] == 't')
      {
        // estraggo il numero dal comando
        char num[6];
        int i = 10, w = 0;

        while (isdigit(line[i])) {
          num[w] = line[i];
          w++;
          i++;
        }

        num[w] = '\0';

        // Ottimizzazione push that 0
        if (num[0] == '0')
        {
          fprintf(write_file, "//push that 0\n"
                              "@THAT\n"
                              "A=M\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n");
        }
        // Ottimizzazione push that 1
        else if (num[0] == '1' && isdigit(num[1]) == 0)
        {
          fprintf(write_file, "//push that 1\n"
                              "@THAT\n"
                              "A=M+1\n"
                              "D=M\n"
                              "@SP\n"
                              "M=M+1\n"
                              "A=M-1\n"
                              "M=D\n");
        }
        else
        fprintf(write_file, "//push that\n"
                            "@THAT\n"
                            "D=M\n"
                            "@%s\n"
                            "A=D+A\n"
                            "D=M\n"
                            "@SP\n"
                            "M=M+1\n"
                            "A=M-1\n"
                            "M=D\n", num);
        return;
      }
    }

  //printf("|%s|\n", line);
}

void translateFlow(char line[], FILE *write_file, char funcname[])
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

  strcat(c, "$");
  strcat(c, funcname);

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
                        "@RETURN_EQ%s\n"
                        "D=A\n"
                        "@R14\n"
                        "M=D\n"
                        "@VMGlobal.eq\n"
                        "0;JMP\n"
                        "(RETURN_EQ%s)\n", conv, conv);

    counters[0]++;
  }

  // gt
  if (line[0] == 'g' && line[1] == 't')
  {
    // Converte il counter da int a char
    char conv[6];
    sprintf(conv, "%d", counters[1]);

    fprintf(write_file, "//gt\n"
                        "@RETURN_GT%s\n"
                        "D=A\n"
                        "@R14\n"
                        "M=D\n"
                        "@VMGlobal.gt\n"
                        "0;JMP\n"
                        "(RETURN_GT%s)\n", conv, conv);

    counters[1]++;
  }

  // lt
  if (line[0] == 'l' && line[1] == 't')
  {
    fprintf(write_file, "//lt\n"
                        "@RETURN_LT%d\n"
                        "D=A\n"
                        "@R14\n"
                        "M=D\n"
                        "@VMGlobal.lt\n"
                        "0;JMP\n"
                        "(RETURN_LT%d)\n", counters[2], counters[2]);

    counters[2]++;
  }

  //printf("|%s|\n", line);
}

/* Precondizione: viene passata una stringa che termina con .vm
                  che ha almeno 4 caratteri ma non più di 30

   Postcondizione: viene scritto il nome del file.asm in dotasm
*/
void change_extension(char dotvm[], char dotasm[])
{
  int i, a = strlen(dotvm);

  if (a < 4) {printf("ERROR: name too short\n");}
  else
  if (a > 30) {printf("ERROR: name too long\n");}
  else
  {
      for(i = 0; i < a-2; i++)
          dotasm[i] = dotvm[i];

      dotasm[i] = '\0';

      //printf("%s\n", dotasm);
      strcat(dotasm, "asm");
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
