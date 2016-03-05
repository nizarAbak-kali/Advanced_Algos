/*
Algorithmique Avancée L3 - 2013/2014
Nom : Nouira
Prénom : Chafik
N° étudiant : 11291558
Programme : Morphogenèse - Séance du 27 septembre 2013
*/

#include <SDL/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define L 300
#define CA 0
#define CN 1

SDL_Surface * screen;
Uint32 couleur[100];
int c[L][L];
int nc[L][L];


void pause(void) {
  SDL_Event evenement;
  do SDL_WaitEvent(&evenement);
  while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}

void putpixel(int xe, int ye, Uint32 c) {
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;
  *numerocase=c;
}

Uint32 getpixel(int xe, int ye) { 
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;
  return (*numerocase);
}

void filldisc(int xo, int yo, int R, Uint32 c) {
  int x, y, F, F1, F2,newx,newy,xx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (x,2*yo-y, c);
  while( y>yo) {
    F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
    if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
    else {y-=1; F=F2;}
    newx=2*xo-x ; newy=2*yo-y ;
    for(xx=newx; xx<=x; xx++)
      if(xx<800 && xx>=0 && y>=0 && y<600 )
	putpixel(xx,y,c);
    for(xx=newx; xx<=x; xx++)
      if(xx<800 && xx>=0 && newy>=0 && newy<600 )
	putpixel(xx,newy,c);
  }
  if(xo+R<800 && xo+R>=0&& y>=0 && y<600) putpixel(xo+R,yo,c);
  if(xo-R<800 && xo-R>=0&& y>=0 && y<600) putpixel(xo-R,yo, c);
}

void circle( int xo, int yo, int R, Uint32 c) {
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel(x,2*yo-y, c);
  while( y>yo) {
    F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
    if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
    else {y-=1; F=F2;}
    if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
    newx=2*xo-x ; newy=2*yo-y ;
    if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(x, newy,c);
    if (newx<800 && newx>=0 && y>=0 && y<600) putpixel(newx,y,c);
    if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(newx,newy, c);
  }
  if (xo+R<800 && xo+R>=0) putpixel(xo+R,yo,c);
  if (xo-R<800 && xo-R>=0) putpixel(xo-R,yo, c);
}


int main(){ 
  int R1=2, R2=6, hasard, i, j, dx, dy, vx, vy, etape;
  float w, wmoin=0.2;
  srand(time(NULL));

  SDL_Init(SDL_INIT_VIDEO);
  screen=SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  couleur[0]=SDL_MapRGB(screen->format, 255, 0, 0); //Rouge
  couleur[1]=SDL_MapRGB(screen->format, 255, 255, 0); //Jaune 
  couleur[2]=SDL_MapRGB(screen->format, 255, 255, 255); //Blanc
  
  SDL_FillRect(screen, 0, couleur[2]);
  
  for(i=0;i<L;i++)  
    for(j=0;j<L;j++){
      hasard=rand()%2;
      if(hasard==0) c[i][j]=CA;
      else c[i][j]=CN;
      putpixel(i,j,couleur[c[i][j]]);
    }
  SDL_Flip(screen);
  
  for(etape=1;etape<=10;etape++){
    for(i=0;i<L;i++){  
      for(j=0;j<L;j++){
	w=0.;
	for(dx=-R2;dx<=R2;dx++)
	  for(dy=-R2;dy<=R2;dy++){
	    vx=(i+dx+L)%L;
	    vy=(j+dy+L)%L;
	    if(c[vx][vy]==CA){
	      if(abs(dx)<=R1 && abs(dy)<=R1) w+=1.;
	      else w-=wmoin;
	    }
	  }
	if(w>0.) nc[i][j]=CA; //Rouge
	else if(w<0.) nc[i][j]=CN; //Jaune
	else nc[i][j]=c[i][j];
      } 
    }
    
    for(i=0;i<L;i++) 
      for(j=0;j<L;j++){
	c[i][j]=nc[i][j];
	putpixel(i,j,couleur[c[i][j]]);
      }
    SDL_Flip(screen);
  }
  
  pause();
  SDL_Quit();
  return 0;
}
