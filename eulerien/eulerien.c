/*
Algorithmique Avancée L3 - 2013/2014
Nom : Nouira
Prénom : Chafik
N° étudiant : 11291558
Programme : Cycles et chemins eulériens - Séance du 29 Novembre 2013
*/

/* Question 1 :
   Pourquoi n'existe-t-il des cycles eulériens que si le nombre de sommets est impair ? 
   Réponse :
   Un graphe connexe G admet un cycle eulérien si et seulement si tous ses sommets sont de degré pair.
   Autrement dit, il faut que le nombre de voisins de chaque sommet soit pair.
*/
#include <SDL/SDL_ttf.h>
#include "bibosdl.h"

#define LARGEUR 800
#define HAUTEUR 600
#define NS 6
#define NA 10
#define OUI 1
#define NON 0

int voisin[NS][NS],x_orig,y_orig,taille_pt=4;
int nbre_voisin[NS];
float x[NS],y[NS],xe[NS],ye[NS];
int depart,zoom=75,pred[NS];
int hauteur=0,compteur=1;

int appartientlistepredarete(int v,int i,int e) {
  int k;
  for(k = 1; k <= e-1; k++)
    if((v == pred[k+1] && i == pred[k]) || (v == pred[k] && i == pred[k+1]))
      return OUI;
  return NON;
}

void cycle (SDL_Surface *ecran,int i,int etage) {
  int j,v;
  TTF_Font *font = TTF_OpenFont("font.ttf",15);
  TTF_Font *font2 = TTF_OpenFont("font.ttf",30);
  char chaine[100];
  SDL_Surface *texte = NULL, *blanc = NULL, *rectangle = NULL;
  SDL_Rect postexte,positionblanc,positionrect;
  SDL_Color color = {0,0,255,0};

  postexte.x = (LARGEUR/2)+100; postexte.y = 0;
  positionblanc.x = (LARGEUR/2)+100; positionblanc.y = 0;
  positionrect.x = 0; positionrect.y = 0;
  
  if(etage == NA-1) {
    hauteur += 20;
    postexte.y = hauteur;
    sprintf(chaine,"Nombre de cycles Euleriens : %d",compteur);
    compteur++;
    texte=TTF_RenderText_Blended(font2,chaine,color);
    rectangle=SDL_CreateRGBSurface(SDL_HWSURFACE,texte->w,texte->h,32,0,0,0,0);
    SDL_FillRect(rectangle,NULL,SDL_MapRGB(ecran->format,255,255,255));
    positionrect.x=0;positionrect.y=30;
    SDL_BlitSurface(rectangle,NULL,ecran,&positionrect);
    SDL_BlitSurface(texte,NULL,ecran,&positionrect);
    SDL_Flip(ecran);
      
    if(postexte.y > HAUTEUR) {
      blanc = SDL_CreateRGBSurface(SDL_HWSURFACE,(LARGEUR/2)+100,HAUTEUR,32,0,0,0,0);
      SDL_FillRect(blanc,NULL,SDL_MapRGB(ecran->format,255,255,255));
      SDL_BlitSurface(blanc,NULL,ecran,&positionblanc);
      hauteur = 0;
    }
    for(j = 1; j <= NA-1; j++) {
      /*Afficher le pred de j */
      sprintf(chaine,"%d-",pred[j]);
      texte = TTF_RenderText_Blended(font,chaine,color);
      SDL_BlitSurface(texte,NULL,ecran,&postexte);
      postexte.x += 20;
    }
    /* Afficher i */
    sprintf(chaine,"%d-%d",i,depart);
    texte = TTF_RenderText_Blended(font,chaine,color);
    SDL_BlitSurface(texte,NULL,ecran,&postexte);
    postexte.x += 20;
    SDL_Flip(ecran);
    SDL_Delay(250);
  }
  
  for(j = 0; j < nbre_voisin[i]; j++) {
    v = voisin[i][j];
    if(v != pred[etage] && appartientlistepredarete(v,i,etage) == NON) {
      pred[etage+1] = i; 
      cycle(ecran,v,etage+1);
    }
  }
  
  TTF_CloseFont(font);
  SDL_FreeSurface(texte);
  SDL_FreeSurface(blanc);
  SDL_FreeSurface(rectangle);
}


int main () {
  SDL_Surface *ecran = NULL, *texte = NULL;
  SDL_Rect position;
  SDL_Color color_ttf = {0,0,255,0};
  int i = 0, j = 0, k = 0;
  char chaine[100];
  
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  ecran = SDL_SetVideoMode(LARGEUR,HAUTEUR,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
  SDL_WM_SetCaption(" Cycles et chemins eulériens ",NULL);
  SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));

  TTF_Font *font1 = TTF_OpenFont("font.ttf",15);
  TTF_Font *font2 = TTF_OpenFont("font.ttf",20);
  Uint32 noir = SDL_MapRGB(ecran->format,0,0,0);
  
  x_orig=zoom*4;
  y_orig=HAUTEUR/2;
  
  /*** Coordonnées des sommets ***/
  x[0] = -1; y[0] = 1;
  x[1] = 1; y[1] = 1;
  x[2] = 1; y[2] = -1;
  x[3] = -1; y[3] = -1;
  x[4] = 0; y[4] = 2;
  x[5] = 0; y[5] = -2;
  
  /*** Coordonnées des sommets sur l'ecran ***/
  for(i = 0; i < NS; i++) {
    xe[i] = x_orig+zoom*x[i];
    ye[i] = y_orig-zoom*y[i];
  }
 
  /*** Liste d'adjacences ***/ 
  for(i = 0; i <= 3; i++){
    k = 0;
    for(j = 0; j <= 3; j++)
      if(i != j)
	voisin[i][k++] = j;
  }
  
  voisin[0][3] = 4; nbre_voisin[0] = 4;
  voisin[1][3] = 4; nbre_voisin[1] = 4;
  voisin[2][3] = 5; nbre_voisin[2] = 4;
  voisin[3][3] = 5; nbre_voisin[3] = 4;

  voisin[4][0] = 0; nbre_voisin[4] = 2;
  voisin[4][1] = 1;
  voisin[5][0] = 2; nbre_voisin[5] = 2;
  voisin[5][1] = 3;
  
  //Sommet de départ
  depart = 4;
  
  /*** Mise du graphe sur l'écran ***/ 
  for(i = 0; i < NS; i++){
    filldisc(ecran,xe[i],ye[i],taille_pt,noir);
    position.x = xe[i]-10;
    position.y = ye[i]+5;
    sprintf(chaine,"%d",i);
    texte = TTF_RenderText_Blended(font1,chaine,color_ttf);
    SDL_BlitSurface(texte,NULL,ecran,&position);
    for(j = 0; j < nbre_voisin[i]; j++){
      if(i < voisin[i][j])
      line(ecran,xe[i],ye[i],xe[voisin[i][j]],ye[voisin[i][j]],noir);
    }
    sprintf(chaine,"Sommet de depart = %d",depart);
    texte = TTF_RenderText_Blended(font2,chaine,color_ttf);
    position.x = 0;
    position.y = 0;
    SDL_BlitSurface(texte,NULL,ecran,&position);
  }

  pred[1] = 4;
  SDL_Flip(ecran);
  
  cycle(ecran,0,1);
  SDL_Flip(ecran);
  pause();
  
  SDL_FreeSurface(texte);
  TTF_CloseFont(font1);
  TTF_Quit();
  SDL_Quit();
  return EXIT_SUCCESS;
}
