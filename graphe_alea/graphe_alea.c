#include<stdlib.h>
#include<stdio.h>
#include<SDL/SDL.h>
#include<math.h>

#define nb_sommet 300
#define R1 40 
#define OUI 1
#define NON 0
SDL_Surface * screen;
Uint32 blans, noir, rouge, bleu;
Uint32 couleur[1];
int x[nb_sommet];
int y[nb_sommet];
int file[nb_sommet];
int voisin[nb_sommet][3];
int deja_vu[nb_sommet];
int pred[nb_sommet];
int depart;
void pause();
Uint32 getpixel(int, int);
void putpixel(int, int, Uint32);
void filldisc(int, int, int, Uint32);
void arrow(int, int, int, int, Uint32);
void circle(int, int, int, Uint32);
void line(int, int, int, int, Uint32);
void linewithwidth(int, int, int, int, int, Uint32);
void chemin(int);
void explorer(int);

/****************** le programme principale ********************/
int main(){
  int i, j, d2, arrivee;
 
  x[0] = 20 + rand()%500;
  y[0] = 20 + rand()%500;
 
  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode( 800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("graphe au hasard", NULL);

/**************** les couleurs ***************************/
  Uint32 blanc = SDL_MapRGB(screen->format, 255,255,255);
  Uint32 noir = SDL_MapRGB(screen->format, 0,0,0);
  Uint32 rouge = SDL_MapRGB(screen->format, 255,0,0);
  Uint32 vert = SDL_MapRGB(screen->format, 0,255,0);
  Uint32 bleu = SDL_MapRGB(screen->format, 0,0,255);
  couleur[0] = SDL_MapRGB(screen->format, 0,0,255);
  SDL_FillRect(screen, NULL, blanc);

/****************** l essentiel du programme ******************/

/**************** un poit(sommet) au hasard sur le screen**************/
  filldisc(x[0], y[0], R1, rouge);
  filldisc(x[0], y[0], 3, noir);

 /****************** dessiner nb_sommet******************************/
  srand(time(NULL));
  for(i = 1; i < nb_sommet; i++){
    x[i] = 20 + rand()%500;
    y[i] = 20 + rand()%500;
    
    if(getpixel(x[i], y[i]) != rouge){     
      filldisc(x[i], y[i], R1, rouge);
      filldisc(x[i], y[i], 3, noir);
    }
  }

/**************** effacer les pixels rouges ***************************/
  for(i = 0; i < 800; i++){
    for(j = 0; j < 600; j++){
      if(getpixel(i, j) == rouge)
	putpixel(i, j, blanc);
    }
  }
/**************** tracer les arcs de jonction ***************************/
/**************** le 1er arc de chaque sommet i***************************/
  for(i = 0; i < nb_sommet; i++){    
    int d2min = 10000000;
    int jmin;
    for(j = 0; j < nb_sommet; j++){
      if(i != j){
	d2 = (x[j]-x[i])*(x[j]-x[i]) + (y[j]-y[i])*(y[j]-y[i]);
	if(abs(d2) < d2min){
	  d2min = d2;
	  jmin = j;
	}
      }
    }
    voisin[i][0] = jmin;
    arrow(x[i], y[i], x[jmin], y[jmin], noir);
  }
  /**************** le 2eme arc de chaque sommet i***************************/
  for(i = 0; i < nb_sommet; i++){   
    int d2min = 10000000;
    int jmin; 
    for(j = 0; j < nb_sommet; j++){
      if((i != j) && (j != voisin[i][0])){
	d2 = (x[j]-x[i])*(x[j]-x[i]) + (y[j]-y[i])*(y[j]-y[i]);
	if(abs(d2) < d2min){
	  d2min = d2;
	  jmin = j;
	}
      }
    }
    voisin[i][1] = jmin;
    arrow(x[i], y[i], x[voisin[i][1]], y[voisin[i][1]], noir);
  }
  /**************** le 3eme arc de chaque sommet i***************************/
  for(i = 0; i < nb_sommet; i++){    
    int d2min = 10000000;
    int jmin; 
    for(j = 0; j < nb_sommet; j++){
      if((i != j) && (j != voisin[i][0]) &&(j != voisin[i][1])){
	d2 = (x[j]-x[i])*(x[j]-x[i]) + (y[j]-y[i])*(y[j]-y[i]);
	if(abs(d2) < d2min){
	  d2min = d2;
	  jmin = j;
	}
      }
    }
    voisin[i][2] = jmin;
    arrow(x[i], y[i], x[voisin[i][2]], y[voisin[i][2]], noir);
  }
/**************** les sommets de depart et d'arrivee***************************/
  int d2min = 10000000;
  int d2max = 100;
  int imin = 100;
  int imax = 100;
  for(i = 0; i < nb_sommet; i++){   
    d2 = (x[i] * x[i]) + (y[i] * y[i]);
    if(d2 < d2min){
      d2min = d2;
      imin = i;
    }else if(d2 > d2max){
      d2max = d2;
      imax = i;
    }
  }
  depart = imin;
  arrivee = imax;
  filldisc(x[imin], y[imin], 5, bleu);
  filldisc(x[imax], y[imax], 5, bleu);
/**************** le parcour en largeur***************************/
  //explorer(imin);
    int longueur_file = 1;
    int le_voisin, k;
    file[0] = depart;
    deja_vu[imin] = OUI;
    while(longueur_file != 0){
      i = file[0];
      for(j = 0; j < 3; j++){
	le_voisin = voisin[i][j];
	if(deja_vu[le_voisin] == NON){	 
	  deja_vu[le_voisin] = OUI;
	  pred[le_voisin] = i;	
	  linewithwidth(x[i], y[i], x[le_voisin], y[le_voisin], 3, rouge); 
	  filldisc(x[le_voisin], y[le_voisin],5,vert);       
	  SDL_Flip(screen);
	  SDL_Delay(120);
	  file[longueur_file] = le_voisin;
	  longueur_file++;
	}
      }

    for(k = 0; k < longueur_file-1; k++)
      file[k] = file[k+1];
    longueur_file--;    
    }

    chemin(arrivee);
    SDL_Flip(screen);
    pause();
    return 0;
}

/********************** les fonctions utilitaires ****************************/
void pause(){
  SDL_Event evenement;
  do SDL_WaitEvent(&evenement);
  while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}

void putpixel(int xe, int ye, Uint32 c){

  Uint32 *numerocase;
  numerocase = (Uint32*)(screen->pixels) + xe + ye*screen->w;
  *numerocase = c;
}

Uint32 getpixel(int xe, int ye){
  
  Uint32 *numerocase;
  numerocase = (Uint32 *)(screen->pixels)+ xe + ye*screen->w;
  return (*numerocase);
}


void filldisc(int xo, int yo, int R, Uint32 c){
  int x, y, F, F1, F2,newx,newy,xx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600)
    putpixel (x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ; newy=2*yo-y ;
      for(xx=newx; xx<=x; xx++)if (xx<800 &&xx>=0 && y>=0 && y<600 )
				 putpixel(xx,y,c);
      for(xx=newx; xx<=x; xx++)if (xx<800 &&xx>=0 && newy>=0 && newy<600 )
				 putpixel(xx,newy,c);
    }
  if (xo+R<800 && xo+R>=0&& y>=0 && y<600) putpixel(xo+R,yo,c);
  if (xo-R<800 && xo-R>=0&& y>=0 && y<600) putpixel(xo-R,yo, c);
}
 
