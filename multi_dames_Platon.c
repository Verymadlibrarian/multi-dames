#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 8

typedef struct {
    // 0 si vide, 1 si blanc, 2 si rouge et 3 si noir
    int pion[TAILLE][TAILLE];
} Plateau;

typedef struct {
    int etat; // 1 si dans la partie, 0 sinon
    int score;
} Joueur;

#define MAX_JOUEURS 4

typedef struct {
    Plateau plateau;
    Joueur joueur[MAX_JOUEURS];
    int nb_joueurs;
    int joueur_courant;
    int tour;
    int pion_est_saisi; // 1 si un pion est saisi, 0 sinon
    int pion_i, pion_j; // la ligne et colonne du pion saisi (0 sinon)
} Jeu;

int abs();

void jeu_charger(Jeu *jeu) {
    scanf("%d %d %d", &(jeu->nb_joueurs), &(jeu->tour), &(jeu->joueur_courant));

    for (int i = 0; i < jeu->nb_joueurs; i++)
        scanf("%d %d", &(jeu->joueur[i].etat), &(jeu->joueur[i].score));

    scanf("%d %d %d", &(jeu->pion_est_saisi), &(jeu->pion_i), &(jeu->pion_j));

    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            scanf("%d", &(jeu->plateau.pion[i][j]));

}

void jeu_ecrire(Jeu *jeu){
    printf("%d %d %d\n",jeu->nb_joueurs, jeu->tour, jeu->joueur_courant);

    for (int i = 0; i < jeu->nb_joueurs; i++) 
        printf("%d %d\n", jeu->joueur[i].etat, jeu->joueur[i].score);

    printf("%d %d %d", jeu->pion_est_saisi, jeu->pion_i, jeu->pion_j);

    for (int i = 0; i < TAILLE; i++) {
        printf("\n");
        for (int j = 0; j < TAILLE; j++) 
            printf("%d ", jeu->plateau.pion[i][j]);
    }
}

int jeu_joueur_suivant(Jeu *jeu) {
    //On change de joueur
    do {
    jeu->joueur_courant = (jeu->joueur_courant + 1) % jeu->nb_joueurs;
    if (jeu->joueur_courant == 0) jeu->tour += 1; 
    } while (!(jeu->joueur[jeu->joueur_courant].etat));
    jeu->pion_est_saisi = 0;
    jeu->pion_i = 0;
    jeu->pion_j = 0;

    return 1;
}

int plus_de_un_joueur(Jeu *jeu) {
    int n = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++)
    if (jeu->joueur[i].etat) n++;
    return n++ > 1;
}

int jeu_arreter(Jeu *jeu) {
    if (plus_de_un_joueur(jeu))
    {
        //Mise hors-jeu du joueur
        jeu->joueur[jeu->joueur_courant].etat = 0;
        return 1;
    }
    
    return 0;
}

int actualise_score(Jeu *jeu, int pionvalue) {

    switch (pionvalue)
    {
    case 1:
        jeu->joueur[jeu->joueur_courant].score += 1;
        break;
    case 2:
        jeu->joueur[jeu->joueur_courant].score += 5;
        break;
    case 3:
        jeu->joueur[jeu->joueur_courant].score += 8;
        break;
    case -1:
        jeu->joueur[jeu->joueur_courant].score -= 1;
        break;
    case -2:
        jeu->joueur[jeu->joueur_courant].score -= 5;
        break;
    case -3:
        jeu->joueur[jeu->joueur_courant].score -= 8;
        break;
    default:
        break;
    }

    // printf("\n\n ||jeu|| Score de joueur %d : %d", jeu->joueur_courant + 1, jeu->joueur[jeu->joueur_courant].score);

    return 0;
}


int jeu_initial_retire_pion(Jeu *jeu, int i, int j) {
    int pionpris = jeu->plateau.pion[i][j];

    actualise_score(jeu, pionpris);
    jeu->plateau.pion[i][j] = 0;

    return 1;
}

int jeu_capturer(Jeu *jeu, int i, int j){
    if (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE) {
        jeu_initial_retire_pion(jeu, i, j);
        return 1;
    }
    return 0;
}

