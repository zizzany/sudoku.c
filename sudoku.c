#include <stdio.h>
#include <string.h>
/* dimension du cote des petits carres */
#define MIN 3
/* dimension du cote du grand carre */
#define MAX 9
/* code de sortie des fonctions */
#define FAUX 0
#define VRAI 1

char grille[MAX][MAX]; /* la grille */
long int g_comp_tests; /* compteur de tests */
int g_flag_affiche; /* Drapeau d'option d'affichage de l'option -a */
/********/
int main(int argc, char *argv[]);
int parametre_affichage(char t[]);
/**** SAISIES ET VERIFICATIONS ****/
int saisie_grille(void);
int saisie_une_ligne(int ligne);
int verif_saisie_grille(void);
int verif_saisie_lignes(void);
int verif_saisie_une_ligne(int ligne);
int verif_saisie_colonnes(void);
int verif_saisie_une_colonne(int col);
int verif_saisie_carres(void);
int verif_saisie_un_carre(int carre);
void trans_carre(char tamp[], int carre);
/**** RESOLUTION ****/
void resolve(void);
int case_dispo_nbre(int nbre, int ligne, int col);
void trans_carre_de_case(char tamp[], int ligne, int col);
/**** DIVERS ****/
void affiche_grille_attente(void);
void affiche_grille(void);
void affiche_chaine(int nbre);
int grille_finie(void);
void attente(void);

/******/
int main(int argc, char *argv[])
/******/
{
.   /* option d'affichage dans la fontion resolve */
.   g_flag_affiche = ((argc> = 2) && (parametre_affichage(argv[1])));
.   /**********/
.   printf("\n******* SUDOKU *******\n\n");
.   printf("l'option sudoku -a affiche la resolution case par case.\n\n");
.   printf("Saisissez %d lignes de %d chiffres,\n", MAX, MAX);
.   printf("les chiffres peuvent etre separes par n'importe quels caracteres\n");
.   printf("Rentrez un 0 pour chaque case vide.\n");
.   printf("Pour quitter le programme, valider la lettre Q.\n\n");
.   while (saisie_grille()) {
.   .   g_comp_tests = 0; 
.   .   resolve();
.   .   affiche_grille();
.   .   if (! grille_finie()) printf("Enonce errone...\n");
.   .   printf("Tests : %ld\n\n", g_comp_tests);
.   }/* end while */
.   printf("\n");
.   return 0;
}

/**********************/
int parametre_affichage(char t[])
/**********************/
{
.   /* Renvoi VRAI si l'utilisateur a taper une demande d'option d'affichage
.   sous la forme : a, A, ou /a , /A , -a , -A ect... sinon renvoi FAUX */
.   char param;
.   if (strlen(t) == 1) param = t[0];
.   else if (strlen(t) == 2) param = t[1];
.   else param = 0;
.   return ((param == 'A') || (param == 'a'));
}/* end parametre_affichage */

/**********************************/
/**** SAISIES ET VERIFICATIONS ****/
/**********************************/

/***************/
int saisie_grille(void)
/***************/
{/* l'utilisateur saisie 9 lignes de 9 chiffres */
.   int ligne;
.   printf("Saisissez votre grille : \n");
.   while (VRAI){
.   .   for (ligne = 0; ligne < MAX; ligne++){
.   .   .   if (! saisie_une_ligne(ligne)) return (FAUX);
.   .   }/* end for */
.   .   printf("\nEnonce : \n");
.   .   affiche_grille();
.   .   if (verif_saisie_grille()) return(VRAI);
.   .   else printf("\n");
.   }/* end while */
}/* end saisie */

/******************/
int saisie_une_ligne(int ligne)
/******************/
{
/* l'utilisateur doit saisir une ligne de 0 a 9 chiffres
separes ou non par un ou plusieurs carateres.
Seuls les 9 premiers chiffres seront retenus.
Le + pratique : pave numerique et mettre un point
tous les 3 chiffres 
Eviter d'utiliser gets() a cause des debordements.
Penser a vider entierement le buffer */
.   int c, i;
.   int col = 0;
.   int result = VRAI;
.   /* RAZ des cases de la ligne */
.   for (i = 0; i < MAX; i++) {
.   .   grille[ligne][i] = 0;
.   }/* for */
.   /*******/
.   printf("Ligne %d : ", ligne+1);
.   while(VRAI){
.   .   c = fgetc(stdin);
.   .   if (c == feof(stdin) || c == 0x0A) return result;
.   .   else if ((c == 'Q') || (c == 'q')) result = FAUX;
.   .   /* transfert de la saisie dans les cases de la ligne */
.   .   else if ( (result) && (col < MAX) && (c >= '0') && (c <= '9')) {
.   .   .   grille[ligne][col++] = c-48;
.   .   }/*end if */
.   }/* end while */
}/*end saisie_une_ligne */

