#include "bibosdl.h"

#define LARGEUR 800 
#define HAUTEUR 600
#define NS 20 
#define NORME 200

Uint32 couleur[100];
const int xorig = LARGEUR / 2; 
const int yorig = HAUTEUR / 2; 
int v[NS][NS], nbv[NS], x[NS], y[NS]; 
float anglex[NS], angley[NS];

void dessine_graphe(SDL_Surface* ecran) {
  int i, j;
  filldisc(ecran, xorig, yorig, 4, couleur[2]);
  //Affichage du graphe sur l'écran
  for(i = 0; i < NS; i++) {
    filldisc(ecran, x[i], y[i], 6, couleur[2]);
    for (j = 0; j < nbv[i]; j++)
      line(ecran, x[i], y[i], x[v[i][j]], y[v[i][j]], couleur[2]);
  }
  SDL_Flip(ecran);
}

long nombre_chemins(int compteur, int nombre_sommets, int total) {
  if(nombre_sommets == NS) {
    printf("%d sommets --> %d chemins\n", nombre_sommets, total);
    return total;
  }
  return nombre_chemins(compteur+1, nombre_sommets+1, ((compteur+1) * total) + 1);
}


int main() {
  SDL_Surface* ecran = NULL;
  int i,j; 
  SDL_Init(SDL_INIT_VIDEO);

  ecran = SDL_SetVideoMode(LARGEUR,HAUTEUR,32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Affichage Graphe Complet",NULL);

  //Initialisation des couleurs
  couleur[0] = SDL_MapRGB(ecran->format, 255, 255, 255); //blanc
  couleur[1] = SDL_MapRGB(ecran->format, 0, 0, 255);     //bleu
  couleur[2] = SDL_MapRGB(ecran->format, 0, 0, 0);       //noir
  couleur[3] = SDL_MapRGB(ecran->format, 255, 0, 0);     //rouge
  couleur[4] = SDL_MapRGB(ecran->format, 0, 255, 0);     //vert

  SDL_FillRect(ecran, NULL, couleur[0]);
  SDL_Flip(ecran);
  
  
  
  //Initialisation de la liste d'adjacence pour chaque sommet
  for(i = 0; i < NS; i++) 
    for(j = 0; j < NS; j++) v[i][j] = j;

  //Dans un graphe complet, le nb de voisins d'un sommet est toujours N-1
  for(i = 0; i < NS; i++) nbv[i] = NS - 1;
  
  //Calcul  des angles
  for(i = 0; i < NS; i++) {
    anglex[i] = cos((float)i * 2.0 * M_PI / (float) NS);
    angley[i] = sin((float)i * 2.0 * M_PI / (float) NS);
  }
  
  //Calcul des coordonnées des sommets sur l'écran
  for(i = 0; i < NS; i++) {
    x[i] = xorig + NORME * anglex[i];
    y[i] = yorig - NORME * angley[i];
  }
  
  dessine_graphe(ecran);
  nombre_chemins(1, 3, 2); //Fonction qui affiche le nb total des chemins dans le graphe dans le terminal
  pause();
  SDL_FreeSurface(ecran);
  SDL_Quit();
  return EXIT_SUCCESS;
}
