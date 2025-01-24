#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//nisu sve implementirane metode koriscene
typedef struct zaposleni {
	int id;
	char imeiprezime[50];
	double cena_rada;//po satu

}ZAPOSLENI;

typedef struct angazovanje {
	int id_zaposlenog;
	int broj_sati;
	int id_angazovanja; //zadnje 2 cifre su id projekta
	char naziv_projekta[100];

}ANGAZOVANJE;

typedef struct cvor CVOR;
typedef struct cvor* PCVOR;
struct cvor {
	ANGAZOVANJE angazovanje;
	PCVOR sledeci;
};

void unos_zaposlenog(ZAPOSLENI zaposleni[], int *n) {
	ZAPOSLENI z;
	char pomocni[100];
	printf("Unesite id:\n");
	scanf("%d", &z.id);
	getchar();
	printf("Unesite ime I prezime:\n");
	scanf("%s %s", z.imeiprezime, pomocni);
	getchar();
	strcat(z.imeiprezime, " ");
	strcat(z.imeiprezime, pomocni);
	printf("Unesite cenu rada po satu:\n");
	scanf("%lf", &z.cena_rada);
	getchar();
	zaposleni[(*n)] = z;
	(*n)++;
}

double izracunaj_medijanu(double cene_rada[], int n) {
	if (n == 0) {
		printf("prazan niz");
		return;
	}
	double p = cene_rada[0];
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (cene_rada[i] > cene_rada[j]) {
				p = cene_rada[i];
				cene_rada[i] = cene_rada[j];
				cene_rada[j] = p;
			}
		}
	}


	if (n % 2 == 1) {
		return cene_rada[n / 2];
	}
	else {
		p = (cene_rada[n / 2] + cene_rada[n / 2 - 1]) / 2;
		return p;
	}
}

void izbaci(int id, ZAPOSLENI zaposleni[], int* n) {
	int i = 0;
	for (i = 0; i < (*n); i++) {
		if (zaposleni[i].id == id) {
			break;
		}
	}

	for (int j = i; j < (*n)-1; j++) {
		zaposleni[j] = zaposleni[j + 1];
	}

	/*zaposleni[(*n)] = NULL;*/
	(*n)--;
}

void pretrazi_po_imenu(char pretraga[], ZAPOSLENI zaposleni[], int n) {
	int p = 1;

	for (int i = 0; i < n; i++) {
		p = zavrsava_sa(zaposleni[i].imeiprezime, pretraga);
		if (p == 0) {
			printf("%s, %d, %lf         ", zaposleni[i].imeiprezime, zaposleni[i].id, zaposleni[i].cena_rada);
		}
		p = 1;
	}
}

int zavrsava_sa(char s1[], char s2[]) {
	int ss1 = strlen(s1);
	int ss2 = strlen(s2);
	if (ss2 > ss1) {
		printf("ne moze");
		return;
	}

	for (int i = ss2; i > 0; i--) {
		if (s2[i] != s1[ss1]) {
			return 0;
		}
		ss1--;
	}

	return 1;
}

void dodaj_angazovanje(ANGAZOVANJE a, PCVOR* glava) {
	PCVOR novi = (PCVOR)malloc(sizeof(CVOR));
	novi->angazovanje = a;
	novi->sledeci = NULL;
	PCVOR tek = (*glava);
	if (tek == NULL) {
		(*glava) = novi;
		return;
	}
	while (tek->sledeci != NULL) {

		tek = tek->sledeci;
	}
	tek->sledeci = novi;
}

double broj_sati_projekta(int id_projekta, PCVOR glava) {
	double br = 0;
	while (glava != NULL) {
		if (glava->angazovanje.id_angazovanja % 10 == id_projekta % 10 && glava->angazovanje.id_angazovanja / 10 % 10 == id_projekta / 10 % 10) {
			br = br + glava->angazovanje.broj_sati;
		}
		glava = glava->sledeci;
	}
	return br;
}

void ispisi_projekte(char* naziv_datoteke, PCVOR glava) {
    printf("Projekti:\n");
	ANGAZOVANJE niz[100];
	int n = 0;
	int p = 0;
	PCVOR tek = glava;
	while (tek != NULL) {
		p = 0;
		for (int i = 0; i < n; i++) {
			if (!strcmp(tek->angazovanje.naziv_projekta, niz[i].naziv_projekta)) {
				p = 1;
			}
		}
		if (p == 0) {
			niz[n] = tek->angazovanje;
			n++;
		}
		tek = tek->sledeci;
	}
	FILE* datoteka = fopen(naziv_datoteke, "w");
	if (datoteka == NULL) {
		printf("greska 0");
		return;
	}
	for (int i = 0; i < n; i++) {
		fprintf(datoteka, "%s\n", niz[i].naziv_projekta);
		printf("%s\n", niz[i].naziv_projekta);
	}

	fclose(datoteka);
}

