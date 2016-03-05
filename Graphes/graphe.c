#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>
#include <time.h>

#define NS 6
#define true 1
#define false 0

Uint32 color[3];
int depart, arrivee=5, pred[NS];//NS= nombre de sommets
int v[NS][NS]={
  {1, 2, 0, 0, 0, 0},
  {0, 2, 3, 4, 0, 0},
  {0, 1, 4, 0, 0, 0},
  {1, 4, 5, 0, 0, 0},
  {1, 2, 3, 5, 0, 0},
  {3, 4, 0, 0, 0, 0}
};

int xe[NS]={40, 30, 50, 30, 50, 40};
int ye[NS]={20, 30, 30, 50, 50, 60};
int nbv[NS]={2, 4, 3, 3, 4, 2};



void pause() {
  SDL_Event evenement;
  do SDL_WaitEvent(&evenement);
  while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}

void putpixel(int xe, int ye, Uint32 c, SDL_Surface *ecran) {
  Uint32 * numerocase;
  numerocase = (Uint32*)(ecran->pixels)+xe+ye*ecran->w;
  *numerocase = c;
}

void line(int x0,int y0, int x1,int y1, Uint32 c, SDL_Surface *ecran) {
  int dx,dy,x,y,residu,absdx,absdy,stepx,stepy,i;
  dx=x1-x0; dy=y1-y0; 
  residu=0;
  x=x0;y=y0; 
  putpixel(x,y,c, ecran);
  if (dx>0) stepx=1;else stepx=-1; if (dy>0) stepy=1; else stepy=-1;
  absdx=abs(dx);absdy=abs(dy);
  if (dx==0) for(i=0;i<absdy;i++) { y+=stepy;
      putpixel(x,y,c, ecran); }
  else if(dy==0) for(i=0;i<absdx;i++){ x+=stepx;
      putpixel(x,y,c, ecran); }
  else if (absdx==absdy)
    for(i=0;i<absdx;i++) {x+=stepx; y+=stepy;
      putpixel(x,y,c, ecran);
    }
  else if (absdx>absdy)
    for(i=0;i<absdx;i++)
      { x+=stepx; residu+=absdy;
	if(residu >= absdx) {residu -=absdx; y+=stepy;}
	putpixel(x,y,c, ecran);
      }
  else for(i=0;i<absdy;i++)
	 {y+=stepy; residu +=absdx;
	   if (residu>=absdy) {residu -= absdy;x +=stepx;}
	   putpixel(x,y,c, ecran);
	 }
}


void linewithwidth(int x1, int y1, int x2, int y2, int width,Uint32 c, SDL_Surface *ecran) {
  int dx,dy;
  float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
  line(x1,y1,x2,y2,c, ecran);
  dx=x2-x1; dy=y2-y1;
  d=sqrt(dx*dx+dy*dy);
  if (d!=0.)
    /* si le vecteur n¡¯est step nul */
    { dx1=(float)width*(float)dx/d; dy1=(float)width*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      for(k=0;k<=1.;k+=0.1/d)
	{
	  xf1=(1.-k)*x1+k*x2; yf1=(1.-k)*y1+k*y2;
	  xf2=xf1-ndx1; yf2=yf1-ndy1; line(xf1,yf1,xf2,yf2,c, ecran);
	  xf2=xf1-ndx2; yf2=yf1-ndy2; line(xf1,yf1,xf2,yf2,c, ecran);
	}
    }
}

void cree_graphe(){//creation du graphe
  int i, j;
  for(i=0; i<NS; i++){//pour chaque sommet du graphe
    nbv[i]=rand()%NS;//choix aleatoire du nombre de voisin
    for(j=0; j<nbv[i]; j++)
      v[i][j]= rand()% NS;//choix aleatoire du voisin du sommet
  }
}

/*dessin des aretes*/
void dessin_arete(SDL_Surface *ecran) {
  int i, j;
  for(i = 0; i < NS; i++)
    for(j=0; j<nbv[i]; j++){
      line(xe[i], ye[i], xe[v[i][j]], ye[v[i][j]], color[1], ecran);
      SDL_Flip(ecran);
    }
}

int intopred(int voisin_x, int etage_x){
  int k;  
  for(k = etage_x; k>=1; k--)
    if(voisin_x== pred[k])
      return true;
  return false;
}

void chemins(int sommet, int etage_sommet, SDL_Surface *ecran){
  int i, k, voisin;
  if(sommet == arrivee){
    dessin_arete(ecran);
    // sleep(1);
    SDL_Delay(1);
    for(k=1; k<etage_sommet; k++) //Dessin du chemin
      linewithwidth(xe[pred[k]],ye[pred[k]],xe[pred[k+1]],ye[pred[k+1]],2,color[2], ecran); 
    //dessin du dernier chemins
    linewithwidth(xe[pred[k]], ye[pred[k]],xe[arrivee],ye[arrivee],2,color[2], ecran); 
    SDL_Flip(ecran);
    pause();
    SDL_FillRect(ecran, NULL, color[0]);
  }
  else{
    for(i=0; i<nbv[sommet]; i++){
      voisin= v[sommet][i];
      if(intopred(voisin, etage_sommet)== false){
	pred[etage_sommet+1]= sommet;
	chemins(voisin, etage_sommet+1, ecran);
      }
    }
  }
}

int main(){
  srand(time(NULL));
  int x, y;
  SDL_Surface *ecran; 
 
  SDL_Init(SDL_INIT_VIDEO);
  ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  
  color[0] = SDL_MapRGB(ecran->format,255,255,255); //Blanc
  color[1] = SDL_MapRGB(ecran->format,0,0,0);//Noir
  color[2] = SDL_MapRGB(ecran->format,255,0,0); //Rouge
  
  SDL_FillRect(ecran, NULL, color[0]);
  SDL_Flip(ecran);
  SDL_WM_SetCaption("graphe", NULL);
  chemins(0,0, ecran);//etage 0= 0;
  SDL_Quit();
  return 0;
}
