#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <complex.h>
#include <string.h>
#include "polynomes.h"

#define TEST_EVAL 1
#define TEST_PROD 1
#define TEST_KARA 0
#define TEST_KARA_VS_NAIF 0
#define TEST_FFT 1
#define TEST_IFFT 0
#define TEST_PRODUIT_FFT 0
#define TEST_FFT_VS_KARA 0

#define DEBTEST(chaine) printf(" *** début : %s *** \n", (chaine));
#define FINTEST(chaine) printf(" *** fin : %s *** \n", (chaine));
int main()
{
#if TEST_EVAL == 1
	{
#define NB_CARS 20 /* nombre max de caractères pour l'affichage d'un complexe */
#define Afficher_image(p, z, fonction_p_z) { \
	char ch1[NB_CARS], ch2[NB_CARS]; \
	printf("P(%s) = %s\n", sprint_cplx(ch1, z), sprint_cplx(ch2, fonction_p_z(p, z))); \
}
		struct polynome *p = pn_zero(4);
		DEBTEST("pn_eval");
		printf("Pour le polynôme P(X) = ");
		pn_afficher(p);
		Afficher_image(p, 0, pn_eval);
		Afficher_image(p, 1 + I, pn_eval);
		Afficher_image(p, -2, pn_eval);

		p->coeffs[2] = 1;
		printf("Pour le polynôme P(X) = ");
		pn_afficher(p);
		Afficher_image(p, 0, pn_eval);
		Afficher_image(p, 1 + I, pn_eval);
		Afficher_image(p, -2, pn_eval);

		p->coeffs[0] = I;
		p->coeffs[1] = 3;
		printf("Pour le polynôme P(X) = ");
		pn_afficher(p);
		Afficher_image(p, 0, pn_eval);
		Afficher_image(p, 1 + I, pn_eval);
		Afficher_image(p, -2, pn_eval);

		FINTEST("pn_eval");
		pn_liberer(p);
#undef NB_CARS
#undef Afficher_image
	}
#endif
#if TEST_PROD == 1
	{
		struct polynome *p = pn_zero(4), *q = pn_zero(4), *r;
		DEBTEST("produit")
		q->coeffs[3]=-2;
		q->coeffs[2]=1;
		q->coeffs[1]=I;
		q->coeffs[0]=10;

		p->coeffs[3]=2;
		p->coeffs[2]=-1;
		p->coeffs[1]=1;
		p->coeffs[0]=3;

		pn_afficher(p);
		pn_afficher(q);

		r = pn_produit(p, q);

		pn_afficher(r);

		FINTEST("produit")
		pn_liberer(p);
		pn_liberer(q);
		pn_liberer(r);
	}
#endif
#if TEST_KARA == 1
	{
		struct polynome *p = pn_zero(4), *q = pn_zero(4), *r;
		DEBTEST("Karatsuba")
		q->coeffs[3]=-2;
		q->coeffs[2]=1;
		q->coeffs[1]=I;
		q->coeffs[0]=10;

		p->coeffs[3]=2;
		p->coeffs[2]=-1;
		p->coeffs[1]=1;
		p->coeffs[0]=3;

		pn_afficher(p);
		pn_afficher(q);

		r = pn_karatsuba(p, q);

		pn_afficher(r);

		FINTEST("Karatsuba")
		pn_liberer(p);
		pn_liberer(q);
		pn_liberer(r);
	}
#endif
#if TEST_KARA_VS_NAIF == 1
#define BIGN 0x10000
	{
		struct polynome *p = pn_zero(BIGN), *q = pn_zero(BIGN), *r1, *r2;
		clock_t tic, toc;
		int i;
		for (i = 0; i < BIGN; ++i) {
			p->coeffs[i] = rand() % 10;
			q->coeffs[i] = rand() % 10;
		}
		DEBTEST("Performances multiplications (naïf vs Karatsuba)");
		tic = clock();
		r1 = pn_produit(p, q);
		toc = clock();
		printf("Multiplication naïve sur polynomes de taille %d : %g\n",
				BIGN,
				(double) (toc - tic) / CLOCKS_PER_SEC);
		tic = clock();
		r2 = pn_karatsuba(p, q);
		toc = clock();
		printf("Karatsuba sur polynomes de taille %d : %g\n",
				BIGN,
				(double) (toc - tic) / CLOCKS_PER_SEC);
		FINTEST("Performances multiplications (naïf vs Karatsuba)");
		pn_liberer(p); pn_liberer(q); pn_liberer(r1); pn_liberer(r2);
	}
