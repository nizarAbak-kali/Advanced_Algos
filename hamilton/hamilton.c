
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 5
#define NN  (N*N)
#define zoom 9.

void pause(void);
void putpixel(int xe, int ye, Uint32 couleur);
Uint32 getpixel(int xe, int ye);
void circle( int xo, int yo, int R, Uint32 couleur);
void line(int x0,int y0, int x1,int y1, Uint32 c);void fleche(int x1, int y1, int x2, int y2, Uint32 c);
void linewithwidrh(int x1, int y1, int x2, int y2, int width,Uint32 c);
void drawing(int e);
void drawinggraph(void);
void tree(int i, int level);
int belonginglistofpred(int v, int e);
SDL_Surface * screen; Uint32 white,noir,rouge;
SDL_Surface *texte;
SDL_Rect position;
int v[NN][4], pred[NN],nbv[NN],depart,end,count;  int xorig,yorig;
int x[NN],y[NN] ;  char chiffre[200];

int main(int argc, char ** argv)
{  int i,j,k;
   SDL_Init(SDL_INIT_VIDEO);
   screen=SDL_SetVideoMode(800,600,32, SDL_HWSURFACE|SDL_DOUBLEBUF);
   white=SDL_MapRGB(screen->format,255,255,255);
   noir=SDL_MapRGB(screen->format,0,0,0);
   rouge=SDL_MapRGB(screen->format,255,0,0);
   SDL_FillRect(screen,0,white);
    SDL_Color blackcolor={0,0,0};

    depart=0;end=NN-1;

    for(i=0;i<NN;i++)
      { x[i]=i%N;    
	y[i]=i/N;    
	 circle(xorig+40*x[i],yorig+40*y[i],2,noir);   }
    for(i=0;i<NN;i++)
      { k=0;
        if ((i+1)%N !=0)  
	{ v[i][k]=i+1;
	k++;
	}     
	if ( i>=N) { 
	v[i][k]=i-N;
	 k++;}
        if (i%N !=0) 
	{v[i][k]=i-1; 
	k++;}  
	if(i<NN-N) 
	{v[i][k]=i+N; 
	k++;}
        nbv[i]=k;
        }

     for(i=0;i<NN;i++)   
	for(j=0;j<nbv[i];j++)
      line(xorig+40*x[i],yorig+40*y[i], xorig+40*x[v[i][j]],yorig+40*y[v[i][j]], noir);

    
    SDL_Flip(screen); 
	pause();
	SDL_FillRect(screen,0,white);

    xorig=zoom; yorig=zoom;
    nbv[0]=1;
    tree(depart,0);

    
}

void tree(int i, int level)
{ int j,neighbour;
   if (i==end) { count++;  drawing(level);    }
   else
   { for(j=0;j<nbv[i];j++)
     { neighbour=v[i][j];    
	if(belonginglistofpred(neighbour,level)==0)
            { 
	pred[level+1]=i; tree(neighbour,level+1);      }
     }
   }
}
  int belonginglistofpred(int v, int e)
  { int i;
     for(i=1;i<=e;i++)    
	if(v==pred[i]) return 1;
     return 0;
  }

  void drawinggraph(void)
 {   int i,j;
     for(i=0;i<NN;i++)   for(j=0;j<nbv[i];j++)
      line(xorig+zoom*x[i],yorig+zoom*y[i], xorig+zoom*x[v[i][j]],yorig+zoom*y[v[i][j]], noir);
}

 void drawing(int e)
 {   int i,j;
     drawinggraph();
     for(j=1;j<e;j++)
          for(j=1;j<e;j++)
     linewithwidrh(xorig+zoom*x[pred[j]] , yorig+zoom*y[pred[j]] ,
             xorig+zoom*x[pred[j+1]] ,  yorig+zoom*y[pred[j+1]] , 1, rouge);
      linewithwidrh(xorig+zoom*x[pred[e]] , yorig+zoom*y[pred[e]] ,
             xorig+zoom*x[end] ,  yorig+zoom*y[end] , 1, rouge);

     xorig+=N*zoom; if (xorig>750) {yorig+=N*zoom; xorig= zoom;}
     if (yorig>550) {SDL_Flip(screen);pause();SDL_FillRect(screen,0,white);xorig=zoom;yorig=zoom;}
 }

