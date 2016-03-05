
#include <SDL/SDL_ttf.h>
#include"bibosdl.h"

#define OUI 1
#define NON 0
#define NS 8

Uint32 couleur[5];
int v[NS][NS], nbv[NS],dejavus[NS],F[NS],pred[NS];
int zoom=75, x[NS],y[NS], xe[NS],ye[NS], depart=0, L=1;
 
void chemin(SDL_Surface* ecran, int arrive){
  if(arrive != depart){
    chemin(ecran, pred[arrive]);
    linewithwidth(ecran,xe[arrive],ye[arrive],xe[pred[arrive]], ye[pred[arrive]],2, couleur[3]);
  }
}

void explorer_largeur(SDL_Surface* ecran){
  TTF_Font *police = NULL;
  SDL_Surface *sommet_t = NULL, *voisin_t = NULL;
  SDL_Rect sommet_pos, voisin_pos;
  SDL_Color couleur_texte = {255,0,0,0};
  char tab_s[20]; 
  char tab_v[20]; 
  police = TTF_OpenFont("font.ttf",30);
  int j;
  int arrive=3;
  while(L != 0){
    int tete = F[0];
    for(j = 0; j < nbv[tete]; j++) {
      if(dejavus[v[tete][j]] == NON) {
	F[L] = v[tete][j];
	dejavus[F[L]] = OUI;
	L++;
	pred[v[tete][j]] = tete;
	sommet_pos.x=xe[tete];
	sommet_pos.y=ye[tete];
	voisin_pos.x=xe[v[tete][j]];
	voisin_pos.y=ye[v[tete][j]];
	sprintf(tab_s, "%d", tete);
	sommet_t = TTF_RenderText_Blended(police,tab_s, couleur_texte);
	SDL_BlitSurface(sommet_t, NULL,ecran, &sommet_pos); /*blit du texte*/
	sprintf(tab_v, "%d",v[tete][j] );
	voisin_t = TTF_RenderText_Blended(police,tab_v, couleur_texte);
	SDL_BlitSurface(voisin_t, NULL,ecran, &voisin_pos); /*blit du texte*/
	line(ecran, xe[tete],ye[tete],xe[v[tete][j]], ye[v[tete][j]], couleur[2]);
	SDL_Flip(ecran);	
      }
    }
    for(j = 0; j < L-1; j++)
      F[j]=F[j+1];
    L--;
  }
  chemin(ecran, arrive);
}

int main() {
  SDL_Surface *ecran, *texte;
  TTF_Font *police = NULL;
  SDL_Rect pos_texte = {0,0,0,0};
  SDL_Color couleur_texte = {255,0,0,0};

  ecran = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
  couleur[0] = SDL_MapRGB(ecran->format,255,255,255); //Blanc
  couleur[1] = SDL_MapRGB(ecran->format,0,0,0); //Noir
  couleur[2] = SDL_MapRGB(ecran->format,255,0,0); //Rouge
  couleur[3] = SDL_MapRGB(ecran->format,0,0,255); //Bleu
  SDL_FillRect(ecran,NULL,couleur[0]);

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  police = TTF_OpenFont("font.ttf",30);
  texte = TTF_RenderText_Blended(police,"Parcours en Largeur - Plus Court Chemin", couleur_texte);

  v[0][0]=1; v[0][1]=2;v[0][2]=5;v[0][3]=7; nbv[0]=4;
  v[1][0]=0; v[1][1]=4;v[1][2]=7;nbv[1]=3;
  v[2][0]=0;nbv[2]=1;
  v[3][0]=4;v[3][1]=5;v[3][2]=6;nbv[3]=3;
  v[4][0]=1; v[4][1]=3;v[4][2]=6; v[4][3]=7; nbv[4]=4;
  v[5][0]=0;v[5][1]=3;v[5][2]=6; nbv[5]=3;
  v[6][0]=3;v[6][1]=4;v[6][2]=5;nbv[6]=3;
  v[7][0]=0;v[7][1]=1;v[7][2]=4;nbv[7]=3;

  xe[0] = 200; ye[0] = 200; xe[1] = 100; ye[1] = 300;
  xe[2] = 350; ye[2] = 200;  xe[3] = 300; ye[3] = 400;
  xe[4] = 150; ye[4] = 400;  xe[5] = 350; ye[5] = 300;
  xe[6] = 250; ye[6] = 350;  xe[7] = 200; ye[7] = 300;

  memset(dejavus, 0, sizeof(int*));
  F[0]=depart;
  dejavus[depart]=OUI;
 

  /* Dessin du graphe */
  int i,j;
  for(i=0;i<NS;i++) {
    filldisc(ecran, xe[i],ye[i],2,couleur[2]);
    for(j=0;j<=nbv[i];j++)
      if(i<v[i][j])
	line(ecran, xe[i],ye[i],xe[v[i][j]],ye[v[i][j]],couleur[1]);
  }

  explorer_largeur(ecran);
  SDL_BlitSurface(texte, NULL, ecran, &pos_texte); /*blit du texte*/
  SDL_Flip(ecran);
  pause();  
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
