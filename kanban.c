#include <stdio.h>
#include <pthread.h>

# define TIMEPROD 


/* Déclaration des variables globales */
int nbPiecesAProduire, nbAteliers;
int *tabTempsAteliers;



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

  int choix;

  printf("Bienvenue dans la configuration du programme Kanban.\n");
  printf("1 - Nombre de pièces à produire (Actuel : %d)\n", nbPiecesAProduire);
  printf("2 - Nombre d'ateliers (Actuel : %d)\n", nbAteliers);
  printf("3 - Valider configuration\n");
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

  tabAteliers = (pthread_t *) malloc(nbAteliers * sizeof(pthread_t));
  tabTempsAteliers = (int *) malloc(nbAteliers * sizeof(int));

}

pointeurAnnihilation()
{
  printf("Libération des ressources ...");
  free(tabAteliers);
  free(tabTempsAteliers);
  printf("OK.")
}



int main(int argc, char *argv[])
{

    nbPiecesAProduire = 10;
    nbAteliers = 5;

    initConf();

    pointeurAnnihilation();

}