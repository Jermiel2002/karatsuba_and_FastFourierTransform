#include "polynomes.h"
#include <string.h> /* memcpy */
#include <math.h> /* sin et cos */
#define DEG_0 -1

#define MAX(x,y) ((x) > (y) ? (x) : (y))

/* retourne le degré du polynôme p, DEG_0 si p est nul */
static int pn_deg(const struct polynome *p);

/* retourne un pointeur vers un nouveau polynome nul dont la taille est la plus
 * petite puissance de 2 >= taille */
struct polynome *pn_zero(int taille)
{
	struct polynome *p = malloc(sizeof(struct polynome));
	int puiss_2;
	if (!p || taille <= 0)
		return NULL;
	/* Forcer taille à être une puissance de 2 */
	for (puiss_2 = 1; puiss_2 < taille; puiss_2 *= 2)
		;
	taille = puiss_2;
	p->coeffs = calloc(taille, sizeof(complexe));
	if (!(p->coeffs)) {
		free(p);
		return NULL;
	}
	p->taille = taille;
	return p;
}

static int pn_deg(const struct polynome *p)
{
	int deg;
	for (deg = p->taille - 1; deg >= 0 && p->coeffs[deg] == 0; --deg)
		;
	if (deg == 0 && p->coeffs[0] == 0)
		deg = DEG_0;
	return deg;
}

void pn_liberer(struct polynome *p)
{
	if (p) {
		free(p->coeffs);
		free(p);
	}
}

/* Met dans la chaîne s une représentation sous forme de chaîne du complexe z */
char *sprint_cplx(char *s, complexe z)
{
	double r = creal(z), i = cimag(z);
	if (!i) {
		sprintf(s, "%g", r);
		return s;
	}
	if (!r) {
		sprintf(s, "%gI", i);
		return s;
	}
	sprintf(s, "(%g + %gI)", r, i);
	return s;
}

/* Affiche sur le terminal une représentation sous forme de chaîne du complexe z */
void print_cplx(complexe z)
{
	double r = creal(z), i = cimag(z);
	if (!i) {
		printf("%g", r);
		return;
	}
	if (!r) {
		printf("%gI", i);
		return;
	}
	printf("(%g + %gI)", r, i);
}

void pn_afficher(const struct polynome *p)
{
	int k;
	complexe cf;
	for (k = pn_deg(p); k > 0; --k) {
		cf = p->coeffs[k];
		print_cplx(cf);
		printf("X^%d + ", k);
	}
	cf = p->coeffs[0];
	print_cplx(cf);
	puts("");
}

complexe pn_eval(const struct polynome *p, complexe z)
{
	complexe res = 0;
	return res;
}

struct polynome *pn_produit(const struct polynome *p, const struct polynome *q)
{
	struct polynome *r = NULL;
	return r;
}

/* aux_karatsuba travaille directement avec des tableaux */
static complexe *aux_karatsuba(complexe *p, complexe *q, int sz)
{
	complexe *r = NULL;
	return r;
}

/* Sur la fonction suivante : on pourrait faire moins de copies, mais ça
 * rendrait le code un peu plus compliqué.
 * Autres solutions :
 * * enlever const et utiliser "agrandir" pour que les polynomes aient
 * la même taille.
 * * laisser à la fonction appelante la responsabilité d'appeler cette
 * fonction avec des polynomes de même taille.
 */
struct polynome *pn_karatsuba(const struct polynome *p, const struct polynome *q)
{
	complexe *ptab, *qtab;
	int sz = MAX(p->taille, q->taille);
	struct polynome *r;
	/* On fait des copies pour que les polynomes aient la même taille */
	ptab = calloc(sz, sizeof(complexe));
	qtab = calloc(sz, sizeof(complexe));
	r = malloc(sizeof(struct polynome));
	if (!(ptab && qtab && r)) {
		free(ptab);
		free(qtab);
		free(r);
		return NULL;
	}
	memcpy(ptab, p->coeffs, p->taille * sizeof(complexe));
	memcpy(qtab, q->coeffs, q->taille * sizeof(complexe));
	r->coeffs = aux_karatsuba(ptab, qtab, sz);
	r->taille = 2 * sz;
	free(ptab);
	free(qtab);
	return r;
}

int fft(complexe *tab, const complexe *racines, int taille, int taille_racines)
{
	return 0;
}

int ifft(complexe *tab, const complexe *racines, int taille, int taille_racines)
{
	return 0;
}

complexe *racines_unite(int taille_racines)
{
	complexe *r = malloc(taille_racines * sizeof(complexe));
	int i;
	double theta = atan(1) * 8 / taille_racines;
	if (!r)
		return NULL;
	for (i = 0; i < taille_racines; ++i)
		r[i] = cos(i * theta) + I * sin(i * theta);
	return r;
}


struct polynome *pn_produit_fft(const struct polynome *p, const struct polynome *q)
{
	struct polynome *r = NULL;
	return r;
}
