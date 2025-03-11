#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>

typedef struct {
 int minute;
 int heure;
 int jour;
} Horaire ;

typedef struct {
 int id;
 Horaire horaireDateSouhaite;
 Horaire dureeHeureMinute;
 int typeActivite;
 int idZoneSrc;
 int idZoneDest;
 int idLigne;
 int sens;
} Activite ;

typedef struct {
 int cin;
 int idZoneHabitat;
 int idZoneTravail;
 int idZoneCourante;
 int intentionVoyage;
 int idLigneCourante;
 int sensCourant;
 int idZoneDest;
 int nombreActivites;
 Activite activites[8];
} Personne ;

typedef struct {
 int id;
 int row;
 int column;
 int nombrePersonnes;
 Personne personnes[100];
} Zone ;

typedef struct {
 int row;
 int column;
 int codeRue;
} Subdivision ;

typedef struct {
 int idZoneSrc;
 int idZoneDest;
 int distance;
 Horaire dureeApproximative;
 int nombresubdivisions;
 Subdivision parcours[20];
} Troncon ;

typedef struct {
 int id;
 int nombreZones;
 int idZones[10];
} Ligne ;

typedef struct {
 int id;
 int idLigne;
 int sens;
 Horaire horaireDateDepart;
} Tache ;

typedef struct {
 int zoneSrc;
 int zoneDest;
 int idLigne;
 int sens;
 int idBus;
 Horaire dateVente;
 int distance;
 int prix;
} Ticket ;

typedef struct {
 int id;
 int idZoneCourante;
 int row;
 int column;
 int enRoute;
 int idLigneCourante;
 int sensCourant;
 int nombreTaches;
 Tache taches[10];
 int nombrePassagers;
 Personne passagers[50];
 int nombreTickets;
 Ticket tickets[250];
} Bus ;

typedef struct {
 int rue;
 int zone;
 int bus;
} caseCarte ;

//chargement de fichier zone

void chargerZones(Zone zones[], int *nombreZones)
{
    FILE *fic =fopen("zones.txt", "r");
    int i=0;
    while (!feof(fic)) {
        fscanf(fic, "%d %d %d", &zones[i].id, &zones[i].row, &zones[i].column);
        i++;
    }
    *nombreZones=i;
    fclose(fic);
}

//afficher de tableau zone

void afficherZones(Zone zones[], int nombreZones)
{
    printf("\nle contenu du tableau zone :\n");
    for(int i=0; i<nombreZones; i++){
        printf("%d %d %d \n",zones[i].id, zones[i].row, zones[i].column);
    }
    printf("\n");
}

//sauvgarder dans le nouveau fichier zone

void sauvegarderZones(Zone zones[], int nombreZones, char* nomFichier)
{
    FILE *fic =fopen(nomFichier, "w");
    for(int i=0; i<nombreZones; i++){
        fprintf(fic, "%d %d %d", zones[i].id, zones[i].row, zones[i].column);
        if (i < nombreZones - 1) {
            fprintf(fic, "\n");
        }
    }
    fclose(fic);
}

//chargement de fichier ligne

void chargerLignes(Ligne lignes[], int *nombreLignes)
{
    FILE *fic =fopen("lignes.txt", "r");
    int i=-1, j ,nbligne=0;
    while (!feof(fic)) {
        if(nbligne==0){
            i++;
            fscanf(fic, "%d %d", &lignes[i].id, &lignes[i].nombreZones);
            nbligne=lignes[i].nombreZones;
            j=0;
        }
        else{
            fscanf(fic, "%d", &lignes[i].idZones[j]);
            j++;
            nbligne--;
        }
    }
    *nombreLignes=i+1;
    fclose(fic);
}

//afficher le tableau ligne

void afficherLignes(Ligne lignes[], int nombreLignes)
{
    printf("\nle contenu du tableau ligne :\n");
    for(int i=0; i<nombreLignes; i++){
        printf("%d %d\n", i, lignes[i].nombreZones);
        for (int j = 0; j < lignes[i].nombreZones; j++) {
            printf("%d \n", lignes[i].idZones[j]);
        }
        printf("\n");
    }
}

//sauvgarder dans le nouveau fichier ligne

void sauvegarderLignes(Ligne lignes[],int nombreLignes, char* nomFichier)
{
    FILE *fic =fopen(nomFichier, "w");
    for(int i=0; i<nombreLignes; i++){
        fprintf(fic, "%d %d", i, lignes[i].nombreZones);
        for (int j = 0; j < lignes[i].nombreZones; j++) {
            fprintf(fic, "\n%d", lignes[i].idZones[j]);
        }
    }
    fclose(fic);
}

//chargement de fichier troncon

void chargerTroncons(Troncon troncons[], int *nombreTroncons)
{
    FILE *fic =fopen("troncons.txt", "r");
    int i = -1, j ,nbsub=0;
    while (!feof(fic)) {
        if(nbsub==0){
            i++;
            fscanf(fic, "%d %d %d %d %d %d %d", &troncons[i].idZoneSrc, &troncons[i].idZoneDest, &troncons[i].distance,
            &troncons[i].dureeApproximative.jour, &troncons[i].dureeApproximative.heure, &troncons[i].dureeApproximative.minute, &troncons[i].nombresubdivisions);
            nbsub= troncons[i].nombresubdivisions;
            j=0;
        }
        else{
            fscanf(fic, "%d %d %d", &troncons[i].parcours[j].row, &troncons[i].parcours[j].column, &troncons[i].parcours[j].codeRue);
            j++;
            nbsub--;
        }
    }
    *nombreTroncons=i+1;
    fclose(fic);
}

//affichage de tableau troncon

