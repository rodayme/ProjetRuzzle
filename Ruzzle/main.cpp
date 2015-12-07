#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>
#include <time.h>
typedef int bool;
#define true 1
#define false 0

// Différents bonus possibles

typedef enum {NO_BONUS, DOUBLE_WORD, TRIPLE_WORD, DOUBLE_LETTER, TRIPLE_LETTER} bonus_enum;

// Structure des lettres

typedef struct letter_struct letter_struct;
struct letter_struct {
    int letter_alphabetOrder;
    char letter_libelle;
    int letter_points;
    const char* letter_pointsLabel;
    bool isVoyelle;
};

// Structure des éléments de la grille

typedef struct grid_struct grid_struct;
struct grid_struct {
    int position_x;
    int position_y;
    letter_struct letter;
    bonus_enum letter_bonus;
    bool isChoose;
    bool isProposed;
};

// Différentes variables globales
char wordsDico_tab[318813][30] = {{0},{0}};
char wordsUsed_tab[50][16] = {{0},{0}};
grid_struct lettersSelected_tab[16];
grid_struct wordsPossibles[200][16];
grid_struct letters_grid[4][4];
struct letter_struct alphabet_tab[26];
int indexDico_tab[26];
int lettersSelectedLength = 0;
int numberWordsFind = 0;
int numberWordsPossibles = 0;

//Variables globales de l'alphabet
    // Lettre A
    letter_struct letter_a = {1,'A',1,"1",1};
    // Lettre B
    letter_struct letter_b = {2,'B',3,"3",0};
    // Lettre C
    letter_struct letter_c = {3,'C',3,"3",0};
    // Lettre D
    letter_struct letter_d = {4,'D',2,"2",0};
    // Lettre E
    letter_struct letter_e = {5,'E',1,"1",1};
    // Lettre F
    letter_struct letter_f = {6,'F',4,"4",0};
    // Lettre G
    letter_struct letter_g = {7,'G',2,"2",0};
    // Lettre H
    letter_struct letter_h = {8,'H',4,"4",0};
    // Lettre I
    letter_struct letter_i = {9,'I',1,"1",1};
    // Lettre J
    letter_struct letter_j = {10,'J',8,"8",0};
    // Lettre K
    letter_struct letter_k = {11,'K',10,"10",0};
    // Lettre L
    letter_struct letter_l = {12,'L',1,"1",0};
    // Lettre M
    letter_struct letter_m = {13,'M',2,"2",0};
    // Lettre N
    letter_struct letter_n = {14,'N',1,"1",0};
    // Lettre O
    letter_struct letter_o = {15,'O',1,"1",1};
    // Lettre P
    letter_struct letter_p = {16,'P',3,"3",0};
    // Lettre Q
    letter_struct letter_q = {17,'Q',6,"6",0};
    // Lettre R
    letter_struct letter_r = {18,'R',1,"1",0};
    // Lettre S
    letter_struct letter_s = {19,'S',1,"1",0};
    // Lettre T
    letter_struct letter_t = {20,'T',1,"1",0};
    // Lettre U
    letter_struct letter_u = {21,'U',1,"1",1};
    // Lettre V
    letter_struct letter_v = {22,'V',4,"4",0};
    // Lettre W
    letter_struct letter_w = {23,'W',10,"10",0};
    // Lettre X
    letter_struct letter_x = {24,'X',10,"10",0};
    // Lettre Y
    letter_struct letter_y = {25,'Y',10,"10",0};
    // Lettre Z
    letter_struct letter_z = {26,'Z',10,"10",0};
    // Lettre vide
    letter_struct letter_empty = {0,' ',0,"0",0};

//Ensemble des fonctions
void initWordsDico_tab(char filename[]);
void initIndexDico_tab();
void initAlphabet_tab();
bool wordIsExist();
bool wordIsExistBis(grid_struct word[]);
bool wordUsedIsExist();
bool wordAlreadyFind(grid_struct word[]);
void displayAlphabet_tab();
void displayLettersSelected_tab();
void displayWordsDico_tab();
void displayWordsPossibles();
bool add_lettersSelected_tab(grid_struct letter_to_add);
bool add_wordsUsed_tab(char word[]);
void reset_lettersSelected_tab();
void reset_lettersGrid();
int calcul_score_word();
void display_score_word();
void initGrid(char grid[16][2]);
void initGridFile(char nameFile[],char grid[16][2]);
void init();
int compAlphaOrder(char *chaine_un,char *chaine_deux);
bool wordIsPossible(grid_struct word[]);
void initWordsPossibles();
int researchAlphabetLetter(char caractere);

// Programme principal du jeu

