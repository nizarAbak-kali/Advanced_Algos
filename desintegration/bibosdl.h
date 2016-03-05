#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

/***** Fonction graphique sur SDL *****/

/***** Fonction qui met le programme en pause  *****/
void pause(void)
{
  SDL_Event evenement;
  do SDL_WaitEvent(&evenement);
  while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN);
}

/***** Place un pixel dans l'ecran  *****/
void putpixel(SDL_Surface *screen,int xe, int ye, Uint32 c)
{ 
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; 
  *numerocase=c;
}

/***** Donne la couleur d'un pixel *****/
Uint32 getpixel(SDL_Surface *screen,int xe, int ye)
{ Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; return (*numerocase);
}

/***** Trace un cercle  *****/
void circle( SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
      newx=2*xo-x ; newy=2*yo-y ;
      if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(screen,x, newy,c);
      if (newx<800 && newx>=0 && y>=0 && y<600) putpixel(screen,newx,y,c);
      if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(screen,newx,
							       newy, c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(screen,xo+R,yo,c);
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}

/*****  *****/
void filldisc(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newx,newy,xx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
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


/** stair line, must be used for floodfill (ligne en marches d'escalier) */
void line(SDL_Surface *screen,int x0,int y0, int x1,int y1, Uint32 c)
{
  int dx,dy,x,y,residu,absdx,absdy,pasx,pasy,i;
  dx=x1-x0; dy=y1-y0; residu=0; x=x0;y=y0; if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (dx>0) pasx=1;else pasx=-1; if (dy>0) pasy=1; else pasy=-1;
  absdx=abs(dx);absdy=abs(dy);
  if (dx==0) for(i=0;i<absdy;i++) { y+=pasy;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c); }
  else if(dy==0) for(i=0;i<absdx;i++){ x+=pasx;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c); }
  else if (absdx==absdy)
    for(i=0;i<absdx;i++) {x+=pasx; if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
      y+=pasy;
      if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
    }
  else if (absdx>absdy)
    for(i=0;i<absdx;i++)
      { x+=pasx; if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
	residu+=absdy;
	if(residu >= absdx) {residu -=absdx; y+=pasy;
	  if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
	}
      }
  else for(i=0;i<absdy;i++)
	 {y+=pasy; if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
	   residu +=absdx;
	   if (residu>=absdy) {residu -= absdy;x +=pasx;
	     if (x>=0 && x<800 && y>=0 && y<600) putpixel(screen,x,y,c);
	   }
	 }
}

/*****  *****/
void rectangle(SDL_Surface *screen,int x1,int y1, int x2, int y2, Uint32 c)
{
  line(screen,x1,y1,x2,y1,c);line(screen,x1,y2,x2,y2,c);line(screen,x1,y1,x1,y2,c);line(screen,x2,y2,x2,y1,c);
}

