#include <stdio.h>
#include <pthread.h>

# define TIMEPROD 


/* Déclaration des variables globales */
int nbPiecesAProduire, nbAteliers;



/* Déclaration variables moniteur */
pthread_t *tab_ateliers;
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
  scanf("%d", choix);

  switch(choix)
  {
    case 1: 
      printf("Valeur : ");
      scanf("%d", nbPiecesAProduire);
      initConf();

    case 2:
      printf("Valeur : ");
      scanf("%d", nbAteliers);
      initConf();

    case 3:
      printf("Configuration terminée.");
      break;

    default: initConf();

  }
}



int main(int argc, char *argv[])
{

    nbPiecesAProduire = 10;
    nbAteliers = 5;

    initConf();

}