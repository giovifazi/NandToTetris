#include "funzioni.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

tab* add_table(tab *head, char d[], char v[])
{
    tab *newh = malloc(sizeof(struct table));
    
    //printf("\naggiungo |%s| e |%s|\n", d, v);
    strcpy(newh->def, d);
    strcpy(newh->val, v);
    newh->next = head;
    
    return(newh);
}

void stampa_tab(tab *head)
{
    while(head != NULL)
    {
        printf("%s -> %s\n", head->def, head->val);
        head = head->next;
    }
}

tab* add_std_t(tab *head)
{
    head = add_table(head, "@SP",     "0000000000000000");
    head = add_table(head, "@LCL",    "0000000000000001");
    head = add_table(head, "@ARG",    "0000000000000010");
    head = add_table(head, "@THIS",   "0000000000000011");
    head = add_table(head, "@THAT",   "0000000000000100");
    head = add_table(head, "@SCREEN", "0010000000000000");
    head = add_table(head, "@KBD",    "0110000000000000");
    head = add_table(head, "@R0",     "0000000000000000");
    head = add_table(head, "@R1",     "0000000000000001");
    head = add_table(head, "@R2",     "0000000000000010");
    head = add_table(head, "@R3",     "0000000000000011");
    head = add_table(head, "@R4",     "0000000000000100");
    head = add_table(head, "@R5",     "0000000000000101");
    head = add_table(head, "@R6",     "0000000000000110");
    head = add_table(head, "@R7",     "0000000000000111");
    head = add_table(head, "@R8",     "0000000000001000");
    head = add_table(head, "@R9",     "0000000000001001");
    head = add_table(head, "@R10",    "0000000000001010");
    head = add_table(head, "@R11",    "0000000000001011");
    head = add_table(head, "@R12",    "0000000000001100");
    head = add_table(head, "@R13",    "0000000000001101");
    head = add_table(head, "@R14",    "0000000000001110");
    head = add_table(head, "@R15",    "0000000000001111");
  return head;
}

tab* add_dest_t(tab *head)
{
    head = add_table(head, "null", "000");
    head = add_table(head, "M",    "001");
    head = add_table(head, "D",    "010");
    head = add_table(head, "MD",   "011");
    head = add_table(head, "A",    "100");
    head = add_table(head, "AM",   "101");
    head = add_table(head, "AD",   "110");
    head = add_table(head, "AMD",  "111");
  return head;
}

tab* add_comp_t(tab *head)
{
    head = add_table(head, "0",    "0101010");
    head = add_table(head, "1",    "0111111");
    head = add_table(head, "-1",   "0111010");
    head = add_table(head, "D",    "0001100");
    head = add_table(head, "A",    "0110000");
    head = add_table(head, "!D",   "0001101");
    head = add_table(head, "!A",   "0110001");
    head = add_table(head, "-D",   "0001111");
    head = add_table(head, "-A",   "0110011");
    head = add_table(head, "D+1",  "0011111");
    head = add_table(head, "A+1",  "0110111");
    head = add_table(head, "D-1",  "0001110");
    head = add_table(head, "A-1",  "0110010");
    head = add_table(head, "D+A",  "0000010");
    head = add_table(head, "D-A",  "0010011");
    head = add_table(head, "A-D",  "0000111");
    head = add_table(head, "D&A",  "0000000");
    head = add_table(head, "D|A",  "0010101");
    head = add_table(head, "M",    "1110000");
    head = add_table(head, "!M",   "1110001");
    head = add_table(head, "-M",   "1110011");
    head = add_table(head, "M+1",  "1110111");
    head = add_table(head, "M-1",  "1110010");
    head = add_table(head, "D+M",  "1000010");
    head = add_table(head, "D-M",  "1010011");
    head = add_table(head, "M-D",  "1000111");
    head = add_table(head, "D&M",  "1000000");
    head = add_table(head, "D|M",  "1010101");
  return head;
}