/*****  *****/
void arrow(SDL_Surface *screen,int x1, int y1, int x2, int y2, Uint32 c)
{
  int dx,dy;
  float xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/6.;
  line(screen,x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1; d=sqrt(dx*dx+dy*dy);
  if (d!=0.)
    { dx1=6.*(float)dx/d; dy1=6.*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      xf1=0.3*x1+0.7*x2; yf1=0.3*y1+0.7*y2; xf2=xf1-ndx1; yf2=yf1-ndy1;
      line(screen,xf1,yf1,xf2,yf2,c);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      xf2=xf1-ndx2; yf2=yf1-ndy2; line(screen,xf1,yf1,xf2,yf2,c);
    }
  else
    {circle(screen,x1+10,y1,10,c); line(screen,x1+20,y1,x1+23,y1-6,c);
      line(screen,x1+20,y1,x1+15,y1-5,c);
    }
}


/** add #include <math.h> */
void linewithwidth(SDL_Surface *screen,int x1, int y1, int x2, int y2, int width,Uint32 c)
{
  int dx,dy;
  float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
  line(screen,x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1; d=sqrt(dx*dx+dy*dy);
  if (d!=0.) /* si le vecteur n’est step nul */
    { dx1=(float)width*(float)dx/d; dy1=(float)width*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      for(k=0;k<=1.;k+=0.1/d)
	{
	  xf1=(1.-k)*x1+k*x2; yf1=(1.-k)*y1+k*y2;
	  xf2=xf1-ndx1; yf2=yf1-ndy1; line(screen,xf1,yf1,xf2,yf2,c);
	  xf2=xf1-ndx2; yf2=yf1-ndy2; line(screen,xf1,yf1,xf2,yf2,c);
	}
    }
}


void floodfill(SDL_Surface *screen,int x,int y, Uint32 cr,Uint32 cb)
{ int xg,xd,xx;
  if (getpixel(screen,x,y) !=cb && getpixel(screen,x,y) !=cr)
    { putpixel(screen,x,y,cr);
      xg=x-1;
      while(xg>0 && getpixel(screen,xg,y)!=cb) {putpixel(screen,xg,y,cr); xg--;}
      xd=x+1;
      while(xd<800 && getpixel(screen,xd,y)!=cb) {putpixel(screen,xd,y,cr); xd++ ;}
      for(xx=xg; xx<xd;xx++)
	{ if (y>1 ) {floodfill(screen,xx,y-1,cr,cb);}
	  if (y<599 ) {floodfill(screen,xx,y+1,cr,cb);}
	}
    }
}


void arc0_90(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newy;//newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
	newy=2*yo-y ;
      if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(screen,x, newy,c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(screen,xo+R,yo,c);
}


void arc270_360(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2;//newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(screen,xo+R,yo,c);
}


void arc90_180(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ; newy=2*yo-y ;
      if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(screen,newx,
							       newy, c);
    }
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}


void arc180_270(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newx;//newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ;
      if (newx<800 && newx>=0 && y>=0 && y<600) putpixel(screen,newx,y,c);
    }
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}

void arc0_180(SDL_Surface *screen,int xo, int yo, int R, Uint32 c){
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ; newy=2*yo-y ;
      if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(screen,x, newy,c);
      if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(screen,newx,
							       newy, c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(screen,xo+R,yo,c);
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}

void arc180_360(SDL_Surface *screen,int xo, int yo, int R, Uint32 c){
  int x, y, F, F1, F2,newx;//newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
      newx=2*xo-x ;
      if (newx<800 && newx>=0 && y>=0 && y<600) putpixel(screen,newx,y,c);
    }
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}
void arc270_90(SDL_Surface *screen,int xo, int yo, int R, Uint32 c)
{
  int x, y, F, F1, F2,newy;//newx;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
      newy=2*yo-y ;
      if (x<800 && x>=0 && newy>=0 && newy<600) putpixel(screen,x, newy,c);
    }
  if (xo+R<800 && xo+R>=0) putpixel(screen,xo+R,yo,c);
}


void arc90_270(SDL_Surface *screen,int xo, int yo, int R, Uint32 c){
  int x, y, F, F1, F2,newx,newy;
  x=xo; y=yo+R; F=0;
  if (x<800 && x>=0 && y>=0 && y<600) putpixel(screen,x,y,c);
  if (x<800 && x>=0 && 2*yo-y>=0 && 2*yo-y<600) putpixel (screen,x,2*yo-y, c);
  while( y>yo)
    {
      F1=F+2*(x-xo)+1; F2=F-2*(y-yo)+1;
      if ( abs(F1)<abs(F2)) { x+=1; F=F1;}
      else {y-=1; F=F2;}
      newx=2*xo-x ; newy=2*yo-y ;
      if (newx<800 && newx>=0 && y>=0 && y<600) putpixel(screen,newx,y,c);
      if (newx<800 && newx>=0 && newy>=0 && newy<600) putpixel(screen,newx,
							       newy, c);
    }
  if (xo-R<800 && xo-R>=0) putpixel(screen,xo-R,yo, c);
}
