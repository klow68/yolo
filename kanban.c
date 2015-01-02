#include <stdio.h>
#include <pthread.h>

#define true 1
#define false 0

/* Déclaration des variables globales */
int nbPiecesAProduire, nbAteliers;
int *tabTempsAteliers;
int booleanTempsProd = true;
int tempsProd = 3;

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

  char* stempsAuto = (booleanTempsProd == false ? "Désactivé" : "Activé");
  printf("3 - Temps de production commun (Actuel : %s - %d seconde(s))\n", stempsAuto, tempsProd);
  printf("4 - Valider configuration\n");
  printf("Choix : ");
  scanf("%d", &choix);
  printf("\n");

  switch(choix)
  {
    case 1: 
      printf("Valeur : ");
      scanf("%d", &nbPiecesAProduire);
      printf("\n");
      initConf();
      break;

    case 2:
      printf("Valeur : ");
      scanf("%d", &nbAteliers);
      printf("\n");
      initConf();
      break;

    case 3:
      initTempsProd();
      initConf();
      break;

    case 4:
      printf("Configuration terminée.\n");
      initUsine();
      break;

    default: initConf();
    break;

  }
}

void initTempsProd()
{
  int choix;

  printf("Gestion du temps de production : \n");
  char* action = (booleanTempsProd == false ? "activer" : "désactiver");
  printf("1 - %s\n", action);
  if(booleanTempsProd == true) 
  {
    printf("2 - Modifier le temps de production commun\n");
    printf("3 - Valider\n");
  } else {
    printf("2 - Valider\n");
  }

  printf("Choix : ");
  scanf("%d", &choix);
  printf("\n");

  switch(choix)
  {
    case 1: booleanTempsProd = (booleanTempsProd == false ? true : false); initTempsProd(); break;

    case 2:
      if(booleanTempsProd == true) {
        printf("Valeur : ");
        scanf("%d", &tempsProd);
        printf("\n");
        initTempsProd();
      } else {
        printf("Choix validé.\n");
        initConf();
      }
      
      break;

    case 3:
      printf("Choix validé.\n");
      initConf();
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
  

  if (booleanTempsProd == false){
    printf("Configuration manuelle des temps de production des ateliers\n");
    for (i = 0; i < nbAteliers; i++){
        printf("Temps de production atelier n°%d : ", i+1);
        scanf("%d",tabTempsAteliers + i);
    }
  } else {
    for (i = 0; i < nbAteliers; i++){
        tabTempsAteliers[i] = tempsProd;
    }
  }

  printf("\n");
  printf("Affichage du tableau des temps : \n");

  for (i = 0; i < nbAteliers; i++){
    printf("Atelier n°%d : %d seconde(s)\n", i, tabTempsAteliers[i]);
  }
}

void pointeurAnnihilation()
{
  printf("\n");
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

    printf("\nFermeture de l'usine MeinCroft\n");

}