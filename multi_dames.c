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

int jeu_capturer(Jeu *jeu, int i, int j) {
    //On vide la case
    jeu->plateau.pion[i][j] = 0;

    return 1;
}

int jeu_arreter(Jeu *jeu) {
    if (jeu->nb_joueurs>1)
    {
        //Mise hors-jeu du joueur
        jeu->joueur[jeu->joueur_courant].etat = 0;
        return 0;
    }
    
    return 1;
}

int jeu_joueur_suivant(Jeu *jeu) {
    //On change de joueur
    jeu->joueur_courant = (jeu->joueur_courant + 1) % jeu->nb_joueurs;

    //On augmente le tour
    jeu->tour += 1;

    return 1;
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
    default:
        printf("something went wrong when giving out point, check pionpris value");
        break;
    }

    return 1;
}

int jeu_sauter_vers(Jeu *jeu, int i, int j) {
    int pion = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];
    //On duplique le pion qui bouge
    jeu->plateau.pion[i][j] = pion;
    //On met son ancienne position à 0
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;

    //On calcule le delta entre avant et après pour voir le pion à manger
    int deltax = i - jeu->pion_i;
    int deltay = j - jeu->pion_j;
    
    //On le mange
    int pionpris = jeu->plateau.pion[jeu->pion_i + deltax][jeu->pion_j + deltay];

    //On actualise les scores
    actualise_score(jeu, pionpris);
    
    return 1;
}


int jeu_saisir_pion(Jeu *jeu, int i, int j) {
    //On actualise les données du pion dans jeu
    jeu->pion_est_saisi = 1;
    jeu->pion_i = i;
    jeu->pion_j = j;

    return 1;
}

int jeu_initial_retire_pion(Jeu *jeu, int i, int j) {
    int pionpris = jeu->plateau.pion[i][j];

    actualise_score(jeu, pionpris);
    jeu->plateau.pion[i][j] = 0;

    return 1;
}

int jeu_init_board(Jeu *jeu) {
    int left[3] = {34,20,10};
    srand(time(NULL));

    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            //On prend un int random entre 1 et 3
            int valeur = (rand() % 3) + 1;

            //On vérifie qu'il en reste
            while ((left[valeur - 1]) == 0)
            {
                valeur = (rand() % 3) + 1;
            }
            
            //Puis on l'assigne
            left[valeur - 1] -= 1;
            jeu->plateau.pion[i][j] = valeur;
        }
    }
    return 1;
}

int jeu_initialisation(Jeu *jeu) {
    return 1;
}

int main(){
    Jeu game;
    game.nb_joueurs = 1;

    jeu_init_board(&game);
    printf("Plateau initial:\n");
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("%d ", game.plateau.pion[i][j]);
        }
        printf("\n");
    }
}

