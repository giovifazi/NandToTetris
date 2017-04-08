#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funzioni.h"

int main()
{
    // Inizializzazione tables per traduzione
    // std_table contiene (R0..R15, THIS, THAT, KBD... etc)
    // dest_table contiene (A, M, D, MD, AMD... etc)
    // comp_table contiene (D+1, 0, M, -1... etc)
    // jump_table contiene (;JMP, ;JGT... etc)
    // usr_table contiene le varibili/etichette definite nel codice
    tab *std_table = NULL,
        *dest_table = NULL,
        *comp_table = NULL,
        *jump_table = NULL,
        *usr_table = NULL;
    
    std_table = add_std_t(std_table);
    dest_table = add_dest_t(dest_table);
    comp_table = add_comp_t(comp_table);
    jump_table = add_jump_t(jump_table);

    // Apertura file
    FILE *infile, *outfile;
    char namef_in[40], namef_out[41];
    int name_correct = 0;
    
    // Nome dei file da aprire
    do {
        printf("Enter the .asm filename (example: sum.asm (max 30 characters)): ");
        scanf("%s", &namef_in[0]);
        name_correct = hack_extension(namef_in, &namef_out[0]);
    } while (name_correct == 0);
    
    //------------------------------------------------------------------------|
    // Prima passata sul codice, registrazione labels e scrittura dei comandi |
    //------------------------------------------------------------------------|
    int line_counter = 0;
    char line[100], *endf, ln_ct_cnv[17], u[100];
    
    u[0] = '@';
    u[1] = '\0';
    
    ln_ct_cnv[16] = '\0';   // Per evitare stranezze
    
    infile = fopen(namef_in, "r");
    outfile = fopen(".tmp", "w");
    
    if (infile == NULL) // nel caso di errori di apertura
    {
        perror("Error while trying to open the file");
        exit(1);
    }
    else                // registrazione labels e scrittura comandi
    {
        while (1)
        {
            endf = fgets(line, 100, infile); // Prendo la riga in lettura

            if (endf == NULL) {break;} // Esce dalla lettura se il file finisce
            
            if (line[0] == '(') 
            // Se viene trovato un label
            {
                trim_label(&line[0]);
                
                // Aggiunta @ all'inizio di line
                strcat(u, line);
                
                // Aggiunta label e line_ct convertito alla usr_table
                dec_to_bin(line_counter, &ln_ct_cnv[0]);
                usr_table = add_table(usr_table, u, ln_ct_cnv);
                
                // Reinizializzazione di u per evitare stranezze
                u[0] = '@';
                u[1] = '\0';
            }
            else
            if (line[0] == ' ') 
            // Se viene trovato uno spazio fai il trim e controlla cosa è
            {
                trim_line(&line[0]);
                
                if (line[0] == '@' || line[0] == '0' || line[0] == 'A' ||
                    line[0] == 'M' || line[0] == 'D')
                { 
                    line_counter++;
                }
                
                fprintf(outfile, "%s\n", line);
            }
            else
            if (line[0] == '@' || line[0] == '0' || line[0] == 'A' ||
                line[0] == 'M' || line[0] == 'D')
            // Se trovo subito un carattere utile
            {
                fprintf(outfile, "%s", line);
                line_counter++;
            } 
        }
    }
    
    fclose(infile);
    fclose(outfile);
    
    //------------------------------------------------------------------------|
    // Seconda passata sul codice, traduzione di tutto                        |
    //------------------------------------------------------------------------|
    outfile = fopen(namef_out, "w");
    infile = fopen(".tmp", "r");
    
    int var_counter = 16, j;        // tiene traccia delle variabili
    char hackline[17];              // riga tradotta
    tab ret;
    // riuso endf che è un char*
    // riuso line che è un char[100]
    
    hackline[16] = '\0';
    
    
    if (infile == NULL)
    {
        perror("Error while trying to open the file, clean the directory");
        exit(1);
    }
    else
    {
        while(1)
        {
            endf = fgets(line, 100, infile);
            
            if (endf == NULL) {break;}
            
            switch(line[0])
            {
                
                case '@':
                    ret = decode_A(&line[0], &hackline[0], &var_counter, usr_table, std_table);
                    
                    //printf("\n|%s|->|%s|\n", ret.def, ret.val);
                    usr_table = add_table(usr_table, ret.val, ret.def);
                    break;
                    
                default: 
                    decode_C(&line[0], &hackline[0], dest_table, comp_table, jump_table);
                    break;
                    
            }
            
            hackline[16] = '\n';
            fprintf(outfile, "%s", hackline);
            hackline[0] = '\0';                     // reinizializza la hackline
        }
    }
    
    //stampa_tab(usr_table);
    
    fclose(infile);
    fclose(outfile);
    remove(".tmp");
    
    
    
    exit(1);
}