/*********************/
int verif_saisie_grille(void)
/*********************/
{/* verifie si pas de doublon dans l'ensemble de la grille */
.   int result = VRAI;
.   if (! verif_saisie_lignes()) result = FAUX;
.   if (! verif_saisie_colonnes()) result = FAUX;
.   if (! verif_saisie_carres()) result = FAUX;
.   return result;
}/* end verif_saisie_grille */

/*********************/
int verif_saisie_lignes(void)
/*********************/
{/* verifie si pas de doublon dans les lignes */
.   int ligne;
.   int result = VRAI;
.   for (ligne = 0; ligne < MAX; ligne++) {
.   .   if (! verif_saisie_une_ligne(ligne)) result = FAUX;
.   }/* end for */
.   return result;
}/* end verif_saisie_lignes */

/************************/
int verif_saisie_une_ligne(int ligne)
/************************/
{/* verifie si pas de doublon dans la ligne */
.   int i,j;
.   for (i = 0; i < MAX-1; i++) {
.   .   if ( grille[ligne][i] == 0 ) continue;
.   .   for (j = i+1; j < MAX; j++) {
.   .   .   if (grille[ligne][i] == grille[ligne][j]) {
.   .   .   .   printf("Erreur ligne %d : chiffres egaux\n", ligne+1);
.   .   .   .   return FAUX;
.   .   .   }/* end if */
.   .   }/* end for j */
.   }/* end for i */
.   return VRAI;
}/* end verif_saisie_une_ligne */

/***********************/
int verif_saisie_colonnes(void)
/***********************/
{/* verifie si pas de doublon dans les colonnes */
.   int col;
.   int result = VRAI;
.   for (col = 0; col < MAX; col++) {
.   .   if (! verif_saisie_une_colonne(col)) result = FAUX;
.   }/* end for */
.   return result;
}/* end verif_saisie_colonnes */

/**************************/
int verif_saisie_une_colonne(int col)
/**************************/
{/* verifie si pas de doublon dans la colonne */
.   int i, j;
.   for (i = 0; i < MAX-1; i++) {
.   .   if ( grille[i][col] == 0 ) continue;
.   .   for (j = i+1; j < MAX; j++) {
.   .   .   if (grille[i][col] == grille[j][col]) {
.   .   .   .   printf("Erreur colonne %d : chiffres egaux\n", col+1);
.   .   .   .   return FAUX;
.   .   .   }/* end if */
.   .   }/* end for j */
.   }/* end for i */
.   return VRAI;
}/* end verif_saisie_une_colonne */

/*********************/
int verif_saisie_carres(void)
/*********************/
{/* verifie si pas de doublon dans les carres de 3 X 3 */
.   int carre;
.   int result = VRAI;
.   for (carre = 0; carre < MAX; carre++) {
.   .   if (! verif_saisie_un_carre(carre)) result = FAUX;
.   }/* end for */
.   return result;
}/* end verif_saisie_carres */

/***********************/
int verif_saisie_un_carre(int carre)
/***********************/
{/* verifie si pas de doublon dans le carre de 3 X 3 */
.   char tamp[MAX+10];
.   int i, j;
.   trans_carre(tamp, carre);
.   for (i = 0; i < MAX-1; i++) {
.   .   if (tamp[i] == 0 ) continue;
.   .   for (j = i+1; j < MAX; j++) {
.   .   .   if (tamp[i] == tamp[j]) {
.   .   .   .   printf("Erreur carre %d : chiffres egaux\n", carre+1);
.   .   .   .   return FAUX;
.   .   .   }/* end if */
.   .   }/* end for j */
.   }/* end for i */
.   return VRAI;
}/* end verif_saisie_un_carre */

/**************/
void trans_carre(char tamp[], int carre)
/**************/
{/* transfert le carre de 3 X 3 dans un tableau a une dimension */
.   int i,j, x,y,z;
.   x = (((carre) % MIN)*MIN);
.   y = (((carre+MIN) / MIN)*MIN)-MIN;
.   z = 0;
.   for (j = y; j < y+MIN; j++) {
.   .   for (i = x; i < x+MIN; i++) {
.   .   .   tamp[z++] = grille[j][i];
.   .   }/* end for i */
.   }/* end for j */
}/* end trans_carre */

/********************/
/**** RESOLUTION ****/
/********************/

