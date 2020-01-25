
/* Ville Martas - 0543803 */
/* Käytetty C-ohjelmointiopasta,
   Kurssin aineistoa
   https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
   https://en.wikipedia.org/wiki/AVL_tree
   http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-delete.html
   https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Tietue solmulle */
typedef struct Solmu
{ 
    int data, korkeus;
    struct Solmu *vasen; 
    struct Solmu *oikea;
} Solmu, *Osoitin;


int* lueTiedosto(char *, int*);
void lisaaSolmu(Osoitin *, int, int*);
void vasenKierto(Osoitin *, int*);
void oikeaKierto(Osoitin *, int*);
void tulostaPuu(Osoitin, int);
void etsiAlkio(Osoitin, int);
void vapautaMuisti(Osoitin *);


/* Luetaan tiedostosta rivi riviltä ja tallennetaan alkiot listaan. */
int* lueTiedosto(char tiedNimi[256], int lista[10000])
{
    char muisti[12];
    int indeksi = 0;
    FILE *tiedosto;
    tiedosto = fopen(tiedNimi, "r");
    /* Tarkistetaan että tiedosto saadaan luettua */
    if (tiedosto == NULL)
    {
        printf("Tiedostoa ei onnistuttu lukemaan.");
        exit(1);
    }
    printf("Tiedostossa seuraavat avaimet:\n");

    /* Lisätään tiedostosta löytyvät alkiot listaan */
    while(fgets(muisti, 10, tiedosto) != NULL)
    {
        printf("%s\n", muisti);
        lista[indeksi] = atoi(muisti);
        indeksi++;
    }
    fclose(tiedosto);
    return lista;
}


/* Lisätään solmu, ja kutsutaan kiertoja tarvittaessa */
void lisaaSolmu(Osoitin *emo, int alkio, int* tasapaino)
{

    /* Mikäli emoa ei vielä ole */
    if(*emo == NULL)
    {
        *tasapaino = 1;
        if((*emo = (Osoitin)malloc(sizeof(Solmu))) == NULL)
        {
            perror("malloc");
            exit(1);
        }
        (*emo)->vasen = (*emo)->oikea = NULL;
        (*emo)->korkeus = 0;
        (*emo)->data = alkio;
    }
    /* Alkio on pienempi kuin emon avain, alkiosta emon vasen lapsi */
    else if (alkio < (*emo)->data)
    {
        lisaaSolmu(&(*emo)->vasen, alkio, tasapaino);
        if (*tasapaino != 0)
        {
            if ((*emo)->korkeus == -1)
            {
                (*emo)->korkeus = 0;
                *tasapaino = 0;
            }
            else if ((*emo)->korkeus == 0)
            {
                (*emo)->korkeus = 1;
            }
            else if((*emo)->korkeus == 1)
            {
                vasenKierto(emo, tasapaino);
            }
        }
    }
    /* Alkio on suurempi kuin emon avain, alkiosta emon oikea lapsi */
    else if (alkio > (*emo)->data)
    {
        lisaaSolmu(&(*emo)->oikea, alkio, tasapaino);
        if (*tasapaino != 0)
        {
            if ((*emo)->korkeus == 1)
            {
                (*emo)->korkeus = 0;
                *tasapaino = 0;
            }
            else if ((*emo)->korkeus == 0)
            {
                (*emo)->korkeus = -1;
            }
            else if((*emo)->korkeus == -1)
            {
                oikeaKierto(emo, tasapaino);
            }
        }
    } 
    else 
    {
        *tasapaino = 0;
        printf("Luku  %d löytyy jo puusta!", alkio);
    }
}