tab* add_jump_t(tab *head)
{
    head = add_table(head, "null", "000");
    head = add_table(head, "JGT",  "001");
    head = add_table(head, "JEQ",  "010");
    head = add_table(head, "JGE",  "011");
    head = add_table(head, "JLT",  "100");
    head = add_table(head, "JNE",  "101");
    head = add_table(head, "JLE",  "110");
    head = add_table(head, "JMP",  "111");
  return head;
}

int hack_extension(char dotasm[], char dothack[])
{
    int i, a = strlen(dotasm);
    
    if (a < 5) {printf("ERROR: name too short\n"); return(0);}
    else
    if (a > 40) {printf("ERROR: name too long\n"); return(0);}
    else
    if (a >= 5)
    {
        for(i = 0; i < a-3; i++)
        {
            dothack[i] = dotasm[i];
        }
        dothack[i] = '\0';
        
        strcat(dothack, "hack");
        return(1);
    }
}

void trim_label(char lab[])
{
    int a = 0,i;
    
    for(i = 1; lab[i] != ')'; i++)
    {
        lab[a] = lab[i];
        a++;
    }
    
    lab[a] = '\0';
}

void trim_line(char line[])
{
    int a = 0, i;

    // Sposto a fino al primo carattere utile
    while (1)
    {
        if (line[a] != '@' && line[a] != 'A' && line[a] != 'M' &&
            line[a] != 'D' && line[a] != '0' )
        {
            a++;
        }
        else
            break;
    }
    
    // Inizio a copiare 
    for(i = 0; line[a] != '\n' && line[a] != ' ' && line[a] != '/'; i++)
    {
        line[i] = line[a];
        a++;
    }
    
    line[i] = '\0';
}

void dec_to_bin(int dec, char bin[])
{
    int c, k, s = 0;
    
    for (c = 15; c >= 0; c--)
    {
        k = dec >> c;

        if (k & 1)
          bin[s] = '1';
        else
          bin[s] = '0';
        
        s++;
    }
    
    bin[16] = '\0';
    //printf("\nHo convertito |%d| in |%s|", dec, bin);
}

int find_in_table(tab *head, char val[], char write[])
{
    //printf("\nla funzione cerca inizia!\n");
    int i, found;
    
    while(head != NULL)
    {
        //printf("\ncontroolo |%s| con |%s|\n", head->def, val);
        for (i = 0;; i++)
        {
            if (val[i] != head->def[i])
            {
                found = 0;
                break;
            }
            else
            if (val[i] == head->def[i] && val[i] == '\0')
            {
                //printf("trovato! diventera %s\n", head->val);
                found = 1;
                
                strcpy(write, head->val);
                return(1);
            }
        }
        
        if (found == 0)
            head = head->next;
        else
            break;       
    }
    
    // nel caso non trova niente
    //printf("\nnon ho trovato un cazzo\n");
    return 0;
}

tab decode_A(char line[], char hack[], int *c, tab *usr, tab *std)
{
    int j,i = 0,k, conv, found, inc;
    tab ret;
    
    ret.def[0] = '\0';
    ret.val[0] = '\0';
    ret.next = NULL;
    
    // Tolgo il \n dalla line
    for(j = 0; line[j] != '\n'; j++){}
    line[--j] = '\0';
    
    //printf("Ho la linea |%s|\n", line);
    
    if (line[1] != '0' && line[1] != '1' && line[1] != '2' &&
        line[1] != '3' && line[1] != '4' && line[1] != '5' &&
        line[1] != '6' && line[1] != '7' && line[1] != '8' && 
        line[1] != '9')
        // Nel caso non sia un valore numerico la a instruction
    {
        // controlla prima se è una roba standard
        found = find_in_table(std, &line[0], &hack[0]);
        if (found == 1) 
        {
            //printf("trovato %s e %s nella std", line, hack);
            return(ret);
        }
        
        // poi controlla se è una variabile o etichetta
        found = find_in_table(usr, &line[0], &hack[0]);
        
        if (found == 1) 
        {
            //printf("ho trovato |%s| con |%s| nell usr", line, hack);
            return(ret);
        }
        else
        {
            // Traduco la nuova variabile
            dec_to_bin(*c, &hack[0]);
            
            // incremento var counter
            inc = *c;
            inc++;
            *c = inc;
            
            strcpy(ret.def, hack);
            strcpy(ret.val, line);
            
            hack[16] = '\0';
            //printf("\n|%s| |%s| |%d|\n", hack, line, *c);
            return ret;
        }
        
    }
    else
        // Nel caso sia un valore numerico
    {
        // copia il valore temporaneamente dentro hack
        for (k = 1; line[k] != '\0'; k++)
        {
            hack[i] = line[k];
            i++;
        }
        hack[i] = '\0';
        
        //printf("la linea contiene il numero %s che adesso converto\n", hack);
        
        // tramite un cast salva il valore una variabile d'appoggio
        conv = atoi(hack);
        
        // poi fai la conversione dec->bin scrivendo nella hack
        dec_to_bin(conv, &hack[0]);
        //printf("ho convertito %d in %s\n", conv, hack);
    }
}

