#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define true 1
#define false 0

/* Déclaration des variables globales */
int nbPiecesAProduire, nbAteliers;
int *tabNbPiecesAttente;
int *tabTempsAteliers;
int booleanTempsProd = true;
int tempsProd = 3;

/* Déclaration variables moniteur */
pthread_t *tabAteliers;
pthread_mutex_t mutex;
pthread_cond_t *produire, *attendre, threadCrees;


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

  char* stempsAuto = (booleanTempsProd == false ? "désactivé" : "activé");
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
  char* action = (booleanTempsProd == false ? "Activer" : "Désactiver");
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



void *AfficheEtat(void *data)
{

    long num;
    num = (long) data;
    printf("\nJe suis le thread #%ld \n", num);
    printf("de TID : %ld \n", (long) pthread_self());
    fflush(stdout);

    //wait signal tous les thread on été crée
    //uniquement pour l'atelier en aval
    if(num == 0) 
    {
      tabNbPiecesAttente[0] = nbPiecesAProduire;
      pthread_cond_wait(&threadCrees, &mutex);
      sleep(1);
      printf("\n*************************** Usine Crée ***************************\n");
    }
    travaille(num);

    printf("thread num %d a terminer\n", num);
    pthread_exit(NULL);
}

void travaille(int num)
{
  // ne marche pas pourquoi? ^^
  printf("\nthread num : %d travaille\n", num);
  if (tabNbPiecesAttente[num] == 0){
    pthread_cond_wait(&attendre[num], &mutex);
  }
  pthread_cond_signal(&attendre[num+1]);

}

void initUsine()
{

  int i, j, k, rc;

  // attribution de mémoire de manière dynamique au différent tableau
  tabAteliers = (pthread_t *) malloc(nbAteliers * sizeof(pthread_t));
  tabTempsAteliers = (int *) malloc(nbAteliers * sizeof(int));
  tabNbPiecesAttente = (int * ) malloc(nbAteliers * sizeof(int));
  attendre = (pthread_cond_t * ) malloc(nbAteliers * sizeof(pthread_cond_t));
  produire = (pthread_cond_t * ) malloc(nbAteliers * sizeof(pthread_cond_t));
  

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

  // Création des ateliers avec attribution d'un numéro
  for (j = 0; j < nbAteliers; j++) {
  rc = pthread_create(&tabAteliers[j], 0, &AfficheEtat, (void *) j);
  usleep(30000);
  if(rc != 0)
      erreur("Erreur Creation thread");
      //usleep(30000);
  }


  /****************** Signal tous les thread sont crée début du travaille ****************/
  pthread_cond_signal(&threadCrees);

  for(k = 0; k < nbAteliers; k++) {
    // on fait dans boucle après pour éviter des désynchro car terminaison peut etre (très) rapide
    pthread_join(tabAteliers[k], NULL); // idem wait
  }

  //printf("\nJE SUIS APRES LES JOINS YOLO !");

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

    initConf();

    pointeurAnnihilation();



    printf("\nFermeture de l'usine MeinCroft\n");

}