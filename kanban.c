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
		exit(0);
		printf("thread num %d a terminé\n", num);
		pthread_exit(NULL);
}

travaille(int num)
{
	pthread_mutex_lock(&mutex2);
	printf("son pere\n");
	usleep(2000);
	pthread_mutex_unlock(&mutex2);
		if (num == 0){
						printf("yolo2\n");
				aval(num);
		}
		else if (num == nbAteliers-1){
						printf("yolo2\n");
				amont(num);
						//sleep(10);
		}
		else{
						printf("yolo2\n");
						//sleep(10);
				intermediaire(num);
		}
}

intermediaire(int num){
				printf("yolo2\n");
		int pieceConteneurFini = 0;
		pthread_mutex_lock(&mutex2);
		while(arretAtelierBoolean[num] == false) { // variable a changer quand on a fini de tout construire
			pthread_mutex_unlock(&mutex2);
			printf("test1\n");
			pthread_mutex_lock(&mutex2);
				while(tableauDeLancement[num]!=0){
					pthread_mutex_unlock(&mutex2);
					printf("test2\n");
						while (tabMaxConteneurAtelier[num-1] > pieceConteneurFini){
							printf("\n\n");

							pthread_mutex_lock(&mutex2);
							printf("stock : %i\n", stock[num][0]);
							printf("stock 1 : %i\n", stock[num][1]);
								if (stock[num][0]!= 0){
									pthread_mutex_unlock(&mutex2);
									printf("contruire interm\n");
										construire(num);
										pieceConteneurFini++;
										printf("\ndebug fin\n");
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
				printf("j'attend ********************************************\n");
				pthread_cond_wait(&attendre[num], &mutex2);
				printf("j'attend ******************************************** plusssssssss\n");
				pthread_mutex_unlock(&mutex2);
				pthread_mutex_lock(&mutex2);
				printf("enfoiré\n");
		}
		pthread_mutex_unlock(&mutex2);
		// donne l'ordre d'arrêt a l'atelier suivant
		arretAtelierBoolean[num+1] = true;
}

aval(int num){
  		pthread_mutex_lock(&mutex2);
		while(tableauDeLancement[num]!=0){
      		pthread_mutex_unlock(&mutex2);
			printf("stock : %i\n", stock[num][0]);
			printf("stock 1 : %i\n", stock[num][1]);
     		 pthread_mutex_lock(&mutex2);
				if (stock[num][0]!= 0){
          			pthread_mutex_unlock(&mutex2);
					printf("contruire aval\n");
						construire(num);
					livraisonHommeFlux(num);
				}
				else if (stock[num][0]==0 && stock[num][1]!=0){
						printf("lol\n");
						stock[num][0]=stock[num][1];
						printf("stock changer : %i\n", stock[num][0]);
						stock[num][1]=0;
            			pthread_mutex_unlock(&mutex2);
						//printf("lol\n");
						demandeConteneurHommeFlux(num);
						sleep(1);
				}
				else{
					printf("demande anticipé\n");
						pthread_mutex_unlock(&mutex2);
						demandeConteneurHommeFlux(num);
						printf("wait\n");
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

amont(int num){
				printf("yolo2\n");
		int pieceConteneurFini = 0;
		while(arretAtelierBoolean[num] == false) {
				while(tableauDeLancement[num]!=0){
						while (tabMaxConteneurAtelier[num-1] > pieceConteneurFini){
							printf("contruire amont\n");
								construire(num);
								pieceConteneurFini++;
						}
						livraisonHommeFlux(num);
						pieceConteneurFini = 0;
				}
				pthread_cond_wait(&attendre[num], &mutex2);
		}
}

construire(int num){
	//printf("const\n");
	printf("num : %i\n", num);
		if (num != nbAteliers-1){
				stock[num][0] = stock[num][0]-1;
		}
		printf("debug404\n");
		sleep(tabTempsAteliers[num]);
}

demandeConteneurHommeFlux(int num){
	printf("demande\n");
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_lock(&mutex2);
	printf("conard\n");
	tableauDeLancement[num+1]++;
	usleep(200);
	printf("salope\n");
	pthread_mutex_unlock(&mutex2);

	pthread_cond_signal(&attendre[num+1]);
}

livraisonHommeFlux(int num){
	printf("livraison\n");
	if (num!=0){
		stock[num-1][1] = tabMaxConteneurAtelier[num-1];
		tableauDeLancement[num]--;
		pthread_cond_signal(&livraison[num-1]);
	}
	else{
		nbPiecesConstruites++;	
pthread_mutex_lock(&mutex2);
		tableauDeLancement[num]--;
		pthread_mutex_unlock(&mutex2);
	}
		
	printf("tableau : %i\n",tableauDeLancement[num]);
	pthread_mutex_unlock(&mutex2);
	
}

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

		printf("yolo\n");

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

pointeurAnnihilation()
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
