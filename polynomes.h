#ifndef POLYNOMES_H
#define POLYNOMES_H
#define MINFINI -1

#include <complex.h>
#include <stdlib.h>
#include <stdio.h>

typedef double complex complexe;

struct polynome {
	complexe *coeffs; /* tableau de taille "taille", */
	int taille; /* doit être une puissance de 2 */
};

struct polynome *pn_zero(int taille);

/* libère la mémoire associée au polynome p */
void pn_liberer(struct polynome *p);

/* Affiche le polynome d'adresse p sur la sortie standard */
void pn_afficher(const struct polynome *p);

/* Retourne l'évaluation du polynome d'adresse p en le complexe z */
complexe pn_eval(const struct polynome *p, complexe z);

/* Retourne l'adresse d'un polynome qui est le produit des polynomes dont les
 * adresses sont passées en argument (retourne NULL en cas d'échec des
 * allocations mémoires).
 * La fonction appelante doit appeler pn_liberer sur l'adresse retournée pour libérer la
 * mémoire obtenue. */
struct polynome *pn_produit(const struct polynome *p, const struct polynome *q);

/* Retourne l'adresse d'un polynome qui est le produit des polynomes dont les
 * adresses sont passées en argument (retourne NULL en cas d'échec des
 * allocations mémoires).
 * L'algorithme utilisé est celui de Karatsuba.
 * La fonction appelante doit appeler pn_liberer sur l'adresse retournée pour libérer la
 * mémoire obtenue. */
struct polynome *pn_karatsuba(const struct polynome *p, const struct polynome *q);

/* fonctions auxiliaires pour l'affichage de complexes */

/* Met dans s le complexe z représenté par une chaîne de caractères */
char *sprint_cplx(char *s, complexe z);
/* Affiche sur la sortie standard le complexe z représenté par une chaîne de
 * caractères */
void print_cplx(complexe z);

/* Transformée de Fourier : Prend un tableau de complexes tab représentant les coefficients d'un polynome de taille taille, un tableau de
   racines de l'unités de taille taille_racines (qui est un multiple de
   \texttt{taille}) et met dans tab
   la transformée de Fourier à taille points du polynôme*/
int fft(complexe *tab, const complexe *racines, int taille, int taille_racines);

/* Transformée de Fourier inverse : Prend un tableau de points tab et met à sa place le résultat de la transformée de Fourier
   inverse des coefficients initialement présents dans tab en utilisant le
   tableau de racines.*/
int ifft(complexe *tab, const complexe *racines, int taille, int taille_racines);
complexe *racines_unite(int taille_racines);

/* Calcule le produit de deux polynomes passés en argument en utlisant fft et ifft*/
struct polynome *pn_produit_fft(const struct polynome *p, const struct polynome *q);
#endif
