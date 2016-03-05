/*
Algorithmique Avancée L3 - 2013/2014
Nom : Nouira
Prénom : Chafik
N° étudiant : 11291558
Programme : Cryptographie (Partage d'un secret)
Compilation : gcc -W -Wall cryptographie.c -lSDL -lSDL_image -lSDL_ttf
Exécution : ./a.out
*/
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "bibosdl.h"

#define L 180 //longueur du côté de la fenêtre
#define PAS 2 //nombre de pixels que l'on parcourt en un déplacement

int main() {
  SDL_Surface *ecran = NULL;
  SDL_Surface *image = NULL;
  SDL_Surface *texte = NULL;
  SDL_Rect positionImage = {0,0,0,0}, positionTexte = {0,250,0,0};
  TTF_Font *font = NULL;
  SDL_Color couleurtexte = {0,0,0,0};
  int i, j, hasard;
  Uint32 couleur[2];
  Uint32 tab[L][L]; //Matrice qui contiendra l'image brouillée n° 1
  Uint32 tab2[L][L]; //Matrice qui contiendra l'image brouillée n° 2
  
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  
  //Chargement de la police d'écriture
  font = TTF_OpenFont("font.ttf", 30);
  
  ecran = SDL_SetVideoMode(500,500,32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Cryptographie Graphique", NULL);

  //Initialisation des couleurs
  couleur[0] = SDL_MapRGB(ecran->format, 255, 255, 255); //Blanc
  couleur[1] = SDL_MapRGB(ecran->format, 0, 0, 0); //Noir
  
  SDL_FillRect(ecran, NULL, couleur[0]);
  texte = TTF_RenderText_Blended(font,"Image de départ", couleurtexte);
  image = IMG_Load("image.jpg"); //Chargement de l'image de départ
  SDL_BlitSurface(image, NULL, ecran, &positionImage); //Mise de l'image sur lécran
  SDL_BlitSurface(texte, NULL, ecran, &positionTexte); //Mise du texte sur l'écran
  SDL_Flip(ecran);
  pause();
  
  //Parcours de l'image de PAS pixels en PAS pixels
  for(i = 0; i < L; i += PAS) {
    for(j = 0; j < L; j += PAS) {
      hasard = rand()%2;
      if (hasard == 0) {
	if(getpixel(ecran, i, j) == couleur[0]) {
	  tab[i][j] = couleur[1]; tab[i+1][j] = couleur[0];
	  tab[i][j+1] = couleur[0]; tab[i+1][j+1] = couleur[1];
	  
	  tab2[i][j] = couleur[0]; tab2[i+1][j] = couleur[1];
	  tab2[i][j+1] = couleur[1]; tab2[i+1][j+1] = couleur[0];
	}
	
	else {
	  tab[i][j] = couleur[1]; tab[i+1][j] = couleur[0];
	  tab[i][j+1] = couleur[0]; tab[i+1][j+1] = couleur[1];
	  
	  tab2[i][j] = couleur[1]; tab2[i+1][j] = couleur[0];
	  tab2[i][j+1] = couleur[0]; tab2[i+1][j+1] = couleur[1];
	}
      }	
      if (hasard == 1) {
	if(getpixel(ecran, i, j) == couleur[0]) {
	  tab[i][j] = couleur[0]; tab[i+1][j] = couleur[1];
	  tab[i][j+1] = couleur[1]; tab[i+1][j+1] = couleur[0];
	  
	  tab2[i][j] = couleur[1]; tab2[i+1][j] = couleur[0];
	  tab2[i][j+1] = couleur[0]; tab2[i+1][j+1] = couleur[1];
	}	  
	else {
	  tab[i][j] = couleur[0]; tab[i+1][j] = couleur[1];
	  tab[i][j+1] = couleur[1]; tab[i+1][j+1] = couleur[0];
	  
	  tab2[i][j] = couleur[0]; tab2[i+1][j] = couleur[1];
	  tab2[i][j+1] = couleur[1]; tab2[i+1][j+1] = couleur[0];
	}
      }
    }
  }
  //Affichage de l'image brouillée n° 1
  texte = TTF_RenderText_Blended(font,"Image brouillée 1", couleurtexte);
  SDL_FillRect(ecran, NULL, couleur[0]);
  SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
  SDL_Flip(ecran);
  for(i = 0; i < L; i++) {
    for(j = 0; j < L; j++) {
      if(tab[i][j] == couleur[1])
	putpixel(ecran, i, j, couleur[1]);
    }
  }
  SDL_Flip(ecran);
  pause();
  
  //Affichage de l'image brouillée n° 2
  SDL_FillRect(ecran, NULL, couleur[0]);
  for(i = 0; i < L; i++) {
    for(j = 0; j < L; j++) {
      if(tab2[i][j] == couleur[1])
	putpixel(ecran, i, j, couleur[1]);
    }
  }
  texte = TTF_RenderText_Blended(font,"Image brouillée 2", couleurtexte);
  SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
  SDL_Flip(ecran);
  pause();
  
  SDL_FillRect(ecran, NULL, couleur[0]);
  for(i = 0; i < L; i += PAS) {
    for(j = 0; j < L; j += PAS) {
      if(tab[i][j] == tab2[i][j]) //Si la couleur est la même pour les deux tableaux
	putpixel(ecran, i, j, couleur[1]); //On colore le pixel en noir
      
      else if(tab[i][j] != tab2[i][j]) //Si la couleur n'est pas la même pour les deux tableaux
	putpixel(ecran, i, j, couleur[0]); //On colore le pixel en blanc
    }
  }
  texte = TTF_RenderText_Blended(font,"Résultat de comparaison", couleurtexte);
  SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
  SDL_Flip(ecran); 
  pause();  

  SDL_FreeSurface(ecran);
  SDL_FreeSurface(texte);
  SDL_FreeSurface(image);
  TTF_CloseFont(font);
  SDL_Quit();
  TTF_Quit();
  return EXIT_SUCCESS;
}
