/*
Algorithmique Avancée L3 - 2013/2014
Nom : Nouira
Prénom : Chafik
N° étudiant : 11291558
Programme : Exploration en profondeur et en largeur d'un graphe - Séance du 25 octobre 2013
*/
#include "bibosdl.h"

#define NS 8
int dejavu[NS], nbv[NS], v[NS][NS], x[NS], y[NS];

Uint32 couleur[3];

void exploration_profondeur(SDL_Surface* ecran, int i) {
  int j,voisin;
  dejavu[i] = 1;
  for(j = 0; j < nbv[i]; j++){
    voisin = v[i][j];
    if(dejavu[voisin] == 0){
      linewithwidth(ecran,x[i], y[i], x[voisin], y[voisin], 3,couleur[2]);
      SDL_Flip(ecran);
      pause(); 
      exploration_profondeur(ecran, voisin);
    }
  }
}

void exploration_largeur(SDL_Surface *ecran, int ptdepart){
  int L = 1;
  dejavu[ptdepart] = 1;
  int f[NS];
  int tete, k, j, voisin;
  f[0] = ptdepart;
  while(L != 0){
    tete = f[0];
    for(j = 0; j < nbv[tete]; j++){
      voisin = v[tete][j];
      if(dejavu[voisin] == 0){
	f[L] = voisin;
	dejavu[voisin] = 1;	
	L++;
	linewithwidth(ecran, x[tete], y[tete], x[voisin], y[voisin], 3,couleur[2]);	
	SDL_Flip(ecran);
	pause();      
      }
    }
    for(k = 0; k < L-1; k++){
      f[k] = f[k+1];
    }
    L--;
  }
}

int main(){
  SDL_Surface *ecran;
  int i, j, ptdepart, choix = 0;
  SDL_Init(SDL_INIT_VIDEO);
  ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  couleur[0] = SDL_MapRGB(ecran->format,255,255,255); //Blanc
  couleur[1] = SDL_MapRGB(ecran->format,0,0,0); //Noir
  couleur[2] = SDL_MapRGB(ecran->format,255,0,0); //Rouge
  SDL_FillRect(ecran, NULL, couleur[0]);

  for(i = 0; i < NS; i++) dejavu[i]=0;       
  ptdepart = 0;

  //Initialisation du graphe
  v[0][0] = 1; v[0][1] = 2; v[0][2] = 5; v[0][3] = 7; nbv[0] = 4;
  v[1][0] = 0; v[1][1] = 4; v[1][2] = 7; nbv[1] = 3;
  v[2][0] = 0; nbv[2] = 1;
  v[3][0] = 4; v[3][1] = 5; v[3][2] = 6; nbv[3] = 3;
  v[4][0] = 1; v[4][1] = 3; v[4][2] = 6; v[4][3] = 7; nbv[4] = 4;
  v[5][0] = 0; v[5][1] = 3; v[5][2] = 6; nbv[5] = 3;
  v[6][0] = 3; v[6][1] = 4; v[6][2] = 5; nbv[6] = 3;
  v[7][0] = 0; v[7][1] = 1; v[7][2] = 4; nbv[7] = 3;

  //Initialisation des coordonnées des sommets sur l'écran
  x[0]= 400 ,y[0]= 50; x[1]= 200  ,y[1]= 150;
  x[2]= 600 ,y[2]= 50; x[3]= 500 ,y[3]= 270;
  x[4]= 350 ,y[4]= 400; x[5]= 600 ,y[5]= 150;
  x[6]= 600 ,y[6]= 250; x[7]= 450 ,y[7]= 160;
 
  for(i = 0; i < NS; i++) filldisc(ecran, x[i], y[i], 5, couleur[2]);

  for(i = 0; i < NS; i++){
    for(j = 0; j < nbv[i]; j++)
      line(ecran,x[i], y[i], x[v[i][j]], y[v[i][j]], couleur[1]);
  }

  SDL_Flip(ecran);
  do {
    system("clear");
    printf("1 - Exploration en profondeur\n2 - Exploration en largeur\nVotre choix : ");
    scanf("%d",&choix);
  } while(choix != 1 && choix != 2);
  if(choix == 1) {
    SDL_WM_SetCaption("Exploration en profondeur d'un graphe", NULL);
    pause();
    exploration_profondeur(ecran,ptdepart);
  }
  else {
    SDL_WM_SetCaption("Exploration en largeur d'un graphe", NULL);
    pause();
    exploration_largeur(ecran,ptdepart);
  }

  SDL_Quit();    
  return EXIT_SUCCESS;
}
