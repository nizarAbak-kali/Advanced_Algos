#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>
#include "bibosdl.h"

/* Declaration globale */
#define LARGEUR 800
#define HAUTEUR 600
#define DENSITE 0.65
#define PAS 4

Uint32 a[LARGEUR][HAUTEUR];
Uint32 na[LARGEUR][HAUTEUR];
int nombrearbresenfeu;

void arbre (SDL_Surface *ecran,int x,int y,Uint32 c)
{
  int i,j;
  for(i=PAS*x;i<PAS*(x+1);i++)
    for(j=PAS*y;j<PAS*(y+1);j++){
      if (i < LARGEUR-1 && j < HAUTEUR-1)
	putpixel(ecran,i,j,c);
    }
}

void foret (SDL_Surface *ecran)
{
  int i,j,hasard;
  Uint32 vert=SDL_MapRGB(ecran->format,0,100,0);
  srand(time(NULL));

  for(i=1;i<LARGEUR-1;i++)
    for(j=1;j<HAUTEUR-1;j++)
      {
	hasard=rand()%1000;
	if(hasard<(int)(DENSITE*1000)){
	  a[i][j]=vert;
	  arbre(ecran,i,j,vert);
	}
      }
}

void miseafeu (SDL_Surface *ecran)
{
  int i,j,nombrearbresenfeu=0;
  Uint32 vert=SDL_MapRGB(ecran->format,0,100,0);
  Uint32 rouge=SDL_MapRGB(ecran->format,250,0,0);
  
  /* Mise à feu de quelques arbres */
  /* Aléatoirement */
  /*
  while(nombrearbresenfeu !=4000){
    i=(rand()%LARGEUR-2)+1;
    j=(rand()%HAUTEUR-2)+1;
    
    if(a[i][j]==vert){
      a[i][j]=rouge;
      arbre(ecran,i,j,rouge);
      nombrearbresenfeu++;
    }
  }
  */
  
  /* Sur la première colonne */
  
  for(j=1;j<HAUTEUR-1;j++)
    if(a[1][j]==vert){
      a[1][j]=rouge;
      arbre(ecran,1,j,rouge);
      nombrearbresenfeu++;
    }
  

  /* Copie du tableau a dans le tableau na */
  for(i=1;i<LARGEUR-1;i++)
    for(j=1;j<HAUTEUR-1;j++)
      na[i][j]=a[i][j];
}

/* Propagation du feu dans la foret */

void propagation (SDL_Surface *ecran)
{
  int i,j;
  Uint32 vert=SDL_MapRGB(ecran->format,0,100,0);
  Uint32 rouge=SDL_MapRGB(ecran->format,250,0,0);
  Uint32 gris=SDL_MapRGB(ecran->format,50,50,50);
  
  for(i=1;i<LARGEUR-1;i++){
    for(j=1;j<HAUTEUR-1;j++){
      if (a[i][j]==vert)
	{
	  if(a[i][j+1]==rouge){
	    na[i][j]=rouge;
	    nombrearbresenfeu++;
	  }
	  else if(a[i][j-1]==rouge){
	    na[i][j]=rouge;
	    nombrearbresenfeu++;
	  }
	  else if (a[i-1][j]==rouge){
	    na[i][j]=rouge;
	    nombrearbresenfeu++;
	  }
	  else if (a[i+1][j]==rouge){
	    na[i][j]=rouge;
	    nombrearbresenfeu++;
	  }
	}
      else if(a[i][j]==rouge){
	na[i][j]=gris;
	nombrearbresenfeu--;
      }
    }
  }
  
  /* Application à l'écran */
  for(i=1;i<LARGEUR-1;i++)
    for(j=1;j<HAUTEUR-1;j++)
      if(na[i][j] != a[i][j]){
	a[i][j]=na[i][j];
	arbre(ecran,i,j,a[i][j]);
      }
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  
  SDL_Surface *ecran=NULL;
  SDL_Event event;
  int continuer=1;
  
  ecran=SDL_SetVideoMode(800,600,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Feu de Foret",NULL);
  SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
  foret(ecran);
  miseafeu(ecran);
 
  while(continuer)
    {
      SDL_PollEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  continuer=0;
	  break;
	  
	case SDLK_ESCAPE:
	  continuer=0;
	  break;
	}
      /* Propagation du feu */
      SDL_Flip(ecran);
      SDL_Delay(30);
      propagation(ecran);
      SDL_Flip(ecran);
    }
  SDL_Quit();
  return 0;
}
