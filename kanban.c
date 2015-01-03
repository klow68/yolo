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
int nbPiecesAProduire = 10;
int nbAteliers = 5;
int *tabNbPiecesAttente;
int *tabTempsAteliers;
int booleanTempsProd = true;
int tempsProd = 1;
int booleanNbPiecesCommun = true;
int nbPiecesParConteneur = 10;
int nbPiecesConstruites = 0;
int *tabMaxConteneurAtelier;

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

  char* sNbPieces = (booleanNbPiecesCommun == false ? "désactivé" : "activé");
  printf("4 - Nombre de pieces/conteneur commun (Actuel : %s - %d pièce(s))\n", sNbPieces, nbPiecesParConteneur);

  printf("5 - Valider configuration\n");
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
      initNbPiecesConteneur();
      break;

    case 5:
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


void initNbPiecesConteneur()
{
  int choix;

  printf("Gestion du nombre de pieces par conteneur par atelier : \n");
  char* action = (booleanNbPiecesCommun == false ? "Activer" : "Désactiver");
  printf("1 - %s\n", action);
  if(booleanNbPiecesCommun == true) 
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
    case 1: booleanNbPiecesCommun = (booleanNbPiecesCommun == false ? true : false); initNbPiecesConteneur(); break;

    case 2:
      if(booleanNbPiecesCommun == true) {
        printf("Valeur : ");
        scanf("%d", &nbPiecesParConteneur);
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

    printf("thread num %d a terminé\n", num);
    pthread_exit(NULL);
}

void travaille(int num)
{
  // tant qu'il reste des pièce a produire (il faudrat un mutex sur la variable)
  while(tabNbPiecesAttente[0]!=0){
    //printf("thread num : %d travaille\n", num);
    if (tabNbPiecesAttente[num] == 0){
      //printf("Dors : %d\n", num);
      pthread_cond_wait(&attendre[num], &mutex);
    }
    printf("thread num %d c'est réveillé\n", num);
    pthread_cond_signal(&attendre[num+1]);
    if (num != nbAteliers-1){
      pthread_cond_wait(&produire[num], &mutex);
    }
    printf("\nl'atelier n°%d produit une piece", num);
    construire(num);
    if (num != 0){
      pthread_cond_signal(&produire[num-1]);
    } else {
      nbPiecesConstruites ++;
      printf("\n\n######## Nombre de pieces produites : [%d/%d] ########\n\n", nbPiecesConstruites, nbPiecesAProduire);
      tabNbPiecesAttente[num] --;
    }
  }
}

void construire(int num){
  sleep(tabTempsAteliers[num]);
}

void initUsine()
{

  int i, j, k, rc;

  // attribution de mémoire de manière dynamique au différent tableau
  tabAteliers = (pthread_t *) malloc(nbAteliers * sizeof(pthread_t));
  tabTempsAteliers = (int *) malloc(nbAteliers * sizeof(int));
  tabMaxConteneurAtelier = (int *) malloc(nbAteliers * sizeof(int));
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

  if(booleanNbPiecesCommun == false)
  {
    printf("Configuration manuelle du nombre de pieces par conteneur par atelier\n");
    for (i = 0; i < nbAteliers; i++){
        printf("Capacité du conteneur de l'atelier n°%d : ", i+1);
        scanf("%d",tabMaxConteneurAtelier + i);
    }
  } else {
    for (i = 0; i < nbAteliers; i++){
        tabMaxConteneurAtelier[i] = nbPiecesParConteneur;
    }
  }

  printf("\n");
  printf("Affichage du tableau des temps : \n\n");

  for (i = 0; i < nbAteliers; i++){
    printf("Atelier n°%d : %d seconde(s)\n", i, tabTempsAteliers[i]);
  }

  printf("\nAffichage du tableau des capacités : \n\n");

  for (i = 0; i < nbAteliers; i++){
    printf("Atelier n°%d : %d pièce(s)/conteneur\n", i, tabMaxConteneurAtelier[i]);
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

    initConf();

    pointeurAnnihilation();

    printf("\nFermeture de l'usine MeinCroft\n");

}