int main(int argc, char *argv[])
{
    // Affiche sur la console
    freopen("CON","w",stdout);
    // Initialisation
    init();
    // Surface -> sert à afficher les différents éléments graphiques du programme
    SDL_Surface *ecran = NULL, *fondEcran = NULL, *caseGrid = NULL, *caseError = NULL, *caseSelected = NULL, *caseProposed = NULL, *wordsUsedLabel = NULL, *timer = NULL, *scoreLabel = NULL, *triple_word_legend_label = NULL, *double_word_legend_label = NULL, *triple_letter_legend_label = NULL, *double_letter_legend_label = NULL, *timer_icon = NULL, *titre = NULL, *wordLabel = NULL, *double_letter_icon = NULL, *triple_letter_icon = NULL, *double_word_icon = NULL, *triple_word_icon = NULL;
    SDL_Surface *caracteres[4][4], *points[4][4];
    // Rect -> sert au positionnement
    SDL_Rect r,positionTimerIcon,positionFondEcran,positionTitre,positionWordsUsed,positionDoubleWordIcon,positionDoubleWordLegende,positionDoubleLetterIcon,positionDoubleLetterLegende,positionTripleWordIcon,positionTripleWordLegende,positionTripleLetterIcon,positionTripleLetterLegende,positionTimer,positionScore,positionCaractere,positionPoints,positionWord,positionBonus;
    // Polices de caractères
    TTF_Font *police = NULL, *policeCaractere = NULL, *policePoints = NULL, *policeLegende = NULL;
    // Différentes variables
    int continuer = 1, isPause = 0, weight, height,i,j,k,l,positionSouris_x,positionSouris_y;
    int caseChoose_x = -1;
    int caseChoose_y = -1;
    int tempsActuel = 0, tempsPrecedent = 0, compteur = 120, scoreValue = 0;
    // Différents tableaux de caractères pour l'affichage
    char temps[20] = "", score[20] = "", word[16] = "", legend_triple_word[25] = "Mot x 3", legend_triple_letter[25] = "Lettre x 3", legend_double_word[25] = "Mot x 2", legend_double_letter[25] = "Lettre x 2";
    // Couleurs
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Color couleurBlanche = {255, 255, 255};
    SDL_Color couleurViolette = {130, 0, 255};
    SDL_Color couleurGrise = {96, 96, 96};
    // Evenement pour la détection d'appui sur une touche ou un click de la souris par l'utilisateur
    SDL_Event event;
    // Initialisation de la fonction rand()
    srand(time(NULL));
    // Initialisation de la grille
    char grid[16][2];
    //initGrid(grid);
    initGridFile("Grille.txt",grid);
    // Recherche des mots possibles à partir de la grille générée aléatoirement
    initWordsPossibles();
    // Affichage sur la console de ces mots
    displayWordsPossibles();
    //Affectation des positions des éléments
    positionFondEcran.x = 0;
    positionFondEcran.y = 0;
    positionWordsUsed.x = 20;
    positionWordsUsed.y = 15;
    positionScore.x = 380;
    positionScore.y = 400;
    positionTimerIcon.x = 60;
    positionTimerIcon.y = 392;
    positionTimer.x = 120;
    positionTimer.y = 400;
    positionTitre.x = 215;
    positionTitre.y = 20;
    positionWord.x = 220;
    positionWord.y = 115;
    positionDoubleLetterIcon.x = 470;
    positionDoubleLetterIcon.y = 207;
    positionDoubleLetterLegende.x = 500;
    positionDoubleLetterLegende.y = 200;
    positionDoubleWordIcon.x = 470;
    positionDoubleWordIcon.y = 247;
    positionDoubleWordLegende.x = 500;
    positionDoubleWordLegende.y = 240;
    positionTripleLetterIcon.x = 470;
    positionTripleLetterIcon.y = 287;
    positionTripleLetterLegende.x = 500;
    positionTripleLetterLegende.y = 280;
    positionTripleWordIcon.x = 470;
    positionTripleWordIcon.y = 327;
    positionTripleWordLegende.x = 500;
    positionTripleWordLegende.y = 320;
    // Initialisation pour la SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Initialisation et affectation pour les polices
    TTF_Init();
    police = TTF_OpenFont("Animal Silence.otf", 40);
    policeCaractere = TTF_OpenFont("orange juice 2.0.ttf", 36);
    policePoints = TTF_OpenFont("BADABB__.ttf", 15);
    policeLegende = TTF_OpenFont("INFECTED.ttf", 25);
    // Mise dans le tableau des caracteres de la grille les libelles des lettres sorties aléatoirement
    for(int ligne = 0;ligne < 4;ligne++)
    {
        for(int colonne = 0;colonne < 4;colonne++)
        {
            const char* lettre = &letters_grid[ligne][colonne].letter.letter_libelle;
            caracteres[ligne][colonne] = TTF_RenderText_Blended(policeCaractere, lettre , couleurNoire);
        }
    }
    // Mise dans le tableau des points de la grille les points des lettres sorties aléatoirement
    for(int ligne = 0;ligne < 4;ligne++)
    {
        for(int colonne = 0;colonne < 4;colonne++)
        {
            const char* pointsValue = letters_grid[ligne][colonne].letter.letter_pointsLabel;
            points[ligne][colonne] = TTF_RenderText_Blended(policePoints, pointsValue, couleurNoire);
        }
    }
    // Initialisation des textes à afficher
    wordLabel = TTF_RenderText_Blended(police, word, couleurBlanche);
    double_letter_legend_label = TTF_RenderText_Blended(policeLegende,legend_double_letter, couleurNoire);
    triple_letter_legend_label = TTF_RenderText_Blended(policeLegende,legend_triple_letter, couleurNoire);
    double_word_legend_label = TTF_RenderText_Blended(policeLegende,legend_double_word, couleurNoire);
    triple_word_legend_label = TTF_RenderText_Blended(policeLegende,legend_triple_word, couleurNoire);
    sprintf(score, "Score : %d", scoreValue);
    scoreLabel = TTF_RenderText_Blended(police, score, couleurBlanche);
    if(compteur%60 < 10)
    sprintf(temps, "%d : 0%d", compteur/60,compteur%60); /* On écrit dans la chaîne "temps" le nouveau temps */
    else
    sprintf(temps, "%d : %d", compteur/60,compteur%60);
    timer = TTF_RenderText_Blended(police, temps, couleurBlanche);
    // Initialisation de l'écran
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    // Titre de la fenêtre
    SDL_WM_SetCaption("Ruzzle", NULL);
    // Initialisation des images
    timer_icon = IMG_Load("timer.png");
    double_letter_icon = IMG_Load("double_letter_icon.png");
    triple_letter_icon = IMG_Load("triple_letter_icon.png");
    double_word_icon = IMG_Load("double_word_icon.png");
    triple_word_icon = IMG_Load("triple_word_icon.png");
    titre = IMG_Load("Ruzzle_logo.png");
    fondEcran = IMG_Load("FondEcran.jpe");
    caseProposed = IMG_Load("caseProposed.png");
    caseSelected = IMG_Load("caseSelected.png");
    caseError = IMG_Load("caseError.png");
    caseGrid = IMG_Load("case.png");
    // Initialisation de variables pour l'affichage
    weight = (ecran->w / 3);
    height = (ecran->h / 3);
    // Boucle du programme
    while(continuer)
    {
        if(!isPause)
            {
        // Récupération du temps
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent >= 1000) /* Si 1 s au moins s'est écoulée */
        {
            compteur -= 1; /* On rajoute 1 au compteur */
            if(compteur%60 < 10)
            sprintf(temps, "%d : 0%d", compteur/60,compteur%60); /* On écrit dans la chaîne "temps" le nouveau temps */
            else
            sprintf(temps, "%d : %d", compteur/60,compteur%60); /* On écrit dans la chaîne "temps" le nouveau temps */
            SDL_FreeSurface(timer); /* On supprime la surface précédente */
            timer = TTF_RenderText_Blended(police, temps, couleurBlanche);
            tempsPrecedent = tempsActuel; /* On met à jour le tempsPrecedent */
        }
        if(compteur == 0) /* Si le timer est fini */
        continuer = 0;
            }
        while(SDL_PollEvent(&event)) /* Détection d'évènements de la part de l'utilisateur */
        switch(event.type)
        {
            case SDL_QUIT:  /* Fermeture de la fenetre */
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                if(!isPause)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_GetMouseState(&positionSouris_x,&positionSouris_y);
                    for(int ligne = 0;ligne < 4;ligne++)
                    {
                        for(int colonne = 0;colonne < 4;colonne++)
                        {
                            if(positionSouris_x >= (letters_grid[ligne][colonne].position_x + 25) && positionSouris_x <= (letters_grid[ligne][colonne].position_x + 75) && positionSouris_y >= (letters_grid[ligne][colonne].position_y + 25) && positionSouris_y <= (letters_grid[ligne][colonne].position_y + 75) && letters_grid[ligne][colonne].isChoose == 0)
                            {
                                if((caseChoose_x == -1 && caseChoose_y == -1) || (ligne >= (caseChoose_x-1) && ligne <= (caseChoose_x+1) && colonne >= (caseChoose_y-1) && colonne <= (caseChoose_y+1) ) )
                                {
                                    if(strchr(word,' '))
                                {
                                    memset(word, 0, sizeof (word));
                                }
                                lettersSelectedLength++;
                                strcat(word,&letters_grid[ligne][colonne].letter.letter_libelle);
                                SDL_FreeSurface(wordLabel); /* On supprime la surface précédente */
                                wordLabel = TTF_RenderText_Blended(police, word, couleurBlanche);
                                letters_grid[ligne][colonne].isChoose = 1;
                                for(int ligne_compteur = 0;ligne_compteur < 4;ligne_compteur++)
                                {
                                    for(int colonne_compteur = 0;colonne_compteur < 4;colonne_compteur++)
                                    {
                                        letters_grid[ligne_compteur][colonne_compteur].isProposed = 0;
                                    }
                                }
                                for(int compteur_ligne = (ligne - 1) ; compteur_ligne <= (ligne + 1) ; compteur_ligne++)
                                {
                                    for(int compteur_colonne = (colonne - 1) ; compteur_colonne <= (colonne + 1) ; compteur_colonne++)
                                    {
                                        if(compteur_ligne < 4 && compteur_ligne > -1 && compteur_colonne < 4 && compteur_colonne > -1)
                                        letters_grid[compteur_ligne][compteur_colonne].isProposed = 1;
                                    }
                                }
                                add_lettersSelected_tab(letters_grid[ligne][colonne]);
                                caseChoose_x = ligne;
                                caseChoose_y = colonne;
                                }
                            }
                        }
                    }
                    break;
                }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    if(wordIsExistBis(lettersSelected_tab) && !wordUsedIsExist())
                    {
                                add_wordsUsed_tab(word);
                                numberWordsFind++;
                                scoreValue += calcul_score_word();
                                sprintf(score, "Score : %d", scoreValue);
                                SDL_FreeSurface(scoreLabel); /* On supprime la surface précédente */
                                scoreLabel = TTF_RenderText_Blended(police, score, couleurBlanche);
                                sprintf(word, "+ %d points",calcul_score_word());
                    }
                    else
                    {
                                if(wordUsedIsExist() == 1)
                                {
                                    strcpy(word,"Mot deja utilise");
                                }
                                else
                                {
                                    strcpy(word,"Mot Incorrect !");
                                }
                    }
                                for(int ligne_compteur = 0;ligne_compteur < 4;ligne_compteur++)
                                {
                                    for(int colonne_compteur = 0;colonne_compteur < 4;colonne_compteur++)
                                    {
                                        letters_grid[ligne_compteur][colonne_compteur].isProposed = 0;
                                    }
                                }
                                caseChoose_x = -1;
                                caseChoose_y = -1;
                                lettersSelectedLength = 0;
                                SDL_FreeSurface(wordLabel); /* On supprime la surface précédente */
                                wordLabel = TTF_RenderText_Blended(police, word, couleurBlanche);
                                reset_lettersSelected_tab();
                                reset_lettersGrid();

                }
            }
            case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_p:
                    if(isPause == 0)
                    isPause = 1;
                    else
                    {
                        isPause = 0;
                    }
                    break;
            }
            break;

        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 130, 0, 255));
        SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
        for(k=0,i=0;i<ecran->w;i+=height,k++)
        {
        r.y = i/3 + 140;
        for(j=0,l=0;j<(ecran->h*1.5);j+=weight,l++)
            {
                r.x = j/4 + 195;
                if(letters_grid[k][l].isChoose==1)
                {
                    SDL_BlitSurface(caseSelected, NULL, ecran,&r);
                }
                else
                {
                    if(letters_grid[k][l].isProposed==1)
                {
                    SDL_BlitSurface(caseProposed, NULL, ecran,&r);
                }
                else
                {
                    SDL_BlitSurface(caseGrid, NULL, ecran,&r);
                }
                }
                positionCaractere = r;
                if(letters_grid[k][l].letter.letter_alphabetOrder == 2 || letters_grid[k][l].letter.letter_alphabetOrder == 4 || letters_grid[k][l].letter.letter_alphabetOrder == 5 || letters_grid[k][l].letter.letter_alphabetOrder == 6 || letters_grid[k][l].letter.letter_alphabetOrder == 8 || letters_grid[k][l].letter.letter_alphabetOrder == 11 || letters_grid[k][l].letter.letter_alphabetOrder == 12 || letters_grid[k][l].letter.letter_alphabetOrder == 14 || letters_grid[k][l].letter.letter_alphabetOrder == 16 || letters_grid[k][l].letter.letter_alphabetOrder == 18 || letters_grid[k][l].letter.letter_alphabetOrder == 19)
                {
                    positionCaractere.x += 39;
                    positionCaractere.y += 38;
                }
                else if(letters_grid[k][l].letter.letter_alphabetOrder == 9)
                {
                    positionCaractere.x += 42;
                    positionCaractere.y += 38;
                }
                else if(letters_grid[k][l].letter.letter_alphabetOrder == 23)
                {
                    positionCaractere.x += 31;
                    positionCaractere.y += 40;
                }
                else
                {
                    positionCaractere.x += 36;
                    positionCaractere.y += 38;
                }

                letters_grid[k][l].position_x = r.x;
                letters_grid[k][l].position_y = r.y;
                SDL_BlitSurface(caracteres[k][l], NULL, ecran, &positionCaractere); /* Blit du texte */
                if(letters_grid[k][l].letter.letter_points == 10)
                positionPoints.x = letters_grid[k][l].position_x + 58;
                else
                positionPoints.x = letters_grid[k][l].position_x + 61;
                positionPoints.y = letters_grid[k][l].position_y + 28;
                SDL_BlitSurface(points[k][l], NULL, ecran, &positionPoints); /* Blit des points */
                if(letters_grid[k][l].letter_bonus == DOUBLE_LETTER)
                {
                    positionBonus.x = letters_grid[k][l].position_x + 23;
                    positionBonus.y = letters_grid[k][l].position_y + 20;
                    SDL_BlitSurface(double_letter_icon, NULL, ecran, &positionBonus);
                }
                if(letters_grid[k][l].letter_bonus == DOUBLE_WORD)
                {
                    positionBonus.x = letters_grid[k][l].position_x + 23;
                    positionBonus.y = letters_grid[k][l].position_y + 20;
                    SDL_BlitSurface(double_word_icon, NULL, ecran, &positionBonus);
                }
                if(letters_grid[k][l].letter_bonus == TRIPLE_LETTER)
                {
                    positionBonus.x = letters_grid[k][l].position_x + 23;
                    positionBonus.y = letters_grid[k][l].position_y + 20;
                    SDL_BlitSurface(triple_letter_icon, NULL, ecran, &positionBonus);
                }
                if(letters_grid[k][l].letter_bonus == TRIPLE_WORD)
                {
                    positionBonus.x = letters_grid[k][l].position_x + 23;
                    positionBonus.y = letters_grid[k][l].position_y + 20;
                    SDL_BlitSurface(triple_word_icon, NULL, ecran, &positionBonus);
                }
            }
        }
        for(int indexWord = 0; indexWord < numberWordsFind ; indexWord++,positionWordsUsed.y+=15)
        {
            if(indexWord/25 == 0)
            {
                positionWordsUsed.x = 20;
                wordsUsedLabel = TTF_RenderText_Blended(policePoints, wordsUsed_tab[indexWord], couleurBlanche);
                SDL_BlitSurface(wordsUsedLabel, NULL, ecran, &positionWordsUsed);
            }
            else
            {
                if(indexWord == 25)
                {
                    positionWordsUsed.y = 15;
                    positionWordsUsed.x = 120;
                }
                wordsUsedLabel = TTF_RenderText_Blended(policePoints, wordsUsed_tab[indexWord], couleurBlanche);
                SDL_BlitSurface(wordsUsedLabel, NULL, ecran, &positionWordsUsed);
            }
        }
        positionWordsUsed.y = 15;
        SDL_BlitSurface(timer_icon, NULL, ecran, &positionTimerIcon);
        SDL_BlitSurface(wordLabel, NULL, ecran, &positionWord);
        SDL_BlitSurface(titre, NULL, ecran, &positionTitre);
        SDL_BlitSurface(timer, NULL, ecran, &positionTimer);
        SDL_BlitSurface(scoreLabel, NULL, ecran, &positionScore);
        SDL_BlitSurface(double_letter_icon, NULL, ecran, &positionDoubleLetterIcon);
        SDL_BlitSurface(double_letter_legend_label, NULL, ecran, &positionDoubleLetterLegende);
        SDL_BlitSurface(double_word_icon, NULL, ecran, &positionDoubleWordIcon);
        SDL_BlitSurface(double_word_legend_label, NULL, ecran, &positionDoubleWordLegende);
        SDL_BlitSurface(triple_letter_icon, NULL, ecran, &positionTripleLetterIcon);
        SDL_BlitSurface(triple_letter_legend_label, NULL, ecran, &positionTripleLetterLegende);
        SDL_BlitSurface(triple_word_icon, NULL, ecran, &positionTripleWordIcon);
        SDL_BlitSurface(triple_word_legend_label, NULL, ecran, &positionTripleWordLegende);
        SDL_Flip(ecran); // Mise à jour de l'écran
    }

    SDL_FreeSurface(timer);
    SDL_FreeSurface(scoreLabel);
    SDL_FreeSurface(titre);
    SDL_FreeSurface(ecran);
    TTF_Quit();
    SDL_Quit();


    return EXIT_SUCCESS;
}