void oslobodi_listu(PCVOR* glava) {
    while (*glava) {
        PCVOR temp = *glava;
        *glava = (*glava)->sledeci;
        free(temp);
    }
}

int main(void) {
	ZAPOSLENI zaposleni[100];
    ANGAZOVANJE a;
	PCVOR glava = NULL;
	int n = 0; //broj zaposlenih
    int kr = 1;  //da li treba da se prekine program
	int u = 0;  //unos
	int u1 = 0; //da li ima unos
    while (kr == 1) {
		printf("\n   ###MENI###   \n1.Unesi zaposlenog\n2.Izracunaj medijanu\n3.Nadji zaposlenog i dodaj angazovanje\n4.Izbaci zaposlenog\n5.Prikazi projekte\n0.Izlaz\n");
		u1=scanf("%d", &u);
		if (u1 == 0) {
			printf("\ngreska\n");
			return 1;
		}
		switch (u) {
		case 1:
			unos_zaposlenog(zaposleni, &n);
            printf("Unet: ime:%s, id:%d, cena rada:%lf         ", zaposleni[n-1].imeiprezime, zaposleni[n-1].id, zaposleni[n-1].cena_rada);
			break;
		case 2:
            double cene_rada[100];
            for (int i = 0; i < n; i++)
             {
            cene_rada[i]=zaposleni[i].cena_rada;
            }
        
        
			double d = izracunaj_medijanu(cene_rada, n);
	        printf("medijana je %lf", d);
			break;
		case 3:
        	for (int i = 0; i < n; i++) {
		    printf("%s, %d, %lf         ", zaposleni[i].imeiprezime, zaposleni[i].id, zaposleni[i].cena_rada);
	        }
            printf("\nUnesite id zaposlenog cije angazovanje zelite da dodate");
            u1=scanf("%d", &u);
            getchar();
            if (u1 == 0) {
			printf("\ngreska\n");
            break;
		    }
            a.id_zaposlenog=u;
            printf("\nUnesite broj angazovanja");
            u1=scanf("%d", &u);
            getchar();
            if (u1 == 0) {
			printf("\ngreska\n");
            break;
		    }
            a.id_angazovanja = u;
            printf("\nUnesite broj sati");
            u1=scanf("%d", &u);
            if (u1 == 0) {
			printf("\ngreska\n");
            break;
		    }
	        a.broj_sati = u;
            char name[50];//


            //
            printf("\nUnesite naziv projekta");
            u1=scanf("%s", name);
        	getchar();
            if (u1 == 0) {
			printf("\ngreska\n");
            break;
		    }
	        strcat(a.naziv_projekta, name);
            dodaj_angazovanje(a, &glava);
			break;
		case 4:
            for (int i = 0; i < n; i++) {
		    printf("%s, %d, %lf         ", zaposleni[i].imeiprezime, zaposleni[i].id, zaposleni[i].cena_rada);
	        }
            printf("\nUnesite id zaposlenog kog zelite da izbacite");
            u1=scanf("%d", &u);
            getchar();
            if (u1 == 0) {
			printf("\ngreska\n");
            break;
		    }
			izbaci(u, zaposleni, &n);
			break;
		case 5:
			ispisi_projekte("projekti.txt", glava);
	        getchar();
			break;
		case 0:
			return 0;
		default:
			printf("greska1");
			break;

		}
	}


	
	//int z=zavrsava_sa("peric", "ic");
	//printf("%d", z);
	//z = zavrsava_sa("peri", "ic");
	//printf("%d", z);
	//z = zavrsava_sa("peric", "eric");
	//printf("%d", z);
	//101
	/*printf("\npre\n");
	for (int i = 0; i < n; i++) {
		printf("%s, %d, %lf         ", zaposleni[i].imeiprezime, zaposleni[i].id, zaposleni[i].cena_rada);
	}
	printf("\nposle\n");
	pretrazi_po_imenu("ic", zaposleni, n);*/
	




	//while (glava != NULL) {
	//	printf("%d  ", glava->angazovanje.id_angazovanja);
	//	glava = glava->sledeci;
	//}
	//double p = 0;
	//p=broj_sati_projekta(22, glava);
	//printf("broj sati je %lf", p);//9


}