void arrow(int x1, int y1, int x2, int y2, Uint32 c)
{
  int dx,dy;
  float xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/6.;
  line(x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1;
  d=sqrt(dx*dx+dy*dy);
  if (d!=0.)
    { dx1=6.*(float)dx/d; dy1=6.*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      xf1=0.3*x1+0.7*x2; yf1=0.3*y1+0.7*y2; xf2=xf1-ndx1; yf2=yf1-ndy1;
      line(xf1,yf1,xf2,yf2,c);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      xf2=xf1-ndx2; yf2=yf1-ndy2; line(xf1,yf1,xf2,yf2,c);
    }
  else
    {circle(x1+10,y1,10,c); line(x1+20,y1,x1+23,y1-6,c);
      line(x1+20,y1,x1+15,y1-5,c);
    }
}
 
void line(int x0,int y0, int x1,int y1, Uint32 c)
{
  int dx,dy,x,y,residu,absdx,absdy,pasx,pasy,i;
  dx=x1-x0; dy=y1-y0; residu=0;
  x=x0;y=y0; if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
  if (dx>0) pasx=1;else pasx=-1; if (dy>0) pasy=1; else pasy=-1;
  absdx=abs(dx);absdy=abs(dy);
  if (dx==0) for(i=0;i<absdy;i++) { y+=pasy;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c); }
  else if(dy==0) for(i=0;i<absdx;i++){ x+=pasx;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c); }
  else if (absdx==absdy)
    for(i=0;i<absdx;i++) {x+=pasx; if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
      y+=pasy;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
    }
  else if (absdx>absdy)
    for(i=0;i<absdx;i++)
      { x+=pasx; if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
	residu+=absdy;
	if(residu >= absdx) {residu -=absdx; y+=pasy;
	  if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
	}
      }
  else for(i=0;i<absdy;i++)
	 {y+=pasy; if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
	   residu +=absdx;
	   if (residu>=absdy) {residu -= absdy;x +=pasx;
	     if (x>=0 && x<800 && y>=0 && y<600) putpixel(x,y,c);
	   }
	 }
}

void circle( int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,c);
      newx=2*xo-x ; newy=2*yo-y ;
      if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(x, newy,c);
      if (newx<800 && newx>=0 && y>=0 && y<600) putpixel( newx,y,c);
      if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(newx,
							       newy, c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(xo+R,yo,c);
  if (xo-R<800 && xo-R>=0) putpixel(xo-R,yo, c);
}

void linewithwidth(int x1, int y1, int x2, int y2, int width,Uint32 c){
  int dx,dy;
  float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
  line(x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1;
  d=sqrt(dx*dx+dy*dy);
  if (d!=0.)
    /* si le vecteur n’est step nul */
    { 
      dx1=(float)width*(float)dx/d; dy1=(float)width*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      for(k=0;k<=1.;k+=0.1/d){
	xf1=(1.-k)*x1+k*x2; yf1=(1.-k)*y1+k*y2;
	xf2=xf1-ndx1; yf2=yf1-ndy1; line(xf1,yf1,xf2,yf2,c);
	xf2=xf1-ndx2; yf2=yf1-ndy2; line(xf1,yf1,xf2,yf2,c);
      }
    }
}

void explorer(int i){
  int j, le_voisin;
  deja_vu[i] = OUI;
  filldisc(x[i],y[i],5,couleur[0]);
  for(j = 0; j < 3; j++){
    le_voisin = voisin[i][j];
    if(deja_vu[le_voisin] == NON){
      linewithwidth(x[i], y[i], x[le_voisin], y[le_voisin], 2, couleur[0]); 
      explorer(le_voisin);
    }
    SDL_Flip(screen);
    SDL_Delay(100);   
  }
}

void chemin(int i){
  if(i != depart){
    chemin(pred[i]);
    linewithwidth(x[i],y[i], x[pred[i]], y[pred[i]], 3, bleu);
    SDL_Flip(screen);
    SDL_Delay(100);
  }
}

