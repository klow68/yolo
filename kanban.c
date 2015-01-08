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
int nbAteliers = 3;
int *tableauDeLancement;
int *tabTempsAteliers;
int booleanTempsProd = true;
int tempsProd = 1;
int booleanNbPiecesCommun = true;
int nbPiecesParConteneur = 10;
int nbPiecesConstruites = 0;
int *tabMaxConteneurAtelier;
int *arretAtelierBoolean;
int **stock;

/* Déclaration variables moniteur */
pthread_t *tabAteliers;
pthread_mutex_t mutex,mutex2;
pthread_cond_t *livraison, *attendre, threadCrees;


/*                    */
erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

/************************** Initialisation **************************/
/*

initialisation du nombres d'atelier, du temps de production etc...

*/
initConf()
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

/*

	initialisation du temp de production des ateliers

*/
initTempsProd()
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

/*

	initialisation du nombres de pièce par conteneur pour chaques ateliers

*/
initNbPiecesConteneur()
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

/************************** Ateliers **************************/


/*
	Premier pas dans la vie de notre petit thread tout mignon qui viens de n'être
*/

*AfficheEtat(void *data)
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
				//tabNbPiecesAttente[0] = nbPiecesAProduire;
				pthread_cond_wait(&threadCrees, &mutex);
				sleep(1);
				printf("\n*************************** Usine Crée ***************************\n");
				int i;
				
		}

		travaille(num);
		// pour le moment
		printf("thread num %d a terminé\n", num);
		pthread_exit(NULL);
}

/*
	Au bout d'un moment il faut bien travailler et comme dans un régime communiste a chaqu'un ça place

	num : 0 -> aval
		  1/(n-2) -> intermédiaire
		  n-1 -> amont
*/
travaille(int num)
{
		if (num == 0){
				aval(num);
		}
		else if (num == nbAteliers-1){
				amont(num);
		}
		else{
				intermediaire(num);
		}
}

/*
	Travaille de l'atelier intermédiaire
	arretAtelierBoolean[num] est a true lors de la fermeture de l'usine
	
	while(tableauDeLancement[num]!=0) Tant qu'on a des commandes on ne s'arrête pas

	if (stock[num][0]!= 0) si on a du stock on construit

	else if (stock[num][0]==0 && stock[num][1]!=0) si l'un des deux conteneur est vide on les échange pour n'utiliser a chaque fois que le premier

	else on a plus de stock alors on attend une livraison

*/
intermediaire(int num){
		int pieceConteneurFini = 0;
		pthread_mutex_lock(&mutex2);
		while(arretAtelierBoolean[num] == false) { // variable a changer quand on a fini de tout construire
			pthread_mutex_unlock(&mutex2);
			pthread_mutex_lock(&mutex2);
				while(tableauDeLancement[num]!=0){
					pthread_mutex_unlock(&mutex2);

						while (tabMaxConteneurAtelier[num-1] > pieceConteneurFini){
							printf("\n------Atelier numéro %i\n", num);

							pthread_mutex_lock(&mutex2);
							printf("stock 0 : %i\n", stock[num][0]);
							printf("stock 1 : %i\n", stock[num][1]);
								if (stock[num][0]!= 0){
									pthread_mutex_unlock(&mutex2);
									printf("\nconstruction intermédiaire\n");
										construire(num);
										pieceConteneurFini++;
										pthread_mutex_unlock(&mutex2);
								}
								else if (stock[num][0]==0 && stock[num][1]!=0){
										stock[num][0] = stock[num][1];
										stock[num][1] = 0;
										pthread_mutex_unlock(&mutex2);
										demandeConteneurHommeFlux(num);
								}
								else{
										pthread_mutex_unlock(&mutex2);
										demandeConteneurHommeFlux(num);
										pthread_cond_wait(&livraison[num], &mutex2);
								}
						}
						livraisonHommeFlux(num);
						pieceConteneurFini = 0;

						pthread_mutex_lock(&mutex2);
				}
				pthread_mutex_unlock(&mutex2);
				pthread_cond_wait(&attendre[num], &mutex2);
				pthread_mutex_unlock(&mutex2);
				pthread_mutex_lock(&mutex2);
		}
		pthread_mutex_unlock(&mutex2);
		// donne l'ordre d'arrêt a l'atelier suivant
		arretAtelierBoolean[num+1] = true;
}