int compAlphaOrder(char *chaine_un,char *chaine_deux)
{
    int i;

    for(i=0; chaine_un[i] != '\0' || chaine_deux[i] != '\0'; i++)
    {
        if (chaine_un[i] < chaine_deux[i])
           return -1;
        if (chaine_un[i] > chaine_deux[i])
           return 1;
    }
    return 0;
}

void initWordsDico_tab(char filename[])
{
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      char line [ 30 ];
      int compteur = 0;
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
        int compteur_line = 0;
        while ( compteur_line < 30 )
        {
            wordsDico_tab[compteur][compteur_line]=line[compteur_line];
            compteur_line++;
        }
         compteur++;
      }
      fclose ( file );
   }
   else
   {
      perror ( filename );
   }
}

// Initialisationd des index

void initIndexDico_tab()
{
    indexDico_tab[0]=1;
    indexDico_tab[1]=23159;
    indexDico_tab[2]=37500;
    indexDico_tab[3]=69640;
    indexDico_tab[4]=108208;
    indexDico_tab[5]=141347;
    indexDico_tab[6]=153299;
    indexDico_tab[7]=162998;
    indexDico_tab[8]=168537;
    indexDico_tab[9]=179843;
    indexDico_tab[10]=182159;
    indexDico_tab[11]=182641;
    indexDico_tab[12]=189331;
    indexDico_tab[13]=204841;
    indexDico_tab[14]=208391;
    indexDico_tab[15]=213551;
    indexDico_tab[16]=239214;
    indexDico_tab[17]=240245;
    indexDico_tab[18]=274203;
    indexDico_tab[19]=294773;
    indexDico_tab[20]=309898;
    indexDico_tab[21]=310840;
    indexDico_tab[22]=317738;
    indexDico_tab[23]=317891;
    indexDico_tab[24]=317954;
    indexDico_tab[25]=318043;
}

