#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>

int main(void);
void start_level(int *level);

void vider_buffer(void) {
    int c;
    while((c=getchar()) != EOF && c != '\n');
}

void getColor(int t,int f) {
    /*
     0 : noir      1 : bleu foncé    2 : vert           3 : bleu-gris
     4 : marron    5 : pourpre       6 : kaki           7 : gris clair
     8 : gris      9 : bleu         10 : vert fluo     11 : turquoise
    12 : rouge    13 : rose fluo    14 : jaune fluo    15 : blanc
     */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}
/** creer une structure de type case */
typedef struct {
    int x; // position x
    int y; // position y
    int state; // état de la case: -1 = vide, 0 = chaine, 1 = occupe
    int value; // valeur a afficher
    int color; // couleur
    int scope; // cible: 0 = false, 1 = true
}BOX;

/** affecter les données a une case choisi*/
BOX defineBox(int x, int y, int state, int value, int color){
    BOX myBox;
    myBox.x = x;
    myBox.y = y;
    myBox.state = state;
    myBox.value = value;
    myBox.color = color;
    myBox.scope = 0;
    return myBox;
}

BOX *build_tab_box(int row, int column, const int *tab_int) {
    int list_color[5] = {1, 2, 10, 12, 14};
    int tab_increment = 0, list_color_increment = 0, color, state;
    BOX *tab_case = malloc (row * column * sizeof (BOX));
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < column; j++) {
            if(*(tab_int + (i * column) + j) == 0) {
                state = 0;
                color = list_color[list_color_increment];
                list_color_increment += 1;
            } else {
                state = -1;
                color = 15;
            }
            tab_case[tab_increment] = defineBox(j, i, state, *(tab_int + (i * column) + j), color);
            tab_increment += 1;
        }
    }
    return tab_case;
}

void move_droite(int column, BOX **list_chaine, int list_chaine_size) {
    BOX *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos+1)->value >= pos->value && pos->x < column-1 && (pos+1)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos+1);
            }
        }
        pos->scope = 0;
        (pos+1)->scope = 1;
        (pos+1)->color = pos->color;
        (pos+1)->state = 1;
    }
}

void move_gauche(BOX **list_chaine, int list_chaine_size) {
    BOX *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos-1)->value >= pos->value && pos->x > 0 && (pos-1)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos-1);
            }
        }
        pos->scope = 0;
        (pos-1)->scope = 1;
        (pos-1)->color = pos->color;
        (pos-1)->state = 1;
    }
}

void move_bas(int row, int column, BOX **list_chaine, int list_chaine_size) {
    BOX *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos+column)->value >= pos->value && pos->y < row-1 && (pos+column)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos+column);
            }
        }
        pos->scope = 0;
        (pos+column)->scope = 1;
        (pos+column)->color = pos->color;
        (pos+column)->state = 1;
    }
}

void move_haut(int column, BOX **list_chaine, int list_chaine_size) {
    BOX *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos-column)->value >= pos->value && pos->y > 0 && (pos-column)->state != 1){
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos-column);
            }
        }
        pos->scope = 0;
        (pos-column)->scope = 1;
        (pos-column)->color = pos->color;
        (pos-column)->state = 1;
    }
}

int choose_chain(BOX **list_chaine, int list_chaine_size) {
    int p;
    bool change = false;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1) {
            p = i;
            list_chaine[i]->scope = 0;
        }
    }
    for(int i = p+1; i < list_chaine_size && change == false; i++) {
        if(list_chaine[i]->scope == 0) {
            list_chaine[i]->scope = 1;
            change = true;
        }
    }
    if(change == false){
        for(int i = 0; i < list_chaine_size && change == false; i++) {
            if(list_chaine[i]->scope == 0) {
                list_chaine[i]->scope = 1;
                change = true;
            }
        }
    }
}

/** afficher le tableau */

void display(BOX *tab_case, int row, int column) {
    /** faire une pause*/
	system("cls");
    int count = 0, bg_color, fg_color;
    printf("\n");
    for (int i = 0; i < row; i++) {
        printf("     ");
        for (int j = 0; j < column; j++) {
            printf("  ");
            if((tab_case + count)->scope == 1){
                bg_color = 15;
                fg_color = 0;
            }else{
                bg_color = 0;
                fg_color = (tab_case + count)->color;
            }
            getColor(fg_color, bg_color);
            if((tab_case + count)->state == 0){
                printf("x");
            }else{
                printf("%d", (tab_case + count)->value);
            }
            getColor(15, 0);
            count += 1;
        }
        printf("\n");
    }
    printf("\n");
}
/** vérifier si partie fini */
int verifier_partie(BOX *tab_case, int row, int column) {
    for(int i = 0; i < row*column; i++) {
        if((tab_case+i)->state == -1) {
            return 0;
        }
    }
    return 1;
}

