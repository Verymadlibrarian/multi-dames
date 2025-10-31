# Crédits
### Fait par :
- Viste Lilan
- Vilmot Mathieu

**Du groupe TP_double_licence.**

# Instructions
## Compilation du programme.

- Dans votre terminal, rendez-vous dans le dossier du fichier "multi_dames.c"
- Puis entrez la commande 
> gcc -Wall -Werror -Wextra -O2 -o multi_dames multi_dames.c
- enfin pour lancer le programme, entrez la commande
> ./multi_dames.out
- ou pour Windows 
> ./multi_dames.exe

## Comment Jouer ?

En lançant l'executable, le terminal affichera les instructions nécéssaires pour jouer.

Pour commencer, il faudra choisir le nombre de joueurs pour la partie. (Entre 2 et 4 inclus).

Après cela, le plateau de jeu sera affiché avec l'instruction de l'action actuelle.

Au tour 0 (ou tour Initial), chaque joueur devra retirer un pion. Il faudra alors entrer les coordonnées du pion choisi pour chaque joueur (le joueur actuel étant indiqué au dessus du plateau).

Les coordonnées sont toujours deux entiers entre 1 et 8 inclus. Leur pion correspondants étant facilement déduisable avec les lignes et les colonnes indiquées.

Après le tour 0, le jeu peut enfin commencer!

Les joueurs, tour à tour, vont devoir choisir un pion a faire sauter pour récupérer des points. A la saisie du pion, les joueurs ont aussi la possibilité de partir du jeu, permettant d'éviter la pénalité de fin de partie.

Pour cela, le joueur courant devra entrer "0 0" en tant que coordonnées du pion. Le programme détecte alors cette entrée comme fin de partie pour le joueur courant. Ceci n'est cependant pas possible si il reste qu'un seul joueur dans la partie. (Indiqué par le manque d'information sur "0 0" dans les instructions d'entrée).

A la saisie du pion, le joueur courant devra alors sauter jusqu'a l'impossibilité de faire cela, permettant de capturer les pions passés augmentant alors son score.

La partie prend fin lorsque plus aucun pion peut avoir de sauts valable. Tous les pions sont alors comptabilisés comme points négatifs pour le joueur courant.

Si vous voulez lancer une nouvelle partie, relancer le programme.

# Difficultées
### 1. L'insertitude de certaines attentes.

Je pense que la partie la plus complexe était de comprendre exactement ce qu'on attendait de nous. Notre programme est très grand pour pas grand chose pour cette raison.

Lorsque nous commençions, nous pensions que le but était de faire des fonctions très simple, avec d'autre fonctions vérifiants les cas impossible. Puis Platon nous a montré que les fonctions comme cela ne sont pas possible pour les tests.

Nous avons dû alors créer les fonctions de tests séparément du programme du jeu.

Les fonctions avec le suffixe "dummy" étant littéralement, leurs version brute, sans reflexion.

Les fonctions de vérification sont cependant elles fonctionnelles comme elle le sont, permettant un test rapide et fonctionnel.

### 2. La normalisation de x et y.

Le deuxième problèmes survenu très tôt était la normalisation de x et y.

En effet, les fonctions au début n'ayant pas vraiment de tests réels, des difficultés sont survenus quand certaines fonctions n'utilisait pas le même système de coordonnées que d'autres.

Nous voulions de plus, une entrée extrêmement simplifiée pour les utilisateurs et la seul action qu'ils ont est l'entrée de coordonnées de pion.

Il fallait alors un système de coordonnées simple à comprendre même si il était difficile a utiliser en programmation. Nous avon décider sur x : colonnes, y : lignes et la valeur de début étant 1.

Nous avons aussi créer les fonctions d'affichage permettant une simple décision pour l'utilisateur.