void decode_C(char line[], char hack[], tab *destt, tab *compt, tab *jumpt)
{
    char dest[5] = "null", comp[4], jump[5] = "null";
    
    dest[4] = '\0';
    comp[3] = '\0';
    jump[4] = '\0';
    
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, j = 0;
    
    // Tolgo il \n dalla line
    j = 0;
    while(1)
    {
        if (line[j] != '0' && line[j] != '1' && line[j] != 'D' && line[j] != 'A' &&
            line[j] != '!' && line[j] != '-' && line[j] != '+' && line[j] != '&' &&
            line[j] != '|' && line[j] != 'M' && line[j] != 'J' && line[j] != 'G' &&
            line[j] != 'T' && line[j] != 'E' && line[j] != 'Q' && line[j] != 'L' &&
            line[j] != 'N' && line[j] != 'P' && line[j] != '=' && line[j] != ';')
        {
            line[j] = '\0';
            break;
        }
        else
        {
            j++;
        }
    }
    
    //printf("Ho la linea |%s|\n", line);
    
    for(a = 0; line[a] != '\0'; a++)
    // Scorro tutta la riga
    {
        if (line[a] == '=')
        {
            strncpy(dest, line, a);
            dest[a] = '\0';
            //printf("ho copiato in dest|%s|\n", dest);
            
            c = a+1;
            while(line[c] != '\0')
            {
                comp[b] = line[c];
                b++;
                c++;
            }
            comp[b] = '\0';
            //printf("ho copiato in comp|%s|\n", comp);
        }
        else
        if (line[a] == ';')
        {
            strncpy(comp, line, a);
            comp[a] = '\0';
            //printf("ho copiato in comp|%s|\n", comp);
            
            d = a+1;
            while(line[d] != '\0')
            {
                jump[e] = line[d];
                e++;
                d++;
            }
            jump[e] = '\0';
            //printf("ho copiato in jump|%s|\n", jump);
        }
    }
    
    //printf("|%s|%s|%s|\n", dest, comp, jump);
    
    int x;
    char tmp[16];
    tmp[15] = '\0';
    
    hack[0] = '\0';
    hack[0] = '1';
    hack[1] = '1';
    hack[2] = '1';
    hack[3] = '\0';
    
    x = find_in_table(compt, comp, tmp);
    //printf("|%s| è diventato |%s|\n", comp, tmp);
    strcat(hack, tmp);
    tmp[0] = '\0';
    
    x = find_in_table(destt, dest, tmp);
    //printf("|%s| è diventato |%s|\n", dest, tmp);
    strcat(hack, tmp);
    tmp[0] = '\0';
    
    x = find_in_table(jumpt, jump, tmp);
    //printf("|%s| è diventato |%s|\n", jump, tmp);
    strcat(hack, tmp);
    tmp[0] = '\0';
    
    //printf("%s has become %s\n", line, hack);
}