/**********/
void resolve(void)
/**********/
{/* Attention, fonction recursive... */
.   int ligne, col, nbre, nbre_tamp;
.   for (ligne = 0; ligne < MAX; ligne++) {
.   .   for (col = 0; col < MAX; col++) {
.   .   .   if (grille[ligne][col]) continue;
.   .   .   for (nbre = 1; nbre <= MAX; nbre++) {
.   .   .   .   if (! case_dispo_nbre(nbre, ligne, col)) continue;
.   .   .   .   nbre_tamp = grille[ligne][col];
.   .   .   .   grille[ligne][col] = nbre;
.   .   .   .   g_comp_tests++;
.   .   .   .   if (g_flag_affiche) affiche_grille_attente();
.   .   .   .   resolve();
.   .   .   .   if (grille_finie()) return;
.   .   .   .   /* pour voir toutes les solutions des grilles
.   .   .   .   a solutions multiples
.   .   .   .   mettre en rem la ligne du dessus et
.   .   .   .   retirer le rem de la ligne du dessous */
.   .   .   .   /* if (grille_finie()) affiche_grille_attente(); */
.   .   .   .   grille[ligne][col] = nbre_tamp;
.   .   .   }/* end for nbre */
.   .   .   return;
.   .   }/* end for col */
.   }/* end for ligne */
.   return;
}/* end resolve */

/******************/
int case_dispo_nbre(int nbre, int ligne, int col)
/******************/
{
/* teste si le nbre est deja place dans
la ligne, ou la colonne, ou le carre de 3 X 3
afferent a la case pointee par ligne, col */
.   char tamp[MAX+10];
.   int i;
.   /*si nbre est utilise dans la ligne, renvoi FAUX */
.   for (i = 0; i < MAX; i++) if (grille[ligne][i] == nbre) return(FAUX);
.   /*si nbre est utilise dans la colonne, renvoi FAUX */
.   for (i = 0; i < MAX; i++) if (grille[i][col] == nbre) return(FAUX);
.   /*si nbre est utilise dans le mini carre, renvoi FAUX */
.   trans_carre_de_case(tamp, ligne, col);
.   for (i = 0; i < MAX; i++) if (tamp[i] == nbre) return (FAUX);
.   /*donc nbre est diponible pour la case */
.   return(VRAI);
}/* end case_dispo_nbre */

/**********************/
void trans_carre_de_case(char tamp[], int ligne, int col)
/**********************/
/* tranfert le carre de 3 X 3 d'une 
case dans un tableau a une dimension */
{
.   int i, j, k;
.   while ((ligne % MIN) != 0) ligne--;
.   while ((col % MIN) != 0) col--;
.   k = 0;
.   for (j = ligne; j < ligne+MIN; j++) {
.   .   for (i = col; i < col+MIN; i++) {
.   .   .   tamp[++k] = grille[j][i];
.   .   }/* end for i */
.   }/* end for j */
}/* end trans_carre_de_case */

/****************/
/**** DIVERS ****/
/****************/

/*************************/
void affiche_grille_attente(void)
/*************************/
{/* affiche la grille avec mise en attente de la touche entree */
.   affiche_grille();
.   printf("%ld, Touche entree pour la suite...", g_comp_tests);
.   attente();
}/* end option_affiche */

/*****************/
void affiche_grille(void)
/*****************/
{/* affiche l'ensemble de la grille */
.   int ligne, col;
.   for (ligne = 0; ligne < MAX; ligne++) {
.   .   for (col = 0; col < MAX; col++) {
.   .   .   printf(" %1d ", grille[ligne][col]);
.   .   .   if ( ((col % MIN) == MIN-1) && (col < MAX-1) ) {
.   .   .   .   printf(" | ");
.   .   .   }
.   .   }/* end for col */
.   .   printf("\n");
.   .   if ( ((ligne % MIN) == MIN-1) && (ligne < MAX-1) ) {
.   .   .   affiche_chaine(33);
.   .   }
.   }/* end for ligne */
.   printf("\n");
}/* end affiche_grille */

/*****************/
void affiche_chaine(int nbre)
/*****************/
{/* Affiche une suite de caractere etoile */
.   int i;
.   for (i = 1; i <= nbre; i++) {
.   .   printf("*");
.   }
.   printf("\n");
}/* end affiche_chaine */

/**************/
int grille_finie(void)
/**************/
/* teste si la grille est finie.
Retourne VRAI si grille finie, sinon retourne FAUX.
Commence par la fin de la grille
celle ci ce completant par son debut */
{
.   int ligne, col;
.   for (ligne = MAX-1; ligne >= 0; ligne--) {
.   .   for (col = MAX-1; col >= 0; col--) {
.   .   .   if (grille[ligne][col] == 0 ) {
.   .   .   .   return FAUX;
.   .   .   }
.   .   }/* end for col */
.   }/* end for ligne */
.   return VRAI;
}/* end grille_finie */

/**********/
void attente(void)
/**********/
{/* attente de la touche entree au clavier */
.   char c;
.   while(VRAI){
.   .   c = fgetc(stdin);
.   .   if (c == feof(stdin) || c == 0x0A) return;
.   }/* end while */
}/* end attente */
