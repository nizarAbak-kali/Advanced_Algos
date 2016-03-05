/*
Algorithmique Avancée L3 - 2013/2014
Nom :ABAK-KALI
Prénom :Nizar
N° étudiant : 11290569
Programme : Désintégration - Séance du 04 octobre 2013
*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>

#define PAS 4

int X[1000], Y[1000];

//Place un pixel dans l'ecran
void putpixel(SDL_Surface *screen,int xe, int ye, Uint32 c) { 
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; 
  *numerocase=c;
}


//Donne la couleur d'un pixel
Uint32 getpixel(SDL_Surface *screen,int xe, int ye) {
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; return (*numerocase);
}

void filldisc(SDL_Surface *screen,int xo, int yo, int R, Uint32 c) {
  int x, y, F, F1, F2,newx,newy,xx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo) {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if (abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ; newy=2*yo-y ;
      for(xx=newx; xx<=x; xx++)if (xx<800 && xx>=0 && y>=0 && y<600 )
				 putpixel(screen,xx,y,c);
      for(xx=newx; xx<=x; xx++)if (xx<800 && xx>=0 && newy>=0 && newy<600 )
				 putpixel(screen,xx,newy,c);
  }
  if (xo+R<800 && xo+R>=0&& y>=0 && y<600) putpixel(screen,xo+R,yo,c);
  if (xo-R<800 && xo-R>=0&& y>=0 && y<600) putpixel(screen,xo-R,yo, c);
}



void carre(SDL_Surface* ecran,int x, int y, Uint32 c) {
  int i, j;
  for(i = x; i < x + PAS; i++)
    for(j = y; j < y + PAS; j++)
      putpixel(ecran,i,j,c);
}

void desintegration(SDL_Surface* ecran) {
  int i=0, x, y, h, etape, nb_points=0, nX[1000], nY[1000];
  Uint32 rouge=SDL_MapRGB(ecran->format,255,0,0);
  Uint32 blanc=SDL_MapRGB(ecran->format,255,255,255);
  Uint32 noir=SDL_MapRGB(ecran->format,0,0,0);

  filldisc(ecran, 300, 200, 100, rouge);
  for(x = 0; x < 800; x += PAS)
    for(y = 0; y < 600; y += PAS)
      if(getpixel(ecran,x,y) == rouge) {
	X[i] = x; Y[i] = y; i++;
      }

  nb_points = i;
  for(i = 0; i < nb_points; i++)
    carre(ecran,X[i],Y[i],noir);

  for(etape = 1; etape < 100; etape++)
    for(i = 0; i < nb_points; i++)
      h = rand()%4;
  if((h==0 && getpixel(ecran,X[i]-PAS,Y[i]) == noir) && (getpixel(ecran,X[i]+PAS,Y[i]) == blanc)) {
    nX[i] = X[i] + PAS;
    nY[i] = Y[i];
  }

  if((((h==1 || h==2 || h==3) && getpixel(ecran,X[i]-PAS,Y[i]) == blanc) && (getpixel(ecran,X[i]+PAS,Y[i]) == blanc))) {
    nX[i] = X[i] + (2+rand()%2)*PAS;
    nY[i] = Y[i];
  }
}


int main() {
  SDL_Surface *ecran=NULL;
  SDL_Event event;
  int continuer=1;

  SDL_Init(SDL_INIT_VIDEO);  
  ecran=SDL_SetVideoMode(600,800,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Désintegration",NULL);
  SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
  while(continuer) {
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_KEYDOWN :
      if(event.key.keysym.sym == SDLK_RETURN) continuer = 0;
      break;
    default : break;
    }
    desintegration(ecran);
    SDL_Flip(ecran);
  }
  SDL_Quit();
  return 0;
}