int jeu_peut_sauter(Jeu *jeu, int i,__attribute__((__unused__)) int j) {
    //Vérifie si un saut est possible pour le pion saisi
    (void)i;
    int di[8] = {-2,-2, 2, 2,-2, 0, 2, 0};
    int dj[8] = {-2, 2,-2, 2, 0,-2, 0, 2};

    for (int index = 0; index < 8; index++)
    {   
        int ni = jeu->pion_i + di[index];
        int nj = jeu->pion_j + dj[index];

        //Vérifie que la case d'arrivée est dans le plateau
        if (ni >= 0 && ni < TAILLE && nj >= 0 && nj < TAILLE)
        {   
            //Vérifie que la case d'arrivée est vide et qu'il y a un pion à manger
            if (jeu->plateau.pion[ni][nj] == 0)
            {   
                int mi = (jeu->pion_i + ni) / 2;
                int mj = (jeu->pion_j + nj) / 2;

                if (jeu->plateau.pion[mi][mj] != 0)
                {
                    return 1; // Un saut est possible
                }
            }
        }
    }

    return 0; // Aucun saut possible
}

int jeu_saisir_pion_dummy(Jeu *jeu, int i, int j) {
    //On actualise les données du pion dans jeu
    jeu->pion_est_saisi = 1;
    jeu->pion_i = i;
    jeu->pion_j = j;

    return 1;
}


int jeu_saisir_pion(Jeu *jeu, int i,int j){
    if (jeu->pion_est_saisi) return 0;
    if (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE) {
        if (!(jeu->plateau.pion[i][j])) return 0;
        int value = jeu_saisir_pion_dummy(jeu, i, j);
        if (!jeu_peut_sauter(jeu,i,j)) {
            jeu->pion_est_saisi = 0;
            jeu->pion_i = 0;
            jeu->pion_j = 0;
            return 0;
        }
        return value;
    }
    return 0;
}

int jeu_sauter_vers_dummy(Jeu *jeu, int i, int j) {
    // Vérifier coordonnées et pion saisi optionnellement (non ajouté ici)
    int pion = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];

    // calculer coordonnées du pion "pris" (case intermédiaire)
    int mi = (jeu->pion_i + i) / 2;
    int mj = (jeu->pion_j + j) / 2;

    // printf("||DEBUG|| Pion i : %d Pion j : %d\n", jeu->pion_i, jeu->pion_j);
    // printf("||DEBUG|| Coords du pion qui va se FAIRE BOUFFER : %d %d\n", mi, mj);

    int pionpris = jeu->plateau.pion[mi][mj];
    // printf("||DEBUG|| Valeur pion mangé : %d\n", pionpris);

    // déplacer le pion (mettre destination puis vider ancienne case)
    jeu->plateau.pion[i][j] = pion;
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;

    // actualiser le score et enlever le pion pris
    actualise_score(jeu, pionpris);
    jeu->plateau.pion[mi][mj] = 0;

    jeu->pion_i = i;
    jeu->pion_j = j;
    return 1;
}

int jeu_sauter_vers(Jeu *jeu, int i, int j) {
    if (!jeu->pion_est_saisi) return 0;
    if (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE) {
        if (!((abs(jeu->pion_i - i) == 2 || abs(jeu->pion_i - i) == 0)) || !((abs(jeu->pion_j - j) == 2 || abs(jeu->pion_j - j) == 0))) return 0;
        int mi = (jeu->pion_i + i) / 2;
        int mj = (jeu->pion_j + j) / 2;
        if (!jeu->plateau.pion[mi][mj]) return 0;
        if ((jeu->plateau.pion[i][j])) return 0;
        int value = jeu_sauter_vers_dummy(jeu, i, j);
        return value;
    }
    return 0;
}

void joueurs_affiche(Jeu *jeu) {
    for (int i = 0; i < jeu->nb_joueurs; i++) printf("J%d : %dpts | ",i+1,(jeu->joueur)[i].score);
    printf("\n               ");
    for (int i = 0; i < jeu->nb_joueurs; i++) 
    if (jeu->joueur[i].etat == 0) printf(" (a quitte) ");
    else printf("            ");
    printf("\n");
}

void jeu_affiche(Jeu *jeu) {

    if ((jeu->tour)) printf("----------\\ Plateau tour %d /----------\n",jeu->tour);
    else printf("----------\\ Plateau initial /----------\n");
    printf("Score actuels : ");
    joueurs_affiche(jeu);
    printf("Tour du Joueur %d",jeu->joueur_courant+1);
    printf("\n---------\n");
    printf("  x1 2 3 4 5 6 7 8\n");
    printf("y  | | | | | | | |\n");
    for (int i = 0; i < TAILLE; i++) {
        printf("%d— ",i+1);
        for (int j = 0; j < TAILLE; j++) {
            printf("%d ", jeu->plateau.pion[i][j]);
        }
        printf("\n");
    }
    printf("\n---------");
}

int main()
{
    Jeu game;
    jeu_charger(&game);
    jeu_affiche(&game);
    jeu_capturer(&game, 2, 2);
    jeu_affiche(&game);
    return 0;
}