void del_chain(BOX *tab_case, int row, int column, BOX **list_chaine, int list_chaine_size) {
    BOX **save_pointeur;
    int color;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1) {
            save_pointeur = &list_chaine[i];
            list_chaine[i]->scope = 0;
            color = list_chaine[i]->color;
        }
    }
    for(int i = 0; i < row*column; i++) {
        if(color == (tab_case+i)->color && (tab_case+i)->state != 0) {
            (tab_case+i)->state = -1;
            (tab_case+i)->color = 15;
        }else if(color == (tab_case+i)->color && (tab_case+i)->state == 0) {
            (tab_case+i)->scope = 1;
            *save_pointeur = (tab_case+i);
        }
    }
}
/*handle user input and perform actions accordingly.*/

 void turn(BOX *tab_case, int row, int column, BOX **list_chaine, int list_chaine_size, int win, int scope_size, FILE *fichier, int *level_actuel) {
    char action;
    display(tab_case, row, column);
    printf("     Actions :\n\n     - (C) Choose a case\n     - (R) Delete a case\n     - (A) Restart a level\n     - (Z) Up\n     - (S) Down\n     - (Q) Left\n     - (D) Right\n     - (P) Choose a level\n\n     >>>");
    scanf("%s", &action);
    vider_buffer();
	/** choix d'action */
	switch ( action ){
		case 'C': choose_chain(list_chaine, list_chaine_size);  break;
		case 'c': choose_chain(list_chaine, list_chaine_size);  break;
		case 'Z': move_haut(column, list_chaine, list_chaine_size); break;
		case 'z': move_haut(column, list_chaine, list_chaine_size);  break;
		case 'S': move_bas(row, column, list_chaine, list_chaine_size);break;
		case 's': move_bas(row, column, list_chaine, list_chaine_size); break;
		case 'Q': move_gauche(list_chaine, list_chaine_size); break;
		case 'q': move_gauche(list_chaine, list_chaine_size);  break;
		case 'D': move_droite(column, list_chaine, list_chaine_size); break;
		case 'd': move_droite(column, list_chaine, list_chaine_size);break;
		case 'R': del_chain(tab_case, row, column, list_chaine, list_chaine_size); break;
		case 'r': del_chain(tab_case, row, column, list_chaine, list_chaine_size);break;
		case 'P': system("cls"); main(); break;
		case 'p': system("cls");  main();break;
		case 'A': restart_level(scope_size, tab_case, row, column, list_chaine, list_chaine_size); break;
		case 'a': restart_level(scope_size, tab_case, row, column, list_chaine, list_chaine_size);break;
		default :  turn(tab_case, row, column, list_chaine, list_chaine_size, win, scope_size, fichier, level_actuel);break;
	}


    win = verifier_partie(tab_case, row, column);
    char restart;
    if(win == 1) {
        display(tab_case, row, column);
        printf("     Niveau termine !\n\n     - (P) Choix du niveau\n     - (S) Niveau suivant\n     - N'importe quelle autre touche pour fermer le jeu\n\n     >>>");
        scanf("%s", &restart);
        vider_buffer();
        if(restart == 'P' || restart == 'p') {
            system("cls");
            free(list_chaine);
            free(tab_case);
            main();
        }else if(restart == 'S' || restart == 's'){
            system("cls");
            free(list_chaine);
            free(tab_case);
            *level_actuel = *level_actuel + 1;
            if(*level_actuel == 31) {
                *level_actuel = 1;
            }
            start_level(level_actuel);
        } else {
            exit(EXIT_SUCCESS);
        }
    }
    if(win == 0) {
        turn(tab_case, row, column, list_chaine, list_chaine_size, win, scope_size, fichier, level_actuel);
    }
}

/* Restart the level */
void restart_level(int scope_size, BOX *tab_case, int row, int column, BOX **list_chaine, int list_chaine_size) {
    for(int i = 0; i < scope_size; i++) {
        del_chain(tab_case, row, column, list_chaine, list_chaine_size);
        choose_chain(list_chaine, list_chaine_size);
    }
}

/** commencer la partie myLevel*/
void start_level(int *level_actuel) {

    // Récupération du niveau dans un fichier
    FILE *fichier = NULL;
    fichier = fopen("level.txt", "r");
    char chaine[1000];
    for(int i = 0; i < *level_actuel; i++) {
        fgets(chaine, 1000, fichier);
    }
    fclose(fichier);

    // Création d'un tableau avec les valeurs du niveau
    char *end;
    int row = (int) strtol(&chaine[0], &end, 10), column = (int) strtol(&chaine[2], &end, 10), count_chaine = 4;
    int *plateau = malloc (row*column * sizeof(int));
    for(int i = 0; i < row*column; i++) {
        plateau[i] = (int) strtol(&chaine[count_chaine], &end, 10);
        count_chaine += 2;
    }

    // Création du plateau de jeu
    BOX *tab_case;
    tab_case = build_tab_box(row, column, &plateau[0]);

    // Création d'un tableau contenant des pointeurs vers les chaines du niveau
    int count = 0;
    for(int i = 0; i < row*column; i++) {
        if(plateau[i] == 0) {
            count += 1;
        }
    }
    BOX **list_chaine = malloc (count * sizeof(BOX));
    int count_board = 0;
    for(int i = 0; i < row*column; i++) {
        if((tab_case + i)->state == 0) {
            list_chaine[count_board] = (tab_case+i);
            count_board += 1;
        }
    }

    // Cible la première chaine
    list_chaine[0]->scope = 1;

    // Lancement du niveau
    turn(tab_case, row, column, list_chaine, count, 0, count, fichier, level_actuel);

    // Libération de la mémoire
    free(list_chaine);
    free(plateau);
}

int main(void) {
    int level = 1;
    int *level_actuel = &level;
    bool start = false;
    char action;
    while(start == false) {
        printf("     Current level: \"%d\"\n\n     - (Z) Next level\n     - (S) Previous level\n     - (E) Start\n     - (F) Close\n\n     >>>", level);
        scanf("%c", &action);
        vider_buffer();
        if(action == 'S' || action == 's') {
            if(level > 1) {
                level -= 1;
            }
        }else if(action == 'Z' || action == 'z') {
            if(level < 30) {
                level += 1;
            }
        }else if(action == 'E' || action == 'e') {
            start = true;
        }else if(action == 'F' || action == 'f') {
            exit(EXIT_SUCCESS);
        }
        system("cls");
    }
    start_level(level_actuel);
}