/* Vasemman kierron LL ja LR kierrot */
void vasenKierto(Osoitin *emo, int* tasapaino)
{
    Osoitin lapsi, lapsenlapsi;
    lapsi = (*emo)->vasen;

    /* LL-kierto */
    if (lapsi->korkeus == 1)
    {
        (*emo)->vasen = lapsi->oikea;
        lapsi->oikea = *emo;
        (*emo)->korkeus = 0;
        (*emo) = lapsi;
    }
    /* LR-kierto */
    else 
    {
        lapsenlapsi = lapsi->oikea;
        lapsi->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = lapsi;
        (*emo)->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = *emo;

        if (lapsenlapsi->korkeus == 1)
        {
            (*emo)->korkeus = -1;
            lapsi->korkeus = 0;
        }
        else if (lapsenlapsi->korkeus == 0)
        {
            (*emo)->korkeus = lapsi->korkeus = 0;
        }
        else if (lapsenlapsi->korkeus == -1)
        {
            (*emo)->korkeus = 0;
            lapsi->korkeus = 1;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->korkeus = 0;
    *tasapaino = 0;
}

void oikeaKierto(Osoitin *emo, int* tasapaino)
{
    Osoitin lapsi, lapsenlapsi;
    lapsi = (*emo)->oikea;

    /* RR-kierto */
    if (lapsi->korkeus == -1)
    {
        (*emo)->oikea = lapsi->vasen;
        lapsi->vasen = *emo;
        (*emo)->korkeus = 0;
        (*emo) = lapsi;
    }
    /* RL-kierto */
    else 
    {
        lapsenlapsi = lapsi->vasen;
        lapsi->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = lapsi;
        (*emo)->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = *emo;

        if (lapsenlapsi->korkeus == 1)
        {
            (*emo)->korkeus = -1;
            lapsi->korkeus = 0;
        }
        else if (lapsenlapsi->korkeus == 0)
        {
            (*emo)->korkeus = lapsi->korkeus = 0;
        }
        else if (lapsenlapsi->korkeus == -1)
        {
            (*emo)->korkeus = 0;
            lapsi->korkeus = 1;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->korkeus = 0;
    *tasapaino = 0;
}

/* Tulostetaan puu vasemmalta oikealle */
void tulostaPuu(Osoitin emo, int vali)
{
    int i;
    if (emo == NULL)
    {
        return;
    }
    vali += 6;
    tulostaPuu(emo->oikea, vali);
    printf("\n");
    for (i = 4; i < vali; i++)
    {
        printf(" ");
    }
    printf("%d(%d)\n", emo->data, emo->korkeus);
    tulostaPuu(emo->vasen, vali);
}

/* Etsitään alkiota puusta */
void etsiAlkio(Osoitin emo, int avain)
{

    if (emo == NULL)
    {
        printf("Alkiota ei löytynyt puusta.\n");
        return;
    }

    if (emo->data == avain)
    {
        printf("Alkio löytyy puusta.\n");
        return;
    }
    else if (emo->data > avain)
    {
        etsiAlkio(emo->vasen, avain);
    }
    else
    {
        etsiAlkio(emo->oikea, avain);
    }
}

/* Vapautetaan muisti puusta */
void vapautaMuisti(Osoitin *emo)
{
    if (*emo == NULL)
    {
        return;
    }
    vapautaMuisti(&(*emo)->vasen);
    vapautaMuisti(&(*emo)->oikea);
    printf("Vapautettu %d\n",(*emo)->data);
    free(*emo);
}


int main()
{
    int i, vali = 0, avain, valinta, kaynnissa = 1;
    int lista[10000];
    int* alkiot;
    int tasapaino = 0;
    Osoitin puu = NULL;
    char tiedNimi[256];

    printf("Mistä tiedostosta avaimet luetaan?: ");
    scanf("%s", tiedNimi);

    alkiot = lueTiedosto(tiedNimi, lista);

    /* Lisätään solmut puuhun */
    for (i = 0; alkiot[i] != 0; i++)
    {
        lisaaSolmu(&puu, alkiot[i], &tasapaino);
    }
    tulostaPuu(puu, vali);

    while(kaynnissa == 1) {
        printf("Haluatko: \n");
        printf("1) Lisätä avaimen?\n");
        printf("2) Etsiä tiettyä alkiota puusta?\n");
        printf("0) Keskeyttää?\n");
        printf("Valinta: ");
        scanf("%d", &valinta);
        if(valinta == 1) {
            printf("Minkä avaimen haluat lisätä puuhun?: ");
            scanf("%d", &avain);
            lisaaSolmu(&puu, avain, &tasapaino);
            tulostaPuu(puu, vali);
        }
        else if (valinta == 2) {
            printf("Mitä avainta haluat etsiä puusta?: ");
            scanf("%d", &avain);
            etsiAlkio(puu, avain);

        }
        else if (valinta == 0) {
            printf("Keskeytetään\n");
            kaynnissa = 0;
        }
    }

    /* Vapautetaan muisti lopuksi */
    vapautaMuisti(&puu);
    return 0;
}