/*
	Travaille de l'atelier en aval
	
	while(tableauDeLancement[num]!=0) Tant qu'on a des commandes on ne s'arrête pas

	if (stock[num][0]!= 0) si on a du stock on construit

	else if (stock[num][0]==0 && stock[num][1]!=0) si l'un des deux conteneur est vide on les échange pour n'utiliser a chaque fois que le premier

	else on a plus de stock alors on attend

*/
aval(int num){
  		pthread_mutex_lock(&mutex2);
		while(tableauDeLancement[num]!=0){
			printf("\n------Atelier numéro %i\n", num);

      		pthread_mutex_unlock(&mutex2);
			printf("stock 0 : %i\n", stock[num][0]);
			printf("stock 1 : %i\n", stock[num][1]);
     		 pthread_mutex_lock(&mutex2);
				if (stock[num][0]!= 0){
          			pthread_mutex_unlock(&mutex2);
					printf("construction aval\n");
						construire(num);
					livraisonHommeFlux(num);
				}
				else if (stock[num][0]==0 && stock[num][1]!=0){
						stock[num][0]=stock[num][1];
						stock[num][1]=0;
            			pthread_mutex_unlock(&mutex2);
						demandeConteneurHommeFlux(num);
						sleep(1);
				}
				else{
						pthread_mutex_unlock(&mutex2);
						demandeConteneurHommeFlux(num);
						sleep(2);
						pthread_cond_wait(&livraison[num], &mutex2);
				}


				pthread_mutex_lock(&mutex2);
		}
		pthread_mutex_unlock(&mutex2);

    	printf("nb construc terminer : %i\n",nbPiecesConstruites);
    	pthread_mutex_lock(&mutex2);
		arretAtelierBoolean[num+1] = true;
  		pthread_mutex_unlock(&mutex2);
}

/*
	Travaille de l'atelier en amont
	arretAtelierBoolean[num] est a true lors de la fermeture de l'usine
	
	while(tableauDeLancement[num]!=0) Tant qu'on a des commandes on ne s'arrête pas

	on construit sans regarder le stock car il est ilimité

*/
amont(int num){
		int pieceConteneurFini = 0;
		pthread_mutex_lock(&mutex2);
		while(arretAtelierBoolean[num] == false) {
				while(tableauDeLancement[num]!=0){
					pthread_mutex_unlock(&mutex2);
					printf("\n------Atelier numéro %i\n\n", num);
						while (tabMaxConteneurAtelier[num-1] > pieceConteneurFini){
							printf("construction amont\n");
								construire(num);
								pieceConteneurFini++;
						}
						livraisonHommeFlux(num);
						pieceConteneurFini = 0;
				}
				pthread_cond_wait(&attendre[num], &mutex2);
				pthread_mutex_lock(&mutex2);
		}
		pthread_mutex_unlock(&mutex2);
}


/*
	contruit et retire le stock utilisé si nécessaire
*/
construire(int num){
	//printf("const\n");
	printf("# L'atelier numéro %i construit une pièce\n",num);
	pthread_mutex_unlock(&mutex2);	
	pthread_mutex_lock(&mutex2);
		if (num != nbAteliers-1){
				stock[num][0] = stock[num][0]-1;
		}
	pthread_mutex_unlock(&mutex2);	
		sleep(tabTempsAteliers[num]);
}

