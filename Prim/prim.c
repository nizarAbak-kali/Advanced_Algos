
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 7
#define zoom 80.
void pause(void);
void putpixel(int xe, int ye, Uint32 couleur);
Uint32 getpixel(int xe, int ye);
void line(int x0,int y0, int x1,int y1, Uint32 c);
void linewithwidth(int x1, int y1, int x2, int y2, int epaisseur,Uint32 c);
void filldisc( int xo, int yo, int R, Uint32 couleur);
SDL_Surface * screen; Uint32 blanc,noir,rouge,vert;
SDL_Surface *texte;
SDL_Rect position;

int P[N][N],S[N],T[N],weight[N],pred[N];char chiffre[200];int xorig,yorig;
float x[N],y[N];

int main(int argc, char ** argv){
  int i,j,k,step,LS,LT,pmin,imin,K,startpoint;
  SDL_Init(SDL_INIT_VIDEO); 
  screen=SDL_SetVideoMode(800,600,32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  SDL_WM_SetCaption("PARCOUR D'UN CHEMIN by EL KHADEM Amir 255324", NULL);
  blanc=SDL_MapRGB(screen->format,255,255,255);
  noir=SDL_MapRGB(screen->format,0,0,0);
  rouge=SDL_MapRGB(screen->format,0,255,0);
  vert=SDL_MapRGB(screen->format,0,255,0);
  SDL_FillRect(screen,0,blanc);
   SDL_Color couleurnoire={0,0,0};
   position.x=20; position.y=80;
   SDL_BlitSurface(texte,NULL,screen,&position);
   xorig=400; yorig=300;
   for(i=0;i<N;i++) P[i][i]=0;
   P[0][1]=7; P[0][2]=1; P[0][3]=5; P[0][4]=10000; P[0][5]=10000;P[0][6]=10000;
   P[1][2]=6; P[1][3]=10000; P[1][4]=3; P[1][5]=10000;P[1][6]=10000;
   P[2][3]=5; P[2][4]=8; P[2][5]=4; P[2][6]=10000;
   P[3][4]=10000; P[3][5]=2;P[3][6]=10000;
   P[4][5]=10000;P[4][6]=9;
   P[5][6]=3;
   for(i=0;i<N-1;i++)
     for(j=i+1;j<N;j++) P[j][i]=P[i][j];
   x[0]=0.; y[0]=2.; x[1]=-2.; y[1]=0.5;   x[2]=0.; y[2]=0.; x[3]=2.; y[3]=0.5;
   x[4]=-2.; y[4]=-2.; x[5]=1.8; y[5]=-1.9; x[6]=0.1; y[6]=-2.9;
   for(i=0;i<N;i++)
     for(j=0;j<N;j++)
       if(P[i][j]>0 && P[i][j]<1000){
	 line(xorig+zoom*x[i],yorig-zoom*y[i],xorig+zoom*x[j],yorig-zoom*y[j],noir);
	 position.x=xorig+zoom*(x[i]+x[j])/2+5; position.y=yorig-zoom*(y[i]+y[j])/2-22;
	 SDL_BlitSurface(texte,NULL,screen,&position);
       }
   startpoint=3;
   S[0]=startpoint;
   LS=1;
   filldisc(xorig+zoom*x[startpoint],yorig-zoom*y[startpoint],8,rouge);
   SDL_Flip(screen);
   pause();
   k=0;
   for(i=0;i<N;i++)
     if (i!=startpoint)
       T[k++]=i;
   LT=N-1;
   for(i=0;i<LT;i++)
     weight[T[i]]=P[startpoint][T[i]];
   for(i=0;i<LT;i++)
     pred[T[i]]=startpoint;
   for(step=2;step<=N;step++){
     pmin=10000;
     for(i=0;i<LT;i++)
       if (weight[T[i]]<pmin){
	 pmin=weight[T[i]];
	 imin=i;}
         K=T[imin];
	 S[LS]=K;
	 LS++;
	 linewithwidth(xorig+zoom*x[K],yorig-zoom*y[K], xorig+zoom*x[pred[K]],yorig-zoom*y[pred[K]],5,rouge);
	 SDL_Flip(screen);
	 pause();
	 for(i=imin;i<LT-1;i++)
	   T[i]=T[i+1];
	 LT--;
	 for(i=0;i<LT;i++)
	   if (P[K][T[i]]<weight[T[i]]){
	     weight[T[i]]=P[K][T[i]]; pred[T[i]]=K;
	   }
   }
   for(i=0;i<N;i++)
     if (i!=startpoint)
       linewithwidth(xorig+zoom*x[i],yorig-zoom*y[i], xorig+zoom*x[pred[i]],yorig-zoom*y[pred[i]],3,rouge);
}


void pause(void){
  SDL_Event evenement;
  do
    SDL_WaitEvent(&evenement);
  while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}


void putpixel(int xe, int ye, Uint32 couleur){
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;
  *numerocase=couleur;
}


Uint32 getpixel(int xe, int ye){
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;
  return (*numerocase);
}



void line(int x0,int y0, int x1,int y1, Uint32 c){
  int dx,dy,x,y,residu,absdx,absdy,pasx,pasy,i;
  dx=x1-x0;
  dy=y1-y0;
  residu=0;
  x=x0;
  y=y0;
  putpixel(x,y,c);
  if (dx>0)
    pasx=1;
  else
    pasx=-1;
  if (dy>0)
    pasy=1;
  else
    pasy=-1;
  absdx=abs(dx);
  absdy=abs(dy);
  if (dx==0)
    for(i=0;i<absdy;i++){
      y+=pasy;
      putpixel(x,y,c);
    }
  else
    if(dy==0)
      for(i=0;i<absdx;i++){
	x+=pasx;
	putpixel(x,y,c);
      }
    else
      if (absdx==absdy)
	for(i=0;i<absdx;i++){
	  x+=pasx;
	  y+=pasy;
	  putpixel(x,y,c);
	}
      else
	if(absdx>absdy)
	  for(i=0;i<absdx;i++){
	    x+=pasx;
	    residu+=absdy;
	    if(residu >= absdx){
	      residu -=absdx;
	      y+=pasy;
	    }
	    putpixel(x,y,c);
	  }
	else
	  for(i=0;i<absdy;i++){
	    y+=pasy;
	    residu +=absdx;
	    if(residu>=absdy){
	      residu -= absdy;
	      x +=pasx;
	    }
            putpixel(x,y,c);
	  }
}


void linewithwidth(int x1, int y1, int x2, int y2, int epaisseur,Uint32 c){
  int dx,dy;
  float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
  line(x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1;
  d=sqrt(dx*dx+dy*dy);
  if (d!=0.){
    dx1=(float)epaisseur*(float)dx/d;
    dy1=(float)epaisseur*(float)dy/d;
    ndx1=dx1*cos(angle)-dy1*sin(angle);
    ndy1=dx1*sin(angle)+dy1*cos(angle);
    ndx2=dx1*cos(-angle)-dy1*sin(-angle);
    ndy2=dx1*sin(-angle)+dy1*cos(-angle);
    for(k=0;k<=1.;k+=0.1/d){
      xf1=(1.-k)*x1+k*x2;
      yf1=(1.-k)*y1+k*y2;
      xf2=xf1-ndx1; yf2=yf1-ndy1;
      line(xf1,yf1,xf2,yf2,c);
      xf2=xf1-ndx2;
      yf2=yf1-ndy2;
      line(xf1,yf1,xf2,yf2,c);
    }
  }
}

void filldisc( int xo, int yo, int R, Uint32 couleur){
  int x, y, F, F1, F2,newx,newy,xx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600)
    putpixel(x,y,couleur);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600)
    putpixel (x,2*yo-y, couleur);
  while( y>yo){
    F1=F+2*(x-xo)+1;
    F2=F-2*(y-yo)+1;
    if ( abs(F1)<abs(F2)){
      x+=1;
      F=F1;
    }
    else {
      y-=1;
      F=F2;
    }
    newx=2*xo-x ;
    newy=2*yo-y ;
    for(xx=newx; xx<=x; xx++)
      if (xx<800 && xx>=0 && y>=0 && y<600 )
	putpixel(xx,y,couleur);
    for(xx=newx; xx<=x; xx++)
      if (xx<800 && xx>=0 && newy>=0 && newy<600 )
	putpixel(xx,newy,couleur);
  }
  if (xo+R<800 && xo+R>=0&& y>=0 && y<600)
    putpixel(xo+R,yo,couleur);
  if (xo-R<800 && xo-R>=0&& y>=0 && y<600)
    putpixel(xo-R,yo, couleur);
}