void afficherTroncons(Troncon troncons[], int nombreTroncons)
{
    printf("\nle contenu du tableau troncons :\n");
    for(int i=0; i<nombreTroncons; i++){
        printf("%d %d %d %d %d %d %d\n", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,
        troncons[i].dureeApproximative.jour, troncons[i].dureeApproximative.heure, troncons[i].dureeApproximative.minute, troncons[i].nombresubdivisions);
        for (int j = 0; j < troncons[i].nombresubdivisions; j++) {
            printf("%d %d %d\n", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
}

//sauvegarde de nouveau fichier troncons

void sauvegarderTroncons(Troncon troncons[], int nombreTroncons, char* nomFichier)
{
    FILE *fic =fopen(nomFichier, "w");
    for(int i=0; i<nombreTroncons; i++){
        if (i > 0) {
            fprintf(fic, "\n");
        }
        fprintf(fic, "%d %d %d %d %d %d %d", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,
        troncons[i].dureeApproximative.jour, troncons[i].dureeApproximative.heure, troncons[i].dureeApproximative.minute, troncons[i].nombresubdivisions);
        for (int j = 0; j < troncons[i].nombresubdivisions; j++) {
            fprintf(fic ,"\n%d %d %d", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
    fclose(fic);
}

//chargement de fichier flottebus

void chargerFlotteBus(Bus flotteBus[], int *nombreBus)
{
    FILE *fic =fopen("flotteBus1.txt", "r");
    int i = -1, j ,nbtache=0;
    while (!feof(fic)) {
        if(nbtache==0){
            i++;
            fscanf(fic, "%d %d %d %d %d %d %d %d", &flotteBus[i].id, &flotteBus[i].idZoneCourante, &flotteBus[i].row, &flotteBus[i].column, &flotteBus[i].enRoute, &flotteBus[i].idLigneCourante, &flotteBus[i].sensCourant, &flotteBus[i].nombreTaches);
            nbtache= flotteBus[i].nombreTaches;
            j=0;
        }
        else{
            fscanf(fic, "%d %d %d %d %d %d", &flotteBus[i].taches[j].id, &flotteBus[i].taches[j].idLigne, &flotteBus[i].taches[j].sens, &flotteBus[i].taches[j].horaireDateDepart.jour, &flotteBus[i].taches[j].horaireDateDepart.heure, &flotteBus[i].taches[j].horaireDateDepart.minute);
            j++;
            nbtache--;
        }
    }
    *nombreBus=i;
    fclose(fic);
}

//affichage de tableau flottebus

void afficherFlotteBus(Bus flotteBus[], int nombreBus)
{
    printf("\nle contenu du tableau flottebus :\n");
    for(int i=0; i<nombreBus; i++){
        printf("%d %d %d %d %d %d %d %d\n", flotteBus[i].id, flotteBus[i].idZoneCourante, flotteBus[i].row, flotteBus[i].column, flotteBus[i].enRoute, flotteBus[i].idLigneCourante, flotteBus[i].sensCourant, flotteBus[i].nombreTaches);
        for (int j = 0; j < flotteBus[i].nombreTaches; j++) {
            printf("%d %d %d %d %d %d\n", flotteBus[i].taches[j].id, flotteBus[i].taches[j].idLigne, flotteBus[i].taches[j].sens, flotteBus[i].taches[j].horaireDateDepart.jour, flotteBus[i].taches[j].horaireDateDepart.heure, flotteBus[i].taches[j].horaireDateDepart.minute);
        }
    }
}

//sauvegarde de nouveau fichier flotteBus

void sauvegarderFlotteBus(Bus flotteBus[], int nombreBus, char* nomFichier){
    FILE *fic =fopen(nomFichier, "w");
    for(int i=0; i<nombreBus; i++){
        fprintf(fic ,"%d %d %d %d %d %d %d %d", flotteBus[i].id, flotteBus[i].idZoneCourante, flotteBus[i].row, flotteBus[i].column, flotteBus[i].enRoute, flotteBus[i].idLigneCourante, flotteBus[i].sensCourant, flotteBus[i].nombreTaches);
        for (int j = 0; j < flotteBus[i].nombreTaches; j++) {
            fprintf(fic ,"\n%d %d %d %d %d %d", flotteBus[i].taches[j].id, flotteBus[i].taches[j].idLigne, flotteBus[i].taches[j].sens, flotteBus[i].taches[j].horaireDateDepart.jour, flotteBus[i].taches[j].horaireDateDepart.heure, flotteBus[i].taches[j].horaireDateDepart.minute);
        }
        if (i < nombreBus - 1) {
            fprintf(fic, "\n");
        }
    }
    fclose(fic);
}

//Affecter personne a une zone

void affecterPersonneZone(Zone zones[], Personne pers)
{
    zones[pers.idZoneHabitat].personnes[zones[pers.idZoneHabitat].nombrePersonnes]=pers;
    zones[pers.idZoneHabitat].nombrePersonnes++;
}

//Le module recherche sens

int rechercherLigneSens(int idZoneSrc, int idZoneDest, Ligne lignes[], int nombreLignes) {
    for (int i = 0; i < nombreLignes; i++) {
        int src = -1, dest = -1;
        for (int j = 0; j < lignes[i].nombreZones; j++) {
            if (lignes[i].idZones[j] == idZoneSrc) {
                src = j;
            } else if (lignes[i].idZones[j] == idZoneDest) {
                dest = j;
            }
        }
        if (src != -1 && dest != -1) {
            if(src < dest){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
    return -1;
}

//generer des personnes

void genererPersonne(Zone zones[], int nombreZones, Ligne lignes[], int nombreLignes)
{
    srand(time(NULL));
    Personne pers;

    for(int i=0;i<300;i++){
        pers.cin=i+1;
        pers.idZoneHabitat=rand()%10;
        do{
           pers.idZoneTravail=rand()%10;
        }while(pers.idZoneHabitat==pers.idZoneTravail);
        pers.idZoneCourante=pers.idZoneHabitat;
        pers.intentionVoyage=0;
        pers.idLigneCourante=-1;
        pers.sensCourant=-1;
        pers.idZoneDest=pers.idZoneTravail;
        pers.nombreActivites=3;
        //ACTIVITE 1
        pers.activites[0].id=0;
        pers.activites[0].horaireDateSouhaite.jour=0;
        pers.activites[0].horaireDateSouhaite.heure=6+rand()%3;
        pers.activites[0].horaireDateSouhaite.minute=0;
        pers.activites[0].dureeHeureMinute.jour=0;
        pers.activites[0].dureeHeureMinute.heure=8;
        pers.activites[0].dureeHeureMinute.minute=0;
        pers.activites[0].typeActivite=1;
        pers.activites[0].idZoneSrc=pers.idZoneHabitat;
        pers.activites[0].idZoneDest=pers.idZoneTravail;
        pers.activites[0].idLigne=0;
        pers.activites[0].sens=rechercherLigneSens(pers.activites[0].idZoneSrc, pers.activites[0].idZoneDest, lignes, nombreLignes);
        //ACTIVITE 2
        pers.activites[1].id=1;
        pers.activites[1].horaireDateSouhaite.jour=0;
        pers.activites[1].horaireDateSouhaite.heure=6+rand()%3;
        pers.activites[1].horaireDateSouhaite.minute=0;
        pers.activites[1].dureeHeureMinute.jour=0;
        pers.activites[1].dureeHeureMinute.heure=8;
        pers.activites[1].dureeHeureMinute.minute=0;
        pers.activites[1].typeActivite=2;
        pers.activites[1].idZoneSrc=pers.idZoneTravail;
        do{
            pers.activites[1].idZoneDest=rand()%10;
        }while(pers.activites[1].idZoneDest==pers.idZoneTravail || pers.activites[1].idZoneDest==pers.idZoneHabitat);
        pers.activites[1].idLigne=0;
        pers.activites[1].sens=rechercherLigneSens(pers.activites[1].idZoneSrc, pers.activites[1].idZoneDest, lignes, nombreLignes);
        //ACTIVITE 3
        pers.activites[2].id=2;
        pers.activites[2].horaireDateSouhaite.jour=0;
        pers.activites[2].horaireDateSouhaite.heure=6+rand()%3;
        pers.activites[2].horaireDateSouhaite.minute=0;
        pers.activites[2].dureeHeureMinute.jour=0;
        pers.activites[2].dureeHeureMinute.heure=8;
        pers.activites[2].dureeHeureMinute.minute=0;
        pers.activites[2].typeActivite=0;
        pers.activites[2].idZoneSrc=pers.activites[1].idZoneDest;
        pers.activites[2].idZoneDest=pers.idZoneHabitat;
        pers.activites[2].idLigne=0;
        pers.activites[2].sens=rechercherLigneSens(pers.activites[2].idZoneSrc, pers.activites[2].idZoneDest, lignes, nombreLignes);
        affecterPersonneZone(zones, pers);
    }
}

//Affichage personnes

void afficherPersonnes(Zone zones[], int nombreZones)
{
    printf("\nle contenu du tableau zone PERSONNES:\n");
    for(int i=0; i<nombreZones; i++){
        printf("la zone %d \n",i);
        for(int j=0; j<zones[i].nombrePersonnes; j++){
            printf("    le personne %d \n",j);
            printf("    %d %d %d %d %d %d %d %d %d \n",
            zones[i].personnes[j].cin,
            zones[i].personnes[j].idZoneHabitat,
            zones[i].personnes[j].idZoneTravail,
            zones[i].personnes[j].idZoneCourante,
            zones[i].personnes[j].intentionVoyage,
            zones[i].personnes[j].idLigneCourante,
            zones[i].personnes[j].sensCourant,
            zones[i].personnes[j].idZoneDest,
            zones[i].personnes[j].nombreActivites);
            for(int c=0; c<3; c++){
                printf("        l'activite %d \n",c);
                printf("        %d %d %d %d %d %d %d %d %d %d %d %d \n",
                       zones[i].personnes[j].activites[c].id,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.jour,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.heure,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.minute,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.jour,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.heure,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.minute,
                       zones[i].personnes[j].activites[c].typeActivite,
                       zones[i].personnes[j].activites[c].idZoneSrc,
                       zones[i].personnes[j].activites[c].idZoneDest,
                       zones[i].personnes[j].activites[c].idLigne,
                       zones[i].personnes[j].activites[c].sens);
            }
        }
        printf("\n");
    }
}

//Sauvegarde Persones dans sauvPersonnes.txt

void sauvegarderPersonnes(Zone zones[], int nombreZones, char* nomFichier)
{
    FILE *fic =fopen(nomFichier, "w");
    for(int i=0; i<nombreZones; i++){
        for(int j=0; j<zones[i].nombrePersonnes; j++){
            if (!(i == 0 && j == 0)) {
                fprintf(fic, "\n");
            }
            fprintf(fic,"%d %d %d %d %d %d %d %d %d",
            zones[i].personnes[j].cin,
            zones[i].personnes[j].idZoneHabitat,
            zones[i].personnes[j].idZoneTravail,
            zones[i].personnes[j].idZoneCourante,
            zones[i].personnes[j].intentionVoyage,
            zones[i].personnes[j].idLigneCourante,
            zones[i].personnes[j].sensCourant,
            zones[i].personnes[j].idZoneDest,
            zones[i].personnes[j].nombreActivites);
            for(int c=0; c<3; c++){
                fprintf(fic,"\n%d %d %d %d %d %d %d %d %d %d %d %d",
                       zones[i].personnes[j].activites[c].id,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.jour,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.heure,
                       zones[i].personnes[j].activites[c].horaireDateSouhaite.minute,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.jour,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.heure,
                       zones[i].personnes[j].activites[c].dureeHeureMinute.minute,
                       zones[i].personnes[j].activites[c].typeActivite,
                       zones[i].personnes[j].activites[c].idZoneSrc,
                       zones[i].personnes[j].activites[c].idZoneDest,
                       zones[i].personnes[j].activites[c].idLigne,
                       zones[i].personnes[j].activites[c].sens);
            }
        }
    }
    fclose(fic);
}

//Charger Persones dans le tableau zone depuis personnesSauv.txt

void chargerPersonnes(Zone zones[], int* nombreZones, char* nomFichier)
{
    FILE *fic = fopen(nomFichier, "r");
    Personne pers;
    int nbact = 0;
    int c = 0;
    while (!feof(fic)) {
        if (nbact == 0) {
            fscanf(fic, "%d %d %d %d %d %d %d %d %d",
                    &pers.cin,
                    &pers.idZoneHabitat,
                    &pers.idZoneTravail,
                    &pers.idZoneCourante,
                    &pers.intentionVoyage,
                    &pers.idLigneCourante,
                    &pers.sensCourant,
                    &pers.idZoneDest,
                    &pers.nombreActivites);
            nbact = pers.nombreActivites;
            c = 0;
        }
        else {
            fscanf(fic,"%d %d %d %d %d %d %d %d %d %d %d %d",
                    &pers.activites[c].id,
                    &pers.activites[c].horaireDateSouhaite.jour,
                    &pers.activites[c].horaireDateSouhaite.heure,
                    &pers.activites[c].horaireDateSouhaite.minute,
                    &pers.activites[c].dureeHeureMinute.jour,
                    &pers.activites[c].dureeHeureMinute.heure,
                    &pers.activites[c].dureeHeureMinute.minute,
                    &pers.activites[c].typeActivite,
                    &pers.activites[c].idZoneSrc,
                    &pers.activites[c].idZoneDest,
                    &pers.activites[c].idLigne,
                    &pers.activites[c].sens);
            c++;
            nbact--;
            if (nbact == 0) {
                affecterPersonneZone(zones, pers);
            }
        }
    }
    fclose(fic);
}

//Remplissage Totale

void remplissageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int *nombreZones, int *nombreLignes, int *nombreTroncons, int *nombreBus)
{
    chargerZones(zones, nombreZones);
    chargerLignes(lignes, nombreLignes);
    chargerTroncons(troncons, nombreTroncons);
    chargerFlotteBus(flotteBus, nombreBus);
}

//Affichage Totale

void affichageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
    afficherZones(zones, nombreZones);
    afficherLignes(lignes, nombreLignes);
    afficherTroncons(troncons, nombreTroncons);
    afficherFlotteBus(flotteBus, nombreBus);
    afficherPersonnes(zones, nombreZones);
}

//Sauvegarde Totale

void sauvegardeComplete(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, char *nomFichier)
{
    sauvegarderZones(zones, nombreZones, "zones_sauv.txt");
    sauvegarderLignes(lignes, nombreLignes, "lignes_sauv.txt");
    sauvegarderTroncons(troncons,nombreTroncons, "tronconsSauv.txt");
    sauvegarderFlotteBus(flotteBus, nombreBus, "flotteBusSauv.txt");
    sauvegarderPersonnes(zones, nombreZones, "personnesSauv.txt");
}

//calcule distance

int calculerDistance(int idZoneSrc, int idZoneDest, Ligne lignes[], Troncon troncons[], int nombreLignes, int nombreTroncons) {
    if(rechercherLigneSens(idZoneSrc, idZoneDest, lignes, nombreLignes) == -1)
        return -1;
    int dist = 0;
    if(rechercherLigneSens(idZoneSrc, idZoneDest, lignes, nombreLignes) == 1) {
        int aux = idZoneSrc;
        idZoneSrc = idZoneDest;
        idZoneDest = aux;
    }
    for (int i = 0; i < nombreTroncons; i++) {
        if (troncons[i].idZoneSrc == idZoneSrc) {
            for (int j = i; j < nombreTroncons && troncons[j].idZoneSrc != idZoneDest; j++) {
                dist += troncons[j].distance;
            }
        }
    }
    return dist;
}

//initialisation carte

void initialiserCarte(caseCarte cartegeo[20][20], Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
    for( int l=0; l<20 ; l++){
        for( int c=0; c<20 ; c++){
            cartegeo[l][c].rue=0;
            cartegeo[l][c].zone=-1;
            cartegeo[l][c].bus=-1;
        }
    }
    for( int i=0; i<nombreZones ; i++){
        int r=zones[i].row;
        int p=zones[i].column;
        cartegeo[r][p].zone=zones[i].id;
    }
    for( int i=0; i<nombreTroncons ; i++){
        for( int j=0; j<troncons[i].nombresubdivisions ; j++){
            int r=troncons[i].parcours[j].row;
            int p=troncons[i].parcours[j].column;
            cartegeo[r][p].rue=troncons[i].parcours[j].codeRue;
        }
    }
    for( int i=0; i<nombreBus ; i++){
        int r=flotteBus[i].row;
        int p=flotteBus[i].column;
        if(cartegeo[r][p].bus==-1)
            cartegeo[r][p].bus=flotteBus[i].id;
        else
            cartegeo[r][p].bus=100;
    }
}

void afficherMessage(SDL_Surface *ecran, TTF_Font *font, SDL_Color couleur, SDL_Rect position, char* message) {
    // Rendre le texte
    SDL_Surface *texte = TTF_RenderText_Blended(font, message, couleur);

    // Calculer la position du texte
    SDL_Rect positionText;
    positionText.x = position.x + 10;
    positionText.y = position.y - 2;

    // Afficher le texte à l'écran
    SDL_BlitSurface(texte, NULL, ecran, &positionText);

    // Libérer la surface du texte
    SDL_FreeSurface(texte);
}

char* semaine(int j) {
    switch (j) {
        case 0: return "Lundi";
        case 1: return "Mardi";
        case 2: return "Mercredi";
        case 3: return "Jeudi";
        case 4: return "Vendredi";
        case 5: return "Samedi";
        case 6: return "Dimanche";
        default: return "Jour invalide";
    }
}

void afficherCarteGeo( SDL_Surface* ecran, caseCarte cartegeo[20][20] , Zone zones[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus,
                       TTF_Font *font, int selectionOption,int j, int h, int m, SDL_Surface *back, SDL_Rect positionBackground,
                                                                                SDL_Surface *home, SDL_Rect positionHome,
                                                                                SDL_Surface *quitter, SDL_Rect positionQuitter,
                                                                                SDL_Surface *stat, SDL_Rect positionStat)
{
    SDL_BlitSurface(back, NULL, ecran, &positionBackground);
    SDL_BlitSurface(home, NULL, ecran, &positionHome);
    SDL_BlitSurface(quitter, NULL, ecran, &positionQuitter);
    if(h>=13 || j>0){
        SDL_BlitSurface(stat, NULL, ecran, &positionStat);
    }

    SDL_Surface *zone = NULL, *zoneBus = NULL, *busMultiple = NULL, *bus = NULL, *busM = NULL, *troncon3 = NULL, *troncon5 = NULL, *troncon6 = NULL, *troncon9 = NULL, *troncon12 = NULL, *troncon15 = NULL;
    SDL_Rect position;
    SDL_Rect positionHeure;

    zone = IMG_Load("assets/zone.png");
    zoneBus = IMG_Load("assets/zonePleine.jpg");
    busMultiple = IMG_Load("assets/ZoneBus.png");
    bus = IMG_Load("assets/bus.png");
    busM = IMG_Load("assets/busMultiple.jpg");
    troncon12 = IMG_Load("assets/troncon12.png");
    troncon5 = IMG_Load("assets/troncon5.png");
    troncon6 = IMG_Load("assets/troncon6.png");
    troncon9 = IMG_Load("assets/troncon9.png");
    troncon3 = IMG_Load("assets/troncon3.png");
    troncon15 = IMG_Load("assets/troncon15.png");
    SDL_Surface* imagesCarte[5];
    imagesCarte[0] = IMG_Load("assets/11.png");
    imagesCarte[1] = IMG_Load("assets/12.png");
    imagesCarte[2] = IMG_Load("assets/13.png");
    imagesCarte[3] = IMG_Load("assets/14.png");
    imagesCarte[4] = IMG_Load("assets/15.png");

    //initialisation de texte
    SDL_Color couleur_a_utiliser = {0, 0, 0};
    font = TTF_OpenFont("Roboto-Regular.ttf", 10);
    TTF_Font *font1 = TTF_OpenFont("Roboto-Regular.ttf", 22);

    // Draw the grid or the blocks
    for (int l = 0; l < 20; l++) {
        for (int c = 0; c < 20; c++) {
            position.x = c * 35;
            position.y = l * 35;

            if (cartegeo[l][c].zone != -1 && cartegeo[l][c].bus == -1) {
                SDL_BlitSurface(zone, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].zone != -1 && cartegeo[l][c].bus != -1) {
                if (cartegeo[l][c].bus < 100) {
                    SDL_BlitSurface(zoneBus, NULL, ecran, &position);
                }
                else {
                    SDL_BlitSurface(busMultiple, NULL, ecran, &position);
                }
            }
            else if (cartegeo[l][c].bus != -1) {
                if (cartegeo[l][c].bus < 100) {
                    // Créer un tableau pour les textes à afficher
                    char message[10];
                    sprintf(message, "%dP", flotteBus[cartegeo[l][c].bus].nombrePassagers);
                    afficherMessage(ecran, font, couleur_a_utiliser,position, message);
                    SDL_BlitSurface(bus, NULL, ecran, &position);
                }
                else {
                    SDL_BlitSurface(busM, NULL, ecran, &position);
                }
            }
            else if (cartegeo[l][c].rue == 3) {
                SDL_BlitSurface(troncon3, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].rue == 12) {
                SDL_BlitSurface(troncon12, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].rue == 5) {
                SDL_BlitSurface(troncon5, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].rue == 6) {
                SDL_BlitSurface(troncon6, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].rue == 9) {
                SDL_BlitSurface(troncon9, NULL, ecran, &position);
            }
            else if (cartegeo[l][c].rue == 15) {
                SDL_BlitSurface(troncon15, NULL, ecran, &position);
            }
        }
    }

    // Options pour la vitesse de simulation
    SDL_Rect positionImage;
    for (int i = 0; i < 5; i++) {
        positionImage.x = 705;  // Position x fixe
        positionImage.y = 190 + i * 40; // Position y dynamique
        SDL_BlitSurface(imagesCarte[i], NULL, ecran, &positionImage);
    }

    // Afficher l'heure actuelle (h et m)
    SDL_Color noir = {0, 0, 0};
    positionHeure.x=230;
    positionHeure.y=0;
    char message[20];
    sprintf(message, "%s %dh:%dm",semaine(j), h, m);
    afficherMessage(ecran, font1, noir,positionHeure, message);

    SDL_Flip(ecran);

    SDL_FreeSurface(zone);
    SDL_FreeSurface(zoneBus);
    SDL_FreeSurface(busMultiple);
    SDL_FreeSurface(bus);
    SDL_FreeSurface(busM);
    SDL_FreeSurface(troncon3);
    SDL_FreeSurface(troncon5);
    SDL_FreeSurface(troncon6);
    SDL_FreeSurface(troncon9);
    SDL_FreeSurface(troncon12);
    SDL_FreeSurface(troncon15);
    // Libérer les surfaces après utilisation (à faire en fin de programme)
    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(imagesCarte[i]);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
}

//Détermination du numéro de la tâche

int numTacheSuivante(Horaire horaireCourant, int idBusCourant, Bus flotteBus[])
{
    for(int j=0 ; j<flotteBus[idBusCourant].nombreTaches ; j++){
        if(flotteBus[idBusCourant].taches[j].horaireDateDepart.jour==horaireCourant.jour
           && flotteBus[idBusCourant].taches[j].horaireDateDepart.heure==horaireCourant.heure
           && flotteBus[idBusCourant].taches[j].horaireDateDepart.minute==horaireCourant.minute){
            return(flotteBus[idBusCourant].taches[j].id);
        }
    }
    return -1;
}

//Déplacement Bus
void deplacerBus(caseCarte cartegeo[20][20], int idBusCourant, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, Horaire horaireCourant)
{
    int i=0,dest;
    while (flotteBus[idBusCourant].idZoneCourante != lignes[0].idZones[i])
    {
        i++;
    }
    if (flotteBus[idBusCourant].sensCourant == 0)
        dest = i + 1;
    else
        dest = i - 1;

    int tron = 0,sub=0;
    if (flotteBus[idBusCourant].sensCourant == 0)
    {
        while ((tron < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[tron].idZoneSrc) || (lignes[0].idZones[dest] != troncons[tron].idZoneDest)))
            tron++;
    }
    else
    {
        while ((tron < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[tron].idZoneDest) || (lignes[0].idZones[dest] != troncons[tron].idZoneSrc)))
            tron++;
    }

    while ((sub < troncons[tron].nombresubdivisions) && ((troncons[tron].parcours[sub].row != flotteBus[idBusCourant].row) || (troncons[tron].parcours[sub].column != flotteBus[idBusCourant].column)))
    {
        sub++;
    }

    //sens 0
    if(flotteBus[idBusCourant].sensCourant==0){
        if(sub==troncons[tron].nombresubdivisions){
            //l'emplacement de bus est une zone
            int z=0;
            while(z<nombreZones && flotteBus[idBusCourant].idZoneCourante!=zones[z].id)
                z++;
            if(flotteBus[idBusCourant].row==zones[z].row && flotteBus[idBusCourant].column==zones[z].column){
                int r,c;
                r=troncons[tron].parcours[0].row;
                c=troncons[tron].parcours[0].column;
                cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                //Z%dBM***
                if(cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus<100)
                    cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                flotteBus[idBusCourant].row = r;
                flotteBus[idBusCourant].column = c;
            }
        }
        else {
            // kitabda el blassa elli machilha mahich zone w l blassa elli a7na faha taw mahich zone zada
            if(sub<troncons[tron].nombresubdivisions-1){
                int r,c;
                r=troncons[tron].parcours[sub+1].row;
                c=troncons[tron].parcours[sub+1].column;
                cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                flotteBus[idBusCourant].row = r;
                flotteBus[idBusCourant].column = c;
            }
            else{
                //kitabda de5el fi zone
                int s=0;
                while(s<nombreZones && (lignes[0].idZones[dest]!=zones[s].id)){
                    s++;
                }
                int r,c;
                r=zones[s].row;
                c=zones[s].column;
                if(cartegeo[r][c].bus==-1){
                    cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                }
                else if(cartegeo[r][c].bus==100){
                    cartegeo[r][c].bus=100;
                }
                else{
                    cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                }
                cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                flotteBus[idBusCourant].row = r;
                flotteBus[idBusCourant].column = c;
                flotteBus[idBusCourant].idZoneCourante=zones[s].id;
                if(tron==nombreTroncons-1 && sub==troncons[nombreTroncons-1].nombresubdivisions-1){
                    flotteBus[idBusCourant].enRoute=0;
                }
            }
        }
    }
    //sens 1
    else{
        if(sub==troncons[tron].nombresubdivisions){
            //l'emplacement de bus est une zone
            int z=0;
            while(z<nombreZones && flotteBus[idBusCourant].idZoneCourante!=zones[z].id)
                z++;
            if(flotteBus[idBusCourant].row==zones[z].row && flotteBus[idBusCourant].column==zones[z].column){
                int r,c;
                r=troncons[tron].parcours[troncons[tron].nombresubdivisions-1].row;
                c=troncons[tron].parcours[troncons[tron].nombresubdivisions-1].column;
                cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                //Z%dBM***
                if(cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus==100){
                    cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=100;
                }
                else{
                    cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                }
                flotteBus[idBusCourant].row = r;
                flotteBus[idBusCourant].column = c;
            }
        }
        else {
            if(sub>0){
                // kitabda el blassa elli machilha mahich zone w l blassa elli a7na faha taw mahich zone zada
                int r,c;
                r=troncons[tron].parcours[sub-1].row;
                c=troncons[tron].parcours[sub-1].column;
                //[BM]****
                if(cartegeo[r][c].bus==-1){
                    cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                    if(cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus==idBusCourant){
                        cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                    }
                    flotteBus[idBusCourant].row = r;
                    flotteBus[idBusCourant].column = c;
                }
                else{
                    cartegeo[r][c].bus=100;
                    if(cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus==idBusCourant){
                        cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                    }
                    flotteBus[idBusCourant].row = r;
                    flotteBus[idBusCourant].column = c;
                }
            }
            else{
                //kitabda de5el fi zone
                int s=0;
                while(s<nombreZones && (lignes[0].idZones[dest]!=zones[s].id)){
                    s++;
                }
                int r,c;
                r=zones[s].row;
                c=zones[s].column;
                if(cartegeo[r][c].bus!=-1){
                    cartegeo[r][c].bus=100;
                }
                else{
                    cartegeo[r][c].bus=flotteBus[idBusCourant].id;
                }
                cartegeo[flotteBus[idBusCourant].row][flotteBus[idBusCourant].column].bus=-1;
                flotteBus[idBusCourant].row = r;
                flotteBus[idBusCourant].column = c;
                flotteBus[idBusCourant].idZoneCourante=zones[s].id;
                if(tron==0 && sub==0){
                    flotteBus[idBusCourant].enRoute=0;
                }
            }
        }
    }
}

//LE DEPLACEMENT D'UNE PERSONNE D'UN TABLEAU A UN AUTRE
void deplacerPersonne(Personne src[], Personne dest[], int indicePersonneSrc, int *nombrePersonnesSrc, int *nombrePersonnesDest)
{
    // Ajoutez la personne au tableau de destination
    dest[*nombrePersonnesDest] = src[indicePersonneSrc];
    (*nombrePersonnesDest)++;

    // Supprimez la personne du tableau source en décalant les éléments restants
    for (int i = indicePersonneSrc; i < *nombrePersonnesSrc - 1; i++) {
        src[i] = src[i + 1];
    }

    // Réduisez le nombre de personnes dans le tableau source
    (*nombrePersonnesSrc)--;
}

//MISE A JOUR PERSONNE UNE ZONE

void majPersonnesUneZone(Horaire horaireCourant, Zone zones[], int idZone)
{
    for(int j=0 ; j<zones[idZone].nombrePersonnes ; j++){
        for(int i=0; i<zones[idZone].personnes[j].nombreActivites; i++){
            if(zones[idZone].personnes[j].activites[i].horaireDateSouhaite.jour==horaireCourant.jour
               && zones[idZone].personnes[j].activites[i].horaireDateSouhaite.heure==horaireCourant.heure
               && zones[idZone].personnes[j].activites[i].horaireDateSouhaite.minute==horaireCourant.minute){
                zones[idZone].personnes[j].intentionVoyage=1;
                zones[idZone].personnes[j].idZoneCourante=zones[idZone].personnes[j].activites[i].idZoneSrc;
                zones[idZone].personnes[j].idZoneDest=zones[idZone].personnes[j].activites[i].idZoneDest;
                zones[idZone].personnes[j].idLigneCourante=zones[idZone].personnes[j].activites[i].idLigne;
                zones[idZone].personnes[j].sensCourant=zones[idZone].personnes[j].activites[i].sens;
            }
        }
    }
}

//MISE A JOUR PERSONNE TOUTES ZONES

void majPersonnesToutesZones(Horaire horaireCourant, Zone zones[], int nombreZones)
{
    for(int i=0; i<nombreZones; i++){
        majPersonnesUneZone(horaireCourant, zones, i);
    }
}

/****** TICKET ******/

//calcule prix ticket
int calculerPrixTicket(int x)
{
    if(x>=1 && x<=30)
        return 300;
    else if(x>30 && x<=60)
        return 600;
    else
        return 900;
}

//vente ticket
Ticket venteTicket(Horaire horaireVente, int idZoneSrc, int idZoneDest, int idLigne, int sens, Ligne lignes[], Troncon troncons[], int nombreLignes, int nombreTroncons, int idBus)
{
    Ticket tick;
    tick.zoneSrc=idZoneSrc;
    tick.zoneDest=idZoneDest;
    tick.idLigne=idLigne;
    tick.sens=sens;
    tick.idBus=idBus;
    tick.dateVente=horaireVente;
    tick.distance=calculerDistance(idZoneSrc, idZoneDest, lignes, troncons, nombreLignes, nombreTroncons);
    tick.prix=calculerPrixTicket(tick.distance);
    return tick;
}

//deplacement et reinitialisation ticket

void deplacerEtReinitialiserTicketsBus(Bus flotteBus[], int nombreBus, Ticket tickets[]) {
    int nombreTotalTicket=0;
    for (int i = 0; i < nombreBus; i++) {
        for (int j = 0; j < flotteBus[i].nombreTickets; j++) {
            if (nombreTotalTicket < 10000) {
                tickets[nombreTotalTicket] = flotteBus[i].tickets[j];
                nombreTotalTicket++;
            }
        }
        // Réinitialiser les tickets dans les bus
        flotteBus[i].nombreTickets = 0;
    }
}

/******************************/

//EMBARQUER PERSONNE

void embarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus, Horaire horaireCourant, Ligne lignes[] , Troncon troncons[] , int nombreLignes, int nombreTroncons)
{
    int nbEmbarquant=0;
    for(int i=0; i<zones[idZone].nombrePersonnes; i++){
        if(zones[idZone].personnes[i].intentionVoyage==1
           && zones[idZone].personnes[i].idLigneCourante==flotteBus[idBus].idLigneCourante
           && zones[idZone].personnes[i].sensCourant==flotteBus[idBus].sensCourant){
                deplacerPersonne(zones[idZone].personnes, flotteBus[idBus].passagers, i, &zones[idZone].nombrePersonnes, &flotteBus[idBus].nombrePassagers);
                flotteBus[idBus].tickets[flotteBus[idBus].nombreTickets] = venteTicket(horaireCourant, zones[idZone].personnes[i].idZoneCourante, zones[idZone].personnes[i].idZoneDest, zones[idZone].personnes[i].idLigneCourante, zones[idZone].personnes[i].sensCourant, lignes , troncons , nombreLignes, nombreTroncons, i);
                (flotteBus[idBus].nombreTickets)++;
                    nbEmbarquant++;
           }
    }
}

//DEBARQUER PERSONNE

void debarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus, Horaire horaireCourant)
{
    int nbDebarquant=0;
    for(int i=0; i<flotteBus[idBus].nombrePassagers; i++){
        if(flotteBus[idBus].passagers[i].idZoneDest==idZone){
            deplacerPersonne(flotteBus[idBus].passagers, zones[idZone].personnes, i, &flotteBus[idBus].nombrePassagers, &zones[idZone].nombrePersonnes);
            nbDebarquant++;
        }
    }
    majPersonnesUneZone(horaireCourant, zones, idZone);
}

//Calcule de gain

int calculerGainNet(Bus flotteBus[], int nombreBus, Ligne lignes[], int nombreLignes)
{
    int gain=0;
    for(int i=0; i<nombreBus; i++){
        for(int j=0; j<flotteBus[i].nombreTickets; j++)
            gain+=flotteBus[i].tickets[j].prix;
    }
    return gain;
}

//Sauvegarde rapport

void sauvegarderRapport(char* nomFichier, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
    FILE *fic =fopen("rapportSauv.txt", "w");
    for(int i=0; i<nombreBus; i++){
        int gain=0;
        for(int j=0; j<flotteBus[i].nombreTickets; j++)
            gain+=flotteBus[i].tickets[j].prix;
        fprintf(fic,"%d ",i);
        fprintf(fic,"%d\n",gain);
    }
    fprintf(fic,"%d",calculerGainNet(flotteBus, nombreBus, lignes, nombreLignes));
    fclose(fic);
}


//Dérouler

void deroulerMinute(int m, int h, int j, SDL_Surface *ecran, caseCarte cartegeo[20][20], Ticket tickets[], Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus,char* nomFichier)
{
    Horaire now;

    now.jour=j;
    now.heure=h;
    now.minute=m;
    majPersonnesToutesZones(now, zones, nombreZones);

    for(int i=0; i<nombreBus; i++){
        if(flotteBus[i].enRoute==0){
            int k=numTacheSuivante( now, i, flotteBus);
            if(k > -1){
                flotteBus[i].enRoute=1;
                flotteBus[i].sensCourant=flotteBus[i].taches[k].sens;
                flotteBus[i].idLigneCourante=flotteBus[i].taches[k].idLigne;
            }
        }
        if((flotteBus[i].row==zones[flotteBus[i].idZoneCourante].row) && (flotteBus[i].column==zones[flotteBus[i].idZoneCourante].column) && flotteBus[i].enRoute==1){
            embarquerPersonnes( zones, flotteBus, zones[flotteBus[i].idZoneCourante].id, i, now, lignes, troncons, nombreLignes, nombreTroncons);
            debarquerPersonnes( zones, flotteBus, zones[flotteBus[i].idZoneCourante].id, i, now);
        }
        if(flotteBus[i].enRoute==1){
            deplacerBus(cartegeo, i, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons , nombreBus, now);
            sauvegardeComplete(zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, nomFichier);
        }
    }
    /***************************************/
    if(h == 12 && m == 39){
        sauvegarderRapport(nomFichier, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus);
        deplacerEtReinitialiserTicketsBus(flotteBus, nombreBus, tickets);
    return;
    }
    /***************************************/
}

int estDansZone(int x, int y, int x_min, int x_max, int y_min, int y_max) {
    return(x >= x_min && x <= x_max && y >= y_min && y <= y_max);
}

// Fonction pour lire les statistiques depuis un fichier
int lireStatistiques(const char *nomFichier, int stats[], int tailleMax, int *profitTotal) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        return -1; // Erreur d'ouverture
    }

    int index, valeur;
    int i = 0;

    // Lire les 10 premières lignes avec index et valeur
    while (i < tailleMax && fscanf(fichier, "%d %d", &index, &valeur) == 2) {
        stats[index] = valeur; // Stocke la valeur pour l'indice donné (id du bus)
        i++;
    }

    // Lire la dernière ligne pour le profit total (sans index)
    if (fscanf(fichier, "%d", profitTotal) != 1) {
        printf("Erreur : impossible de lire le profit total.\n");
        fclose(fichier);
        return -1;
    }

    fclose(fichier);
    return i; // Retourne le nombre d'éléments lus (pour les bus)
}

void afficherStatistiques(SDL_Surface *ecran, TTF_Font *font, int stats[], int taille, int profitTotal, SDL_Color couleur) {
    SDL_Surface *texte = NULL;
    SDL_Rect positionText;

    // Charger une icône de bus
    SDL_Surface *busIcon = IMG_Load("assets/b.png");
    SDL_Surface *MoneyIcon = IMG_Load("assets/m.png");
    SDL_Surface *money = IMG_Load("assets/money.png");

    // Position de départ pour afficher les statistiques
    positionText.x = (ecran->w / 2) - 130;
    positionText.y = 130;

    char buffer[50];
    for (int i = 0; i < taille; i++) {
        sprintf(buffer, "Le gain net de Bus %d : %ddt", i, stats[i]);
        // Affichage de l’icône du bus
        SDL_Rect positionIcone = {positionText.x - 35, positionText.y-10, 20, 20};
        SDL_BlitSurface(busIcon, NULL, ecran, &positionIcone);
        SDL_Rect positionIconeMoney = {positionText.x + 290, positionText.y-10, 20, 20};
        SDL_BlitSurface(money, NULL, ecran, &positionIconeMoney);

        texte = TTF_RenderText_Blended(font, buffer, couleur);
        SDL_BlitSurface(texte, NULL, ecran, &positionText);
        SDL_FreeSurface(texte);
        positionText.y += 30; // Décaler verticalement pour la ligne suivante
    }

    // Afficher le profit total
    SDL_Rect ligne;
    ligne.x = 230;
    ligne.y = positionText.y;
    ligne.w = 400;
    ligne.h = 2;  // Épaisseur de la ligne

    SDL_FillRect(ecran, &ligne, SDL_MapRGB(ecran->format, 0, 0, 0)); // Noir
    positionText.y += 10;  // Décaler après la ligne

    SDL_Rect positionmoneyIcone = {positionText.x - 35, positionText.y-5, 20, 20};
    SDL_Rect positionMoneyIcone = {positionText.x + 280, positionText.y-5, 20, 20};
    sprintf(buffer, "Profit total du jour : %ddt", profitTotal);
    texte = TTF_RenderText_Blended(font, buffer, couleur);
    SDL_BlitSurface(texte, NULL, ecran, &positionText);
    SDL_BlitSurface(MoneyIcon, NULL, ecran, &positionmoneyIcone);
    SDL_BlitSurface(money, NULL, ecran, &positionMoneyIcone);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(busIcon);
    SDL_FreeSurface(MoneyIcon);
    SDL_FreeSurface(money);
}


void animerMenu(SDL_Surface *ecran, SDL_Surface *menu, int positionFinaleX, int vitesse) {
    SDL_Rect positionMenu;
    positionMenu.x = ecran->w; // Commence en dehors de l'écran (à droite)
    positionMenu.y = 0;

    // Boucle d'animation
    while (positionMenu.x > positionFinaleX) {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Nettoyer l'écran

        positionMenu.x -= vitesse; // Déplacer le menu vers la gauche
        if (positionMenu.x < positionFinaleX) {
            positionMenu.x = positionFinaleX; // S'assurer que ça s'arrête exactement
        }

        SDL_BlitSurface(menu, NULL, ecran, &positionMenu); // Afficher le menu à la nouvelle position
        SDL_Flip(ecran); // Mettre à jour l'écran
        SDL_Delay(10); // Pause pour rendre l'animation fluide (ajustez selon les besoins)
    }
}

void dessinerBouton(SDL_Surface *ecran, TTF_Font *font, SDL_Rect rect, const char *texte,
                    SDL_Color couleurFond, SDL_Color couleurTexte, int survole) {
    // Modifier la couleur de fond si le bouton est survolé
    if (survole) {
        couleurFond.r = 150; couleurFond.g = 150; couleurFond.b = 150;
    }

    // Dessiner le fond du bouton
    SDL_FillRect(ecran, &rect, SDL_MapRGB(ecran->format, couleurFond.r, couleurFond.g, couleurFond.b));

    // Afficher le texte du bouton
    SDL_Surface *surfaceTexte = TTF_RenderText_Blended(font, texte, couleurTexte);
    SDL_Rect positionTexte = {
        rect.x + (rect.w - surfaceTexte->w) / 2,
        rect.y + (rect.h - surfaceTexte->h) / 2
    };
    SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);
    SDL_FreeSurface(surfaceTexte);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    // Déclarations des variables
    caseCarte cartegeo[20][20];
    Zone zones[10];
    Bus flotteBus[10];
    Ligne lignes[1];
    Troncon troncons[9];
    Ticket tickets[10000];
    int nombreZones, nombreLignes, nombreTroncons, nombreBus;
    char nomFichier[30];

    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;

    int continuer = 1;
    int idMenu = 0, indiceOption = 0, indiceOptionCarte = 0;
    int m = 0, h = 6, j = 0;

    // Initialisation SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Configuration de la fenêtre
    SDL_WM_SetIcon(IMG_Load("assets/school-bus.png"), NULL);
    ecran = SDL_SetVideoMode(850, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bus Network Simulator", NULL);

    // Charger le menu
    menu = IMG_Load("assets/busSimulator.png");

    animerMenu(ecran, menu, 0, 15); // Fait glisser le menu jusqu'à la position x=0 avec une vitesse de 15 pixels par frame
    positionMenu.x = 0;
    positionMenu.y = 0;

    // Charger la police d'écriture
    font = TTF_OpenFont("Roboto-Regular.ttf", 22);
    remplissageComplet(zones, lignes, troncons, flotteBus, &nombreZones, &nombreLignes, &nombreTroncons, &nombreBus);
    initialiserCarte(cartegeo, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus);

    int refreshRequired = 1; // Initialiser à 1 pour afficher l'écran au début
    int stats[10] = {0}; // Tableau pour les profits individuels des 10 bus
    int profitTotal = 0; // Variable pour le profit total
    int nbStats = lireStatistiques("rapportSauv.txt", stats, 10, &profitTotal);

    SDL_Rect boutonNouveauJeu = {680, 190, 130, 50}; // Position et taille du bouton "Nouveau jeu"
    SDL_Rect boutonChargerJeu = {680, 270, 130, 50}; // Position et taille du bouton "Charger jeu"
    SDL_Rect boutonQuitter = {680, 370, 130, 50}; // Position et taille du bouton "Quitter"
    int survoleNouveauJeu = 0; // Indicateur pour survol du bouton "Nouveau jeu"
    int survoleChargerJeu = 0; // Indicateur pour survol du bouton "Charger jeu"
    int survoleQuitter = 0; // Indicateur pour survol du bouton "Quitter"

    /******/
    SDL_Surface *back = IMG_Load("assets/back11.png");
    SDL_Rect positionBackground = {0, 0};
    /*******/
    SDL_Surface *retour = IMG_Load("assets/retour.png");
    SDL_Rect positionRetour = {0, 0};
    /*******/
    SDL_Surface *backStat = IMG_Load("assets/backStat.png");
    SDL_Rect positionbackStat = {0, 0};
    /******/
    SDL_Surface *titre = IMG_Load("assets/titre.png");
    SDL_Rect positiontitre = {310, 40};
    /*******/
    SDL_Surface *car = IMG_Load("assets/busM.png");
    SDL_Rect positioncar = {727, 217};
    SDL_Rect positioncar2 = {727, 299};
    /*******/
    SDL_Surface *close = IMG_Load("assets/close.png");
    SDL_Rect positionClose = {727, 400};
    /*******/
    SDL_Surface *home = IMG_Load("assets/menu.png");
    SDL_Rect positionHome = {705, 40};
    /*******/
    SDL_Surface *quitter = IMG_Load("assets/quitter.png");
    SDL_Rect positionQuitter = {705, 92};
    /*******/
    SDL_Surface *stat = IMG_Load("assets/stat.png");
    SDL_Rect positionStat = {705, 630};
    /*******/

    // Boucle principale
    while (continuer) {
        // Gestion des événements
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    continuer = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                survoleNouveauJeu = estDansZone(event.motion.x, event.motion.y,
                    boutonNouveauJeu.x, boutonNouveauJeu.x + boutonNouveauJeu.w,
                    boutonNouveauJeu.y, boutonNouveauJeu.y + boutonNouveauJeu.h
                );
                survoleChargerJeu = estDansZone(
                    event.motion.x, event.motion.y,
                    boutonChargerJeu.x, boutonChargerJeu.x + boutonChargerJeu.w,
                    boutonChargerJeu.y, boutonChargerJeu.y + boutonChargerJeu.h
                );
                survoleQuitter = estDansZone(
                    event.motion.x, event.motion.y,
                    boutonQuitter.x, boutonQuitter.x + boutonQuitter.w,
                    boutonQuitter.y, boutonQuitter.y + boutonQuitter.h
                );
                refreshRequired = 1;
                break;
            case SDL_MOUSEBUTTONUP: // Détection d'un clic gauche
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;

                    if (idMenu == 0) { // Menu principal
                        if (survoleNouveauJeu) {
                            indiceOption = 0; // "Nouveau jeu"
                            refreshRequired = 1;
                        } else if (survoleChargerJeu) {
                            indiceOption = 1; // "Charger jeu"
                            refreshRequired = 1;
                        } else if (survoleQuitter) {
                            indiceOption = 2; // "Quitter"
                            refreshRequired = 1;
                        } else {
                            break;
                        }

                        // Actions du menu principal
                        if (indiceOption == 0) { // "Nouveau jeu"
                            // Initialisation du jeu
                            genererPersonne(zones, nombreZones, lignes, nombreLignes);
                            idMenu = 1; // Passer au menu carte
                            refreshRequired = 1;
                        } else if (indiceOption == 1) {
                            // Charger un jeu sauvegardé
                            chargerPersonnes(zones, &nombreZones, "personnesSauv.txt");
                            idMenu = 1; // Passer au menu carte
                            refreshRequired = 1;
                        } else if(indiceOption == 2) {
                            continuer = 0;
                        }
                    } else if (idMenu == 1) { // Menu des options de la carte
                        int v=0;
                        if (estDansZone(x, y, 705, 845, 40, 91)) {
                            idMenu = 0; // Retourner au menu principale
                            sauvegardeComplete(zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, nomFichier);
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 92, 143)) {
                            continuer = 0; // quitter le jeu sans sauvegarder
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 190, 230)) {
                            indiceOptionCarte = 0; // "Par minute"
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 230, 270)) {
                            indiceOptionCarte = 1; // "Par 10 minutes"
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 270, 310)) {
                            indiceOptionCarte = 2; // "Par heure"
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 310, 350)) {
                            indiceOptionCarte = 3; // "Par 4 heures"
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 350, 390)) {
                            indiceOptionCarte = 4; // "Par jour"
                            refreshRequired = 1;
                        } else if (estDansZone(x, y, 705, 845, 630, 681)) {
                            idMenu = 2; //aller au statistique
                            refreshRequired = 1;
                        } else {
                            break;
                        }

                        // Dérouler la simulation après un clic
                        int valeursV[] = {1, 10, 60, 240};

                        if (indiceOptionCarte >= 0 && indiceOptionCarte <= 3) {
                            int limite = valeursV[indiceOptionCarte];

                            for (v = 0; v < limite; v++) {
                                if (h == 17 && m == 59) {
                                    idMenu = 2;
                                    refreshRequired = 1;
                                }
                                if (m == 59) {
                                    m = -1;
                                    h++;
                                }
                                if (h == 18) {
                                    h = 6;
                                    j++;
                                }
                                deroulerMinute(m, h, j, ecran, cartegeo, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, nomFichier);
                                m++;
                                afficherCarteGeo(ecran, cartegeo, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, font, indiceOptionCarte, j, h, m, back, positionBackground, home, positionHome, quitter, positionQuitter, stat, positionStat);
                                //SDL_Delay(200);
                            }
                        } else if(indiceOptionCarte == 4){
                            int tempsActuel = (h-6)*60 + m;
                            int tempsFinal = 720 - tempsActuel;
                            for (v = 0; v < tempsFinal; v++) {
                                /*if (h == 17 && m == 59) {
                                    idMenu = 2;
                                    refreshRequired = 1;
                                }*/
                                if (m == 59) {
                                    m = -1;
                                    h++;
                                }
                                if (h == 18) {
                                    h = 6;
                                    j++;
                                }
                                deroulerMinute(m, h, j, ecran, cartegeo, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, nomFichier);
                                m++;
                                afficherCarteGeo(ecran, cartegeo, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, font, indiceOptionCarte, j, h, m, back, positionBackground, home, positionHome, quitter, positionQuitter, stat, positionStat);
                                //SDL_Delay(200);
                            }
                        }
                    } else if(idMenu == 2){
                        if (estDansZone(x, y, 0, 50, 0, 50)) {
                            idMenu = 1; // Retourner au menu de la carte
                            refreshRequired = 1;
                        }
                    }
                }
                break;
        }

        if(refreshRequired){
            // Afficher le menu ou les options
            if (idMenu == 0) {
                SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
                dessinerBouton(ecran, font, boutonNouveauJeu, "Nouveau jeu", (SDL_Color){200, 200, 200}, (SDL_Color){0, 0, 0}, survoleNouveauJeu);
                SDL_BlitSurface(car, NULL, ecran, &positioncar);
                dessinerBouton(ecran, font, boutonChargerJeu, "Charger jeu", (SDL_Color){200, 200, 200}, (SDL_Color){0, 0, 0}, survoleChargerJeu);
                SDL_BlitSurface(car, NULL, ecran, &positioncar2);
                dessinerBouton(ecran, font, boutonQuitter, "Quitter", (SDL_Color){200, 200, 200}, (SDL_Color){0, 0, 0}, survoleQuitter);
                SDL_BlitSurface(close, NULL, ecran, &positionClose);
            } else if (idMenu == 1) {
                afficherCarteGeo(ecran, cartegeo, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, font, indiceOptionCarte, j, h, m, back, positionBackground, home, positionHome, quitter, positionQuitter, stat, positionStat);
            }
            else if(idMenu == 2){
                // Nettoyer l'écran
                SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

                SDL_BlitSurface(backStat, NULL, ecran, &positionbackStat);
                SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
                // Afficher les statistiques
                SDL_Rect position;
                position.x = 310;
                position.y=60;
                SDL_Color noir = {0, 0, 0};
                char message[40] = {"Les statistiques du jour"};
                afficherMessage(ecran, font, noir,position, message);
                afficherStatistiques(ecran, font, stats, nbStats, profitTotal, noir);
                SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
                // Mettre à jour l'écran
                SDL_Flip(ecran);
            }

            SDL_Flip(ecran); // Mettre à jour l'écran
            refreshRequired = 0;
        }
    }

    // Libération des ressources
    SDL_FreeSurface(back);
    SDL_FreeSurface(backStat);
    SDL_FreeSurface(retour);
    SDL_FreeSurface(titre);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
