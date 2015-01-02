#include <stdio.h>
#include <pthread.h>

/* Déclaration des variables globales */
int nbPiecesAProduire, nbAteliers;
int *tabTempsAteliers;
int choix;

/* Déclaration variables moniteur */
pthread_t *tabAteliers;
pthread_mutex_t mutex;
pthread_cond_t produire, attendre;


/*                    */
void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

void initConf()
{
  printf("Bienvenue dans la configuration du programme Kanban.\n");
  printf("1 - Nombre de pièces à produire (Actuel : %d)\n", nbPiecesAProduire);
  printf("2 - Nombre d'ateliers (Actuel : %d)\n", nbAteliers);
  printf("3 - Valider configuration\n");
  printf("4 - Configuration automatique -> 5 ateliers, 10 pièces, temps production 3s ");
  printf("Choix : ");
  scanf("%d", &choix);

  switch(choix)
  {
    case 1: 
      printf("Valeur : ");
      scanf("%d", &nbPiecesAProduire);
      initConf();
      break;

    case 2:
      printf("Valeur : ");
      scanf("%d", &nbAteliers);
      initConf();
      break;

    case 3:
      printf("Configuration terminée.\n");
      initUsine();
      break;

    default: initConf();
    break;

  }
}

void initUsine()
{
  int i;
  tabAteliers = (pthread_t *) malloc(nbAteliers * sizeof(pthread_t));
  tabTempsAteliers = (int *) malloc(nbAteliers * sizeof(int));
  
  
  if (choix != 4){
    printf("Configuration temps de production des ateliers\n");
    for (int i=0;i<nbAteliers;i++){
        printf("Temps de production atelier n°%d : ", i+1);
        scanf("%d",tabTempsAteliers[i]);
    }
  }

}

void pointeurAnnihilation()
{
  printf("Libération des ressources ...");
  free(tabAteliers);
  free(tabTempsAteliers);
  printf("OK.\n");
}



int main(int argc, char *argv[])
{

    nbPiecesAProduire = 10;
    nbAteliers = 5;

    initConf(); // si pas de conf => valeur tempsProd par defaut, commune a tous les ateliers [A FAIRE]

    pointeurAnnihilation();

}