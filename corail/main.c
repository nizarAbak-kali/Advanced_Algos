#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define FENETRE_LARGEUR 800 //largeur de la fenêtre
#define FENETRE_HAUTEUR 600 //hauteur de la fenêtre
//les pixels vont de [0][0] à [LARGEUR-1][HAUTEUR-1]
#define RAYON 280

Uint32 couleur[100];

//prototypes
void putpixel(SDL_Surface* surface, int x, int y, Uint32 couleur); //modifier la couleur d'un pixel
Uint32 getpixel(SDL_Surface* surface, int x, int y); //connaitre la couleur d'un pixel
void filldisc(SDL_Surface* surface, int xo, int yo, int R, Uint32 couleur); //créer un disque
void corail(SDL_Window* fenetre, SDL_Surface* surface, int xorig , int yorig , int R1, Uint32 couleur); //formation du corail




int main(int argc, char **argv)
{
    //écran et surface
    SDL_Window* fenetre = NULL; //la fenêtre sur laquelle est faite le rendu
    SDL_Surface* ecran = NULL;  //la surface contenue par la fenêtre
    //gestion des évènements
    SDL_Event evenement;
    int continuer = 1;
    //données
    int R1 = 40;     //rayon du germe
    int xorig = FENETRE_LARGEUR/2; //coordonnée X du germe
    int yorig = FENETRE_HAUTEUR/2; //coordonnée Y du germe

    srand(time(NULL));


    //démarrage de la SDL
    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //création de la fenêtre
    fenetre = SDL_CreateWindow("Corail", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, FENETRE_LARGEUR, FENETRE_HAUTEUR, SDL_WINDOW_SHOWN);
    if(!fenetre)
    {
        fprintf(stderr, "SDL Error : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    else //création de la surface ecran
    {
        ecran = SDL_GetWindowSurface(fenetre);
    }

    //couleurs
    couleur[0] = SDL_MapRGB(ecran->format, 255, 255, 255); //blanc
    couleur[1] = SDL_MapRGB(ecran->format, 0, 0, 255);     //bleu
    couleur[2] = SDL_MapRGB(ecran->format, 0, 0, 0);       //noir
    couleur[3] = SDL_MapRGB(ecran->format, 255, 0, 0);     //rouge
    couleur[4] = SDL_MapRGB(ecran->format, 0, 255, 0);     //vert

    //coloration de la surface ecran en noir
    SDL_FillRect(ecran, NULL, couleur[2]);
    //Réafficher l'écran
    SDL_UpdateWindowSurface(fenetre);

    //remplissage du disque en blanc (le germe)
    filldisc(ecran, xorig, yorig, 3, couleur[0]);
    //Réafficher l'écran
    SDL_UpdateWindowSurface(fenetre);

    //formation du corail en blanc
    corail(fenetre, ecran, xorig, yorig, R1, couleur[0]);
    //Réafficher l'écran
    SDL_UpdateWindowSurface(fenetre);
    puts("Fin de la creation du corail\n");

    //Boucle d'attente des évènements
    while(continuer)
    {
        SDL_WaitEvent(&evenement);

        //clic sur la croix en haut à droite de la fenêtre
        if(evenement.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            continuer = 0; //quitter le programme
        }
    }


    //Quitter SDL
    SDL_DestroyWindow(fenetre);
    SDL_FreeSurface(ecran);
    SDL_Quit();

    return 0;
}




//modifier la couleur d'un pixel
void putpixel(SDL_Surface* surface, int x, int y, Uint32 couleur)
{
    //si le pixel ne dépasse pas de la surface
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
        *((Uint32*)(surface->pixels) + x + y * surface->w) = couleur;
}


//connaitre la couleur d'un pixel
Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    //si le pixel ne dépasse pas de la surface
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
        return *((Uint32*)(surface->pixels) + x + y * surface->w);
    else
        return 0;
}


//créer un disque
void filldisc(SDL_Surface* surface, int xo, int yo, int R, Uint32 couleur)
{
    int x, y;
    int newx, newy, xx;
    int F, F1, F2;

    x = xo;
    y = yo + R;
    F = 0;

    if (x < 800 && x >= 0 && y >= 0 && y < 600)
        putpixel(surface, x, y, couleur);
    if (x < 800 && x >= 0 && 2*yo-y >= 0 && 2*yo-y < 600)
        putpixel (surface, x, 2*yo-y, couleur);

    while(y > yo)
    {
        F1 = F+2 * (x-xo) + 1;
        F2 = F-2 * (y-yo) + 1;
        if (abs(F1) < abs(F2))
        {
            x += 1;
            F = F1;
        }
        else
        {
            y -= 1;
            F = F2;
        }
        newx = 2*xo - x;
        newy = 2*yo - y;
        for(xx = newx; xx <= x; xx++)
            if (xx < 800 && xx >= 0 && y >= 0 && y < 600)
                putpixel(surface, xx, y, couleur);
        for(xx = newx; xx <= x; xx++)
            if (xx < 800 && xx >= 0 && newy >= 0 && newy < 600)
                putpixel(surface, xx, newy, couleur);
    }
    if (xo+R < 800 && xo+R >= 0 && y >= 0 && y < 600)
        putpixel(surface, xo+R, yo, couleur);
    if (xo-R < 800 && xo-R >= 0 && y >= 0 && y < 600)
        putpixel(surface, xo-R, yo, couleur);
}


//formation du corail
void corail(SDL_Window* fenetre, SDL_Surface* surface, int xorig , int yorig , int R1, Uint32 couleur)
{
    int x, y;
    int oldx, oldy;
    int hasard;
    int d2; //distance entre la particule et le centre du germe au carré
    int R2; //rayon du cercle
    int R;  //rayon entre la particule et le centre du germe au départ
    float angled, angler; //angle en degrés, angle en radians

    while(R1 < RAYON)
    {
        R2 = R1 + 20;

        //on lance une particule au hasard dans la zone entre R1 et R2
        R = R1 + rand()%(R2 - R1); //entre 0 et R2 - R1 - 1

        angled = rand()%360;
        angler = M_PI * angled / 180;

        x = xorig + R * cos(angler); //coordonnée x de départ de la particule
        y = yorig + R * sin(angler); //coordonnée y de départ de la particule

        for(;;) //mouvements de la particule
        {
            oldx = x;
            oldy = y;

            //la particule se déplace dans une direction aléatoire en fonction de la valeur de hasard
            hasard = rand()%4; //entre 0 et 3

            if(hasard == 0)
                x++;
            else if(hasard == 1)
                y++;
            else if(hasard == 2)
                x--;
            else if (hasard == 3)
                y--;

            d2 = (x - xorig) * (x - xorig) + (y - yorig) * (y - yorig);

            if(d2 > R2 * R2) //la particule est sortie du cercle de rayon R2
                break;
            if (getpixel(surface, x, y) == couleur) //la particule a atteint un point du germe
            {
                if(d2 > ((R1 - 5) * (R1 - 5 ))) //la distance au carré est proche de la zone de lancement
                    R1 += 20; //on prend une nouvelle zone de lancement

                putpixel(surface, oldx, oldy, couleur); //la particule se colle au germe
                //Réafficher l'écran
                SDL_UpdateWindowSurface(fenetre); //enlever pour un affichage instantané du corail
                break;
            }
        }
    }
}
