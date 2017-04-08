typedef struct table {
    char def[100];
    char val[16];
    struct table *next;
} tab;

tab* add_table(tab *head, char d[], char v[]);

void stampa_tab(tab *head);

tab* add_std_t(tab *head);

tab* add_dest_t(tab *head);

tab* add_comp_t(tab *head);

tab* add_jump_t(tab *head);

int hack_extension(char dotasm[], char dothack[]);

void trim_label(char lab[]);

void trim_line(char line[]); 

void dec_to_bin(int dec, char bin[]);

tab decode_A(char line[], char hack[], int *c, tab *usr, tab *std);

int find_in_table(tab *head, char val[], char write[]);

void decode_C(char line[], char hack[], tab *destt, tab *compt, tab *jumpt);