// Initialisationd de l'alphabet

void initAlphabet_tab(){
    alphabet_tab[0]=letter_a;
    alphabet_tab[1]=letter_b;
    alphabet_tab[2]=letter_c;
    alphabet_tab[3]=letter_d;
    alphabet_tab[4]=letter_e;
    alphabet_tab[5]=letter_f;
    alphabet_tab[6]=letter_g;
    alphabet_tab[7]=letter_h;
    alphabet_tab[8]=letter_i;
    alphabet_tab[9]=letter_j;
    alphabet_tab[10]=letter_k;
    alphabet_tab[11]=letter_l;
    alphabet_tab[12]=letter_m;
    alphabet_tab[13]=letter_n;
    alphabet_tab[14]=letter_o;
    alphabet_tab[15]=letter_p;
    alphabet_tab[16]=letter_q;
    alphabet_tab[17]=letter_r;
    alphabet_tab[18]=letter_s;
    alphabet_tab[19]=letter_t;
    alphabet_tab[20]=letter_u;
    alphabet_tab[21]=letter_v;
    alphabet_tab[22]=letter_w;
    alphabet_tab[23]=letter_x;
    alphabet_tab[24]=letter_y;
    alphabet_tab[25]=letter_z;
}

void initWordsPossiblesBis(int ligne, int colonne)
{
    grid_struct wordPossible[16];
    for(int compteur = 0; compteur < 16 ; compteur++)
    {
        grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
        wordPossible[compteur] = temp;
    }
    wordPossible[0] = letters_grid[ligne][colonne];
    if(wordIsPossible(wordPossible))
    {
        if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
            {
                for(int compteur = 0 ; compteur < 16 ; compteur++)
                {
                    wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                }
                numberWordsPossibles++;
            }
        if(colonne < 3)
            initWordsPossiblesBis(ligne,colonne+1);
        if(ligne < 3)
            initWordsPossiblesBis(ligne+1,colonne);
        if(colonne > 0)
            initWordsPossiblesBis(ligne,colonne-1);
        if(ligne > 0)
            initWordsPossiblesBis(ligne-1,colonne);
    }
    else
    {
        if(colonne < 3)
            initWordsPossiblesBis(ligne,colonne+1);
        if(ligne < 3)
            initWordsPossiblesBis(ligne+1,colonne);
        if(colonne > 0)
            initWordsPossiblesBis(ligne,colonne-1);
        if(ligne > 0)
            initWordsPossiblesBis(ligne-1,colonne);
    }
}

