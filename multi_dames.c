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

void pprint(Jeu *jeu) {
    printf("\n---------\n");
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("%d ", jeu->plateau.pion[i][j]);
        }
        printf("\n");
    }
    printf("\n---------");
}

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

    printf("\n\n ||jeu|| Score de joueur %d : %d", jeu->joueur_courant + 1, jeu->joueur[jeu->joueur_courant].score);

    return 0;
}

int jeu_sauter_vers(Jeu *jeu, int i, int j) {
    // Vérifier coordonnées et pion saisi optionnellement (non ajouté ici)
    int pion = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];

    // calculer coordonnées du pion "pris" (case intermédiaire)
    int mi = (jeu->pion_i + i) / 2;
    int mj = (jeu->pion_j + j) / 2;

    printf("||DEBUG|| Pion i : %d Pion j : %d\n", jeu->pion_i, jeu->pion_j);
    printf("||DEBUG|| Coords du pion qui va se FAIRE BOUFFER : %d %d\n", mi, mj);

    int pionpris = jeu->plateau.pion[mi][mj];
    printf("||DEBUG|| Valeur pion mangé : %d\n", pionpris);

    // déplacer le pion (mettre destination puis vider ancienne case)
    jeu->plateau.pion[i][j] = pion;
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;

    // actualiser le score et enlever le pion pris
    actualise_score(jeu, pionpris);
    jeu->plateau.pion[mi][mj] = 0;

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

void get_coords_init(int* x, int* y) {
    int isitok = 1;
    do
    {

        printf("\n||jeu|| Coordoonées (int x) puis (int y) :");
        scanf("%d %d", y, x);

        isitok = 0<*x && *x<TAILLE && 0<*y && *y<=TAILLE;

    } while (!(isitok));

    //Afin que les indices du tableau soient corrects
}

void get_coords(int* x, int* y) {
    int isitok = 1;
    do
    {

        printf("\n||jeu|| Coordoonées (int x) puis (int y) :");
        scanf("%d %d", y, x);

        isitok = 0<*x && *x<TAILLE && 0<*y && *y<=TAILLE;

    } while (!(isitok));

    //Afin que les indices du tableau soient corrects
}

int jeu_initialisation(Jeu *jeu) {
    int x, y;
    for (int i = 0; i < jeu->nb_joueurs; i++)
    {
        x = -1;
        y = -1;
        
        printf("\n||jeu|| Tour de mise en place, retirez un pion 1 (Joueur %d) :\n", i+1);

        do
        {
            get_coords(&x,&y);
        } while (jeu->plateau.pion[x-1][y-1] != 1);
        
        jeu_initial_retire_pion(jeu, x-1, y-1);
        jeu_joueur_suivant(jeu);
        pprint(jeu);
    }

    return 1;
}

int jeu_peut_sauter(Jeu *jeu, int i, int j) {
    //Vérifie si un saut est possible pour le pion saisi
    int di[4] = {-2, -2, 2, 2};
    int dj[4] = {-2, 2, -2, 2};

    for (int index = 0; index < 4; index++)
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

int jeu_peut_jouer(Jeu *jeu) {
    //Vérifie si le joueur courant peut jouer
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (jeu->plateau.pion[i][j] != 0)
            {
                //On simule la saisie du pion
                jeu->pion_i = i;
                jeu->pion_j = j;

                if (jeu_peut_sauter(jeu, i, j))
                {
                    return 1; // Le joueur peut jouer
                }
            }
        }
    }

    return 0; // Le joueur ne peut pas jouer
}

void penalité (Jeu *jeu) {
    //On enlève la sommes des valeurs des pions restants au score du joueur
    int penalty = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            penalty = jeu->plateau.pion[i][j];
            actualise_score(jeu, -penalty);
        }
    }
}

int pion_saut_autorise(Jeu *jeu, int i, int j) {
    //Vérifie si le saut est autorisé pour le pion saisi
    int deltax = i - jeu->pion_i;
    int deltay = j - jeu->pion_j;

    //Le saut doit être de 2 cases dans une direction
    if ((abs(deltax) == 2 && deltay == 0) || (abs(deltay) == 2 && deltax == 0) || (abs(deltax) == 2 && abs(deltay) == 2))
    {
        //Vérifie que la case d'arrivée est vide
        if (jeu->plateau.pion[i][j] == 0)
        {
            //Vérifie qu'il y a un pion à manger
            int mi = (jeu->pion_i + i) / 2;
            int mj = (jeu->pion_j + j) / 2;

            if (jeu->plateau.pion[mi][mj] != 0)
            {
                return 1; // Le saut est autorisé
            }
        }
    }

    return 0; // Le saut n'est pas autorisé
}

int main(){
    Jeu game;
    game.nb_joueurs = 2;

    jeu_init_board(&game);

    printf("----------\\ Plateau initial /----------\n");
    pprint(&game);


    jeu_initialisation(&game);

    for (int i = 0; i < game.nb_joueurs; i++)
    {
        game.joueur[i].etat = 1;
        game.joueur[i].score = 0;
    }
    game.tour = 0;

    int game_over = 0;

    while (!(game_over))
    {
        for (int i = 0; i < game.nb_joueurs; i++)
        {
            if (jeu_peut_jouer(&game))
            {   printf("\n||DEBUG|| Joueur %d peut jouer\n", i+1);
                if (game.joueur[i].etat)
                {
                    int x, y;
                    game.pion_est_saisi = 0;
                    game.joueur_courant = i;
                    

                    pprint(&game);
                    printf("\n||jeu|| Saisir quel pion ? (int x) puis (int y) :\n");
                    
                    do
                    {
                        get_coords(&x, &y);
                        printf("\n||DEBUG|| Vérification saisie pion %d,%d\n", y-1, x-1);
                        printf("\n||DEBUG|| Valeur du pion : %d\n", game.plateau.pion[x - 1][y - 1]);
                        printf("\n||DEBUG|| Peut sauter ? %d\n", jeu_peut_sauter(&game, x - 1, y - 1));
                    } while (game.plateau.pion[x - 1][y - 1] == 0 || !jeu_peut_sauter(&game, x - 1, y - 1));
                    
                    jeu_saisir_pion(&game, x - 1, y - 1);

                    printf("\n||jeu|| Vers où sauter ? (int x) puis (int y) :\n");
                    
                    do
                    {
                        get_coords(&x, &y);
                        printf("\n||DEBUG|| Vérification saut vers %d,%d\n", y-1, x-1);
                    } while (!pion_saut_autorise(&game, x - 1, y - 1));
                    
                    jeu_sauter_vers(&game, x - 1, y - 1);

                }
            } else {
                game.joueur_courant = i;

                penalité(&game);

                game_over = 1;
                
            }
            
            game.tour += 1;
            
        }
        
    }
    

}