void pause(void)
{
    SDL_Event evenement;
    do   SDL_WaitEvent(&evenement);
    while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}

void putpixel(int xe, int ye, Uint32 couleur)
{ Uint32 * numerocase;
numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;    *numerocase=couleur;
}

Uint32 getpixel(int xe, int ye)
{ Uint32 * numerocase;
   numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w;   return (*numerocase);
}

void circle( int xo, int yo, int R, Uint32 couleur)
  {
      int x, y, F, F1, F2,newx,newy;
      x=xo; y=yo+R; F=0;
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,couleur);
      if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (x,2*yo-y, couleur);
      while( y>yo)
         {
             F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
             if ( abs(F1)<abs(F2))  { x+=1; F=F1;}
             else {y-=1; F=F2;}
             if (x<800 && x>=0 && y>=0 && y<600) putpixel(x,y,couleur);
              newx=2*xo-x ; newy=2*yo-y ;
             if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(x, newy,couleur);
             if (newx<800 && newx>=0 && y>=0 && y<600) putpixel( newx,y,couleur);
             if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(newx,
             newy, couleur);
         }
       if (xo+R<800 && xo+R>=0) putpixel(xo+R,yo,couleur);
       if (xo-R<800 && xo-R>=0) putpixel(xo-R,yo, couleur);
  }

void line(int x0,int y0, int x1,int y1, Uint32 c)
{
int dx,dy,x,y,residu,absdx,absdy,pasx,pasy,i;
dx=x1-x0; dy=y1-y0;  residu=0;       x=x0;y=y0;  putpixel(x,y,c);
if (dx>0) pasx=1;else pasx=-1; if (dy>0) pasy=1; else pasy=-1;
absdx=abs(dx);absdy=abs(dy);
if (dx==0) for(i=0;i<absdy;i++) { y+=pasy;
                                                    putpixel(x,y,c); }
else if(dy==0) for(i=0;i<absdx;i++){ x+=pasx;
                                                         putpixel(x,y,c);  }
else if (absdx==absdy)
   for(i=0;i<absdx;i++) {x+=pasx; y+=pasy;
                                     putpixel(x,y,c);
                                     }
else if (absdx>absdy)
          for(i=0;i<absdx;i++)
			  { x+=pasx; residu+=absdy;
			     if(residu >= absdx) {residu -=absdx; y+=pasy;}
			     putpixel(x,y,c);
			  }
else for(i=0;i<absdy;i++)
             {y+=pasy; residu +=absdx;
              if (residu>=absdy)   {residu -= absdy;x +=pasx;}
              putpixel(x,y,c);
             }
}

void linewithwidrh(int x1, int y1, int x2, int y2, int width,Uint32 c)
{
int dx,dy;
float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
line(x1,y1,x2,y2,c);
dx=x2-x1; dy=y2-y1;       d=sqrt(dx*dx+dy*dy);
if (d!=0.)      /* si le vecteur n’est pas nul */
{ dx1=(float)width*(float)dx/d; dy1=(float)width*(float)dy/d;
   ndx1=dx1*cos(angle)-dy1*sin(angle);
   ndy1=dx1*sin(angle)+dy1*cos(angle);
   ndx2=dx1*cos(-angle)-dy1*sin(-angle);
   ndy2=dx1*sin(-angle)+dy1*cos(-angle);
   for(k=0;k<=1.;k+=0.1/d)
   {
   xf1=(1.-k)*x1+k*x2; yf1=(1.-k)*y1+k*y2;
   xf2=xf1-ndx1; yf2=yf1-ndy1;  line(xf1,yf1,xf2,yf2,c);
   xf2=xf1-ndx2; yf2=yf1-ndy2;    line(xf1,yf1,xf2,yf2,c);
   }
}
}