void initWordsPossibles()
{
    grid_struct wordPossible[16];
    for(int compteur = 0; compteur < 16 ; compteur++)
    {
        grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
        wordPossible[compteur] = temp;
    }

    for(int ligne = 0 ; ligne < 4 ; ligne++)
    {
        for(int colonne = 0 ; colonne < 4 ; colonne++)
        {

            // Test droit

            for(int testdroit = 0; testdroit < 4 && (testdroit+colonne) < 4; testdroit++)
            {
                wordPossible[testdroit] = letters_grid[ligne][colonne+testdroit];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test bas droit

            for(int testbasdroit = 0; testbasdroit < 4 && (testbasdroit+colonne) < 4 && (testbasdroit+ligne) < 4; testbasdroit++)
            {
                wordPossible[testbasdroit] = letters_grid[ligne+testbasdroit][colonne+testbasdroit];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test bas

            for(int testbas = 0; testbas < 4 && (testbas+ligne) < 4; testbas++)
            {
                wordPossible[testbas] = letters_grid[ligne+testbas][colonne];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test bas gauche

            for(int testbasgauche = 0; testbasgauche < 4 && (colonne-testbasgauche) > -1 && (testbasgauche+ligne) < 4; testbasgauche++)
            {
                wordPossible[testbasgauche] = letters_grid[testbasgauche+ligne][colonne-testbasgauche];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test gauche

            for(int testgauche = 0; testgauche < 4 && (colonne-testgauche) > -1 ; testgauche++)
            {
                wordPossible[testgauche] = letters_grid[ligne][colonne-testgauche];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test haut gauche

            for(int testhautgauche = 0; testhautgauche < 4 && (colonne-testhautgauche) > -1 && (ligne-testhautgauche) > -1; testhautgauche++)
            {
                wordPossible[testhautgauche] = letters_grid[ligne-testhautgauche][colonne-testhautgauche];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test haut

            for(int testhaut = 0; testhaut < 4 && (ligne-testhaut) > -1; testhaut++)
            {
                wordPossible[testhaut] = letters_grid[ligne-testhaut][colonne];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }

            // Test haut droit

            for(int testhautdroit = 0; testhautdroit < 4 && (ligne-testhautdroit) > -1 && (testhautdroit+colonne) < 4; testhautdroit++)
            {
                wordPossible[testhautdroit] = letters_grid[ligne-testhautdroit][testhautdroit+colonne];
                if(wordIsExistBis(wordPossible)&&!wordAlreadyFind(wordPossible))
                    {
                        for(int compteur = 0 ; compteur < 16 ; compteur++)
                        {
                            wordsPossibles[numberWordsPossibles][compteur] = wordPossible[compteur];
                        }
                        numberWordsPossibles++;
                    }
            }
            for(int compteur = 0; compteur < 16 ; compteur++)
                {
                    grid_struct temp = {0,0,letter_empty,NO_BONUS,0,0};
                    wordPossible[compteur] = temp;
                }
        }
    }
}

void displayWordsPossibles()
{
    for(int ligne = 0 ; ligne < numberWordsPossibles ; ligne++)
    {
        for(int colonne = 0 ; colonne < 16 ; colonne++)
        {
            printf("%c",wordsPossibles[ligne][colonne].letter.letter_libelle);
        }
        printf("\n");
    }
}

bool wordAlreadyFind(grid_struct word[])
{
    bool isAlreadyFind = 0;
    int compteur = 0;
    while(compteur < numberWordsPossibles)
    {
        bool isFinish = 0;
        bool isSame = 1;
        int compteur_word = 0;
        while(compteur_word < 16 && isFinish == 0 && isSame == 1)
        {
            if(tolower(wordsPossibles[compteur][compteur_word].letter.letter_libelle)==tolower(word[compteur_word].letter.letter_libelle))
            {
                if(wordsPossibles[compteur][compteur_word+1].letter.letter_libelle == letter_empty.letter_libelle && word[compteur_word+1].letter.letter_alphabetOrder == 0)
                {
                    isFinish = 1;
                    isAlreadyFind = 1;
                }
            }
            else
            {
                isSame = 0;
            }
            compteur_word++;
        }
        compteur++;
    }
    return isAlreadyFind;
}

bool wordIsExistBis(grid_struct word[])
{
    bool isExist = 0;
    if(word[0].letter.letter_alphabetOrder!=0)
    {
    int compteur = 0;
    int indexStart = 0;
    int indexEnd = 318813;
    indexStart = indexDico_tab[word[0].letter.letter_alphabetOrder-1]-1;
    if(lettersSelected_tab[0].letter.letter_alphabetOrder < 26)
        {
            indexEnd = indexDico_tab[word[0].letter.letter_alphabetOrder]-1;
        }
    compteur = indexStart;
    while(compteur < indexEnd)
    {
        bool isFinish = 0;
        bool isSame = 1;
        int compteur_word = 0;
        while(compteur_word < 16 && isFinish == 0 && isSame == 1)
        {
            if(wordsDico_tab[compteur][compteur_word]==tolower(word[compteur_word].letter.letter_libelle))
            {
                if(wordsDico_tab[compteur][compteur_word+1] == wordsDico_tab[0][1] && word[compteur_word+1].letter.letter_alphabetOrder == 0)
                {
                    isFinish = 1;
                    isExist = 1;
                }
            }
            else
            {
                isSame = 0;
            }
            compteur_word++;
        }
        compteur++;
    }
    }
    return isExist;
}

// Regarde si le mot selectionné existe dans le dictionnaire

bool wordIsExist()
{
    bool isExist = 0;
    if(lettersSelected_tab[0].letter.letter_alphabetOrder!=0)
    {
    int compteur = 0;
    int indexStart = 0;
    int indexEnd = 318813;
    indexStart = indexDico_tab[lettersSelected_tab[0].letter.letter_alphabetOrder-1]-1;
    if(lettersSelected_tab[0].letter.letter_alphabetOrder < 26)
        {
            indexEnd = indexDico_tab[lettersSelected_tab[0].letter.letter_alphabetOrder]-1;
        }
    compteur = indexStart;
    while(compteur < indexEnd)
    {
        bool isFinish = 0;
        bool isSame = 1;
        int compteur_word = 0;
        while(compteur_word < 16 && isFinish == 0 && isSame == 1)
        {
            if(wordsDico_tab[compteur][compteur_word]==tolower(lettersSelected_tab[compteur_word].letter.letter_libelle))
            {
                if(wordsDico_tab[compteur][compteur_word+1] == wordsDico_tab[0][1] && lettersSelected_tab[compteur_word+1].letter.letter_alphabetOrder == 0)
                {
                    isFinish = 1;
                    isExist = 1;
                }
            }
            else
            {
                isSame = 0;
            }
            compteur_word++;
        }
        compteur++;
    }
    }
    return isExist;
}

bool wordIsPossible(grid_struct word[])
{
    bool isPossible = 0;
    if(word[0].letter.letter_alphabetOrder!=0)
    {
    int compteur = 0;
    int indexStart = 0;
    int indexEnd = 318813;
    indexStart = indexDico_tab[word[0].letter.letter_alphabetOrder-1]-1;
    if(lettersSelected_tab[0].letter.letter_alphabetOrder < 26)
        {
            indexEnd = indexDico_tab[word[0].letter.letter_alphabetOrder]-1;
        }
    compteur = indexStart;
    while(compteur < indexEnd)
    {
        bool isSame = 1;
        int compteur_word = 0;
        while(compteur_word < 16 && isSame == 1)
        {
            if(wordsDico_tab[compteur][compteur_word]==tolower(word[compteur_word].letter.letter_libelle))
            {
                    isPossible = 1;
            }
            else
            {
                isSame = 0;
            }
            compteur_word++;
        }
        compteur++;
    }
    }
    return isPossible;
}

bool wordUsedIsExist()
{
    bool isExist = 0;
    if(lettersSelected_tab[0].letter.letter_alphabetOrder!=0)
    {
    int indexEnd = 50;
    int compteur = 0;
    char chaineSelected[16] = {0};
    for(int ligne = 0 ; ligne < lettersSelectedLength ; ligne++)
    {
        char chaine_to_add = lettersSelected_tab[ligne].letter.letter_libelle;
        chaineSelected[ligne] = chaine_to_add;
    }
    while(compteur < indexEnd && isExist == 0)
    {
        int compteurletter = 0;
        int numberletters = 0;
        while(compteurletter < 16)
        {
            if(chaineSelected[compteurletter] == wordsUsed_tab[compteur][compteurletter])
            {
                numberletters++;
            }
            compteurletter++;
        }
        if(numberletters==16)
        {
            isExist=1;
        }
        compteur++;
    }
    }
    return isExist;
}

// Affichage de l'alphabet

void displayAlphabet_tab()
{
    int compteur = 0;

    printf("Affichage de l'alphabet : \n");

    while (compteur < 26)
    {
        printf("Libelle : %c Points : %d Voyelle : %d\n", alphabet_tab[compteur].letter_libelle,alphabet_tab[compteur].letter_points,alphabet_tab[compteur].isVoyelle);
        compteur++;
    }
}

// Affichage du mot selectionné

void displayLettersSelected_tab()
{
    int compteur = 0;

    printf("Affichage du mot selectionne : \n");

    while (compteur < 16)
    {
        printf("%c",lettersSelected_tab[compteur].letter.letter_libelle);
        compteur++;
    }
    printf("\n");
}

void displayWordsDico_tab()
{
    int compteur = 0;

    printf("Affichage du dico : \n");

    while (compteur < 318813)
    {
        int compteur_lettre = 0;
        while (compteur_lettre < 30)
        {
            printf("%c",wordsDico_tab[compteur][compteur_lettre]);
            compteur_lettre++;
        }
        printf("\n");printf("\n");
        compteur++;
    }
}

bool add_lettersSelected_tab(grid_struct letter_to_add)
{
    int compteur = 0;
    bool isAdd = 0;
    while(compteur < 16 && isAdd == 0)
    {
        if(lettersSelected_tab[compteur].isChoose!=1)
        {
            lettersSelected_tab[compteur] = letter_to_add;
            isAdd = 1;
        }
        compteur++;
    }
    return isAdd;
}

bool add_wordsUsed_tab(char word[])
{
    int compteur = 0;
    bool isAdd = 0;
    int place = 0;
    int lettre = 0;
    for(int compteurWord = 0; compteurWord < numberWordsFind ; compteurWord++)
    {
        if(compAlphaOrder(wordsUsed_tab[compteurWord],word) == -1)
        {
            place++;
        }
    }
    if(place != numberWordsFind)
    {
        for(int compteurWord = numberWordsFind ; compteurWord > place ; compteurWord--)
        {
            while(lettre < 16)
            {
                wordsUsed_tab[compteurWord][lettre] = wordsUsed_tab[compteurWord-1][lettre];
                lettre++;
            }
            lettre = 0;
        }
    }
    while(compteur < 16)
    {
        wordsUsed_tab[place][compteur] = word[compteur];
        compteur++;
    }
    isAdd = 1;
    return isAdd;
}

void reset_lettersSelected_tab()
{
    int compteur = 0;
    grid_struct letterGrid_empty = {0,0,letter_empty,NO_BONUS,0,0};
    while(compteur < 16)
    {
        lettersSelected_tab[compteur] = letterGrid_empty;
        compteur++;
    }
}

int calcul_score_word()
{
    int score = 0;
    int multiplicateur_word = 1;
    int compteur = 0;
    while(compteur < 16)
    {
        if(lettersSelected_tab[compteur].letter_bonus == DOUBLE_WORD)
        {
            multiplicateur_word *= 2;
        }
        if(lettersSelected_tab[compteur].letter_bonus == TRIPLE_WORD)
        {
            multiplicateur_word *= 3;
        }
        compteur++;
    }
    compteur = 0;
    while(compteur < 16)
    {
        int multiplicateur_letter = 1;
        if(lettersSelected_tab[compteur].letter_bonus == DOUBLE_LETTER)
        {
            multiplicateur_letter = 2;
        }
        if(lettersSelected_tab[compteur].letter_bonus == TRIPLE_LETTER)
        {
            multiplicateur_letter = 3;
        }
        score += lettersSelected_tab[compteur].letter.letter_points * multiplicateur_letter;
        compteur++;
    }
    score *= multiplicateur_word;
    if(lettersSelectedLength == 5)
    {
        score += 5;
    }
    if(lettersSelectedLength == 6)
    {
        score += 10;
    }
    if(lettersSelectedLength == 7)
    {
        score += 15;
    }
    if(lettersSelectedLength == 8)
    {
        score += 20;
    }
    if(lettersSelectedLength >= 9)
    {
        score += 25;
    }
    return score;
}

void display_score_word()
{
    printf("%d",calcul_score_word());
}

void initGrid(char grid[16][2])
{
    for(int compteur = 0; compteur < 16; compteur++)
    {
        int random = 0 + (rand () % (25+1));
        int random_bonus = 0 + (rand () % (12+1));
        if(alphabet_tab[random].isVoyelle == 0)
        {
            if(alphabet_tab[random].isVoyelle == 0)
            {
            random = 0 + (rand () % (25+1));
            }
        }
        if(alphabet_tab[random].letter_points == 8)
        {
            if(alphabet_tab[random].letter_points == 8)
            {
                random = 0 + (rand () % (25+1));
            }
        }
        bonus_enum bonus = NO_BONUS;
        if(random_bonus == 1)
        {
            bonus = DOUBLE_LETTER;
        }
        if(random_bonus == 2)
        {
            bonus = DOUBLE_WORD;
        }
        if(random_bonus == 3)
        {
            bonus = TRIPLE_LETTER;
        }
        if(random_bonus == 4)
        {
            bonus = TRIPLE_WORD;
        }
        grid_struct letter_random = {0,0,alphabet_tab[random],bonus,0,0};
        letters_grid[compteur/4][compteur%4] = letter_random;
        grid[compteur][0] = alphabet_tab[random].letter_libelle;
    }
}

int researchAlphabetLetter(char caractere)
{
    for(int compteur = 0 ; compteur < 26 ; compteur++)
    {
        if(alphabet_tab[compteur].letter_libelle == toupper(caractere))
            return compteur;
    }
}

void initGridFile(char nameFile[],char grid[16][2])
{
    FILE* fichier = NULL;
    fichier = fopen(nameFile, "r");

    if (fichier != NULL)
    {
        char line [100];
        int compteur = 0;
        while ( fgets ( line, sizeof line, fichier ) != NULL )
        {
        grid_struct letter_temp_1 = {0,0,alphabet_tab[researchAlphabetLetter(line[0])],NO_BONUS,0,0};
        letters_grid[compteur][0] = letter_temp_1;
        grid[compteur*4][0] = toupper(line[0]);
        grid_struct letter_temp_2 = {0,0,alphabet_tab[researchAlphabetLetter(line[1])],NO_BONUS,0,0};
        letters_grid[compteur][1] = letter_temp_2;
        grid[compteur*4+1][0] = toupper(line[1]);
        grid_struct letter_temp_3 = {0,0,alphabet_tab[researchAlphabetLetter(line[2])],NO_BONUS,0,0};
        letters_grid[compteur][2] = letter_temp_3;
        grid[compteur*4+2][0] = toupper(line[2]);
        grid_struct letter_temp_4 = {0,0,alphabet_tab[researchAlphabetLetter(line[3])],NO_BONUS,0,0};
        letters_grid[compteur][3] = letter_temp_4;
        grid[compteur*4+3][0] = toupper(line[3]);
        compteur++;
        }

        fclose(fichier);
        letters_grid[0][1].letter_bonus = DOUBLE_WORD;
        letters_grid[0][2].letter_bonus = TRIPLE_LETTER;
        letters_grid[0][3].letter_bonus = DOUBLE_WORD;
        letters_grid[2][2].letter_bonus = TRIPLE_LETTER;
        letters_grid[3][1].letter_bonus = DOUBLE_LETTER;
        letters_grid[3][2].letter_bonus = TRIPLE_WORD;

    }
    else
    {
        printf("Impossible d'ouvrir le fichier %s",nameFile);
    }
}

void reset_lettersGrid()
{
    for(int ligne = 0;ligne < 4;ligne++)
        {
            for(int colonne = 0;colonne < 4;colonne++)
                {
                    letters_grid[ligne][colonne].isChoose = 0;
                }
        }
}

void init()
{
    initAlphabet_tab();
    initWordsDico_tab("dico.txt");
    initIndexDico_tab();
    reset_lettersSelected_tab();
}