/*
	demande un conteneur a l'homme flux qui va transmettre la requête a l'atelier concerner
*/
demandeConteneurHommeFlux(int num){
	printf("~ L'atelier numéro %i demande un conteneur\n",num);
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_lock(&mutex2);
	tableauDeLancement[num+1]++;
	usleep(200);
	pthread_mutex_unlock(&mutex2);

	pthread_cond_signal(&attendre[num+1]);
}

/*
	livraison d'un conteneur par l'homme flux
*/
livraisonHommeFlux(int num){
	printf("& L'atelier numéro %i envoie un conteneur\n",num);
	pthread_mutex_lock(&mutex2);
	if (num!=0){
		stock[num-1][1] = tabMaxConteneurAtelier[num-1];
		tableauDeLancement[num]--;
		pthread_cond_signal(&livraison[num-1]);
	}
	else{
		nbPiecesConstruites++;
		tableauDeLancement[num]--;
		printf("Pièce final construites ************************* [%i/%i]\n",nbPiecesConstruites,nbPiecesAProduire );
	}
	pthread_mutex_unlock(&mutex2);
		
	printf("Tableau de lancement : %i\n",tableauDeLancement[num]);
	pthread_mutex_unlock(&mutex2);
	
}

/*
	initialisation de l'usine (mémoire dynamique etc)
*/
initUsine()
{
		int i, j, k, rc;

		// attribution de mémoire de manière dynamique au différent tableau
		tabAteliers = (pthread_t *) malloc(nbAteliers * sizeof(pthread_t));
		tabTempsAteliers = (int *) malloc(nbAteliers * sizeof(int));
		tabMaxConteneurAtelier = (int *) malloc(nbAteliers * sizeof(int));
		tableauDeLancement = (int * ) malloc(nbAteliers * sizeof(int));
		attendre = (pthread_cond_t * ) malloc(nbAteliers * sizeof(pthread_cond_t));
		livraison = (pthread_cond_t * ) malloc(nbAteliers * sizeof(pthread_cond_t));
		stock = (int **) malloc(nbAteliers * sizeof(*stock));
		arretAtelierBoolean = (int * ) malloc(nbAteliers * sizeof(int));

		for (i = 0; i < nbAteliers; ++i)
		{
				// 2 conteneur
				stock[i] = (int *) malloc(2 * sizeof(*(stock[i])));
		}

		//initialisation boolean de fermeture d'atelier
		for(i = 0; i<nbAteliers; i++){
			arretAtelierBoolean[i]=false;
			// init tableauDeLancement
			tableauDeLancement[i]=0;
			// init conditions
			pthread_cond_init(&livraison[i], NULL);
			pthread_cond_init(&attendre[i], NULL);
		}

		tableauDeLancement[0]=nbPiecesAProduire;

		// init mutex et conditions
		//pthread_mutex_init(&mutex, NULL);
		pthread_mutex_init(&mutex2, NULL);
		pthread_cond_init(&threadCrees, NULL);

		// TODO A changer
		for (i = 0; i < nbAteliers; ++i)
		{
			stock[i][0]=5;
			stock[i][1]=3;
		}

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

/*
	libération de la mémoire des ressources utilisé
*/
pointeurAnnihilation()
{
		printf("\n");
		printf("Libération des ressources ...");

		free(tabAteliers);
		free(tabTempsAteliers);
		free(tabMaxConteneurAtelier);
		free(tableauDeLancement);
		free(attendre);
		free(livraison);
		free(stock);
		free(arretAtelierBoolean);

		printf("OK.\n");
}


/*
	traitement du signal de la touche ctrl-c pour fermer l'usine proprement
*/
traitant_SIGINT(){
  pointeurAnnihilation();
  printf("\nFermeture de l'usine MeinCroft\n");
  exit(0);
}

int main(int argc, char *argv[])
{
		signal(SIGINT,traitant_SIGINT);

		initConf();

		pointeurAnnihilation();

		printf("\nFermeture de l'usine MeinCroft\n");

		exit(0);
}