#endif
#if TEST_FFT == 1
	{
		struct polynome *p = pn_zero(4);
		complexe *r, *copie = malloc(4 * sizeof(complexe));
		int i;
		DEBTEST("FFT")

		p->coeffs[3]=2;
		p->coeffs[2]=-1;
		p->coeffs[1]=1;
		p->coeffs[0]=3;

		memcpy(copie, p->coeffs, 4 * sizeof(complexe));
		pn_afficher(p);
		r = racines_unite(4);
		fft(copie, r, 4, 4);
		printf("Transformée de Fourier à %d points : \n", 4);
		for (i = 0; i < 4; i++) {
			print_cplx(copie[i]);
			printf("\t");
		}
		printf("\n");

		FINTEST("FFT")
		free(r);
		free(copie);
		pn_liberer(p);
	}
#endif
#if TEST_IFFT == 1
	{
		struct polynome *p = pn_zero(4);
		complexe *r, *copie = malloc(4 * sizeof(complexe));
		int i;
		DEBTEST("FFT")

		p->coeffs[3]=2;
		p->coeffs[2]=-1;
		p->coeffs[1]=1;
		p->coeffs[0]=3;

		memcpy(copie, p->coeffs, 4 * sizeof(complexe));
		pn_afficher(p);
		r = racines_unite(4);
		fft(copie, r, 4, 4);
		printf("Transformée de Fourier à %d points : \n", 4);
		for (i = 0; i < 4; i++) {
			print_cplx(copie[i]);
			printf("\t");
		}
		printf("\n");

		ifft(copie, r, 4, 4);
		printf("Transformée de Fourier inverse à %d points"
			"de la transformée de Fourier : \n", 4);
		for (i = 0; i < 4; i++) {
			print_cplx(copie[i]);
			printf("\t");
		}
		printf("\n");


		FINTEST("FFT")
		pn_liberer(p);
		free(r);
		free(copie);
	}
#endif
#if TEST_PRODUIT_FFT == 1
	{
		struct polynome *p = pn_zero(4), *q = pn_zero(4), *r;
		DEBTEST("PRODUIT_FFT")
		q->coeffs[3]=-2;
		q->coeffs[2]=1;
		q->coeffs[1]=I;
		q->coeffs[0]=10;

		p->coeffs[3]=2;
		p->coeffs[2]=-1;
		p->coeffs[1]=1;
		p->coeffs[0]=3;

		pn_afficher(p);
		pn_afficher(q);

		r = pn_produit_fft(p, q);

		pn_afficher(r);

		FINTEST("FFT")
		pn_liberer(p);
		pn_liberer(q);
		pn_liberer(r);
	}
#endif
#if TEST_FFT_VS_KARA == 1
#define BIGN 0x10000
	{
		struct polynome *p = pn_zero(BIGN), *q = pn_zero(BIGN), *r1, *r2;
		double erreur = 0;
		clock_t tic, toc;
		int i;
		for (i = 0; i < BIGN; ++i) {
			p->coeffs[i] = rand() % 10;
			q->coeffs[i] = rand() % 10;
		}
		DEBTEST("Performances multiplications (Karatsuba vs FFT)");
		tic = clock();
		r1 = pn_karatsuba(p, q);
		toc = clock();
		printf("Karatsuba sur polynomes de taille %d : %g\n",
				BIGN,
				(double) (toc - tic) / CLOCKS_PER_SEC);
		tic = clock();
		r2 = pn_produit_fft(p, q);
		toc = clock();
		printf("Produit par FFT sur polynomes de taille %d : %g\n",
				BIGN,
				(double) (toc - tic) / CLOCKS_PER_SEC);

		/* Calcul de l'erreur max */
		for (i = 0; i < BIGN; ++i) {
			double err = cabs(r1->coeffs[i] - r2->coeffs[i]);
			if (erreur < err)
				erreur = err;
		}
		printf("Différence maximale entre FFT et Karatsuba : %g\n", erreur);
		FINTEST("Performances multiplications (naïf vs Karatsuba)");
		pn_liberer(p); pn_liberer(q); pn_liberer(r1); pn_liberer(r2);
	}
#endif
	return 0;
}
