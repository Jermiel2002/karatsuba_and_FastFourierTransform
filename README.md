# karatsuba_and_Fourier_Fast_Fourrier_transform

<a name="br1"></a> 

Produit de polynômes

Mehdi Naima et Pierre Rousselin

7 novembre 2023

L’objectif de ce TP est d’implémenter certains algorithmes de calcul polynomial vus

en cours, avec un accent sur le produit des polynômes : méthode naïve, algorithme de

Karatsuba et enﬁn le produit via la FFT (algorithme de Schönhage–Strassen).

Une petite bibliothèque de manipulation des polynômes (à coeﬃcients complexes) est

déjà partiellement écrite. Vous la trouverez en tapant cette commande dans votre termi-

nal :

wget <https://www.lipn.univ-paris13.fr/~dufour/fft.tar>

1 Découverte de la bibliothèque polynomes

1\.1 Les nombres complexes en C

Les types complexes sont apparus dans le langage C avec le standard C99 (en-tête

<complex.h>). Pour déclarer une variable complexe, on utilise le mot-clé complex, suivi

du type ﬂottant que l’on veut utiliser pour les parties réelles et imaginaires.

complex double z1;

complex float z2;

complex long double z3;

Pour simpliﬁer les déclarations, la bibliothèque polynomes contient l’alias de type

typdef complex double complexe;

Les opérations sur les complexes se font de façon naturelle (avec + - \* /). Le nombre

imaginaire i est accessible via la macro I. Les fonctions creal, cimag, conj et cabs←-

permettent d’obtenir, respectivement, les parties réelles et imaginaires, le conjugué et la

valeur absolue.

Il n’y a pas de spéciﬁcateur de format pour imprimer un complexe, donc la bibliothèque

polynomes contient les fonctions utilitaires

char \*sprint\_cplx(char \*s, complexe z);

void print\_cplx(complexe z);

1



<a name="br2"></a> 

1\.2 Le type struct polynome

On représente les polynômes de la façon suivante :

struct polynome {

complexe \*coeffs; /\* tableau de taille "taille", \*/

int taille; /\* doit être une puissance de 2 \*/

};

Par exemple la suite de commandes suivante

struct polynome \*p = malloc(sizeof(struct polynome));

p->taille = 4;

p->coeffs = (complexe \*) malloc (4 \* sizeof(complexe));

p->coeffs [0] = 1;

p->coeffs [1] = I;

p->coeffs [2] = 3;

p->coeffs [3] = 0;

donne le polynôme :

P(X) = 1 + iX + 3X<sup>2</sup>

Noter que la taille doit toujours être une puissance de 2, on complète par des 0.

Le plus simple est encore d’appeler la fonction pn\_zero, déjà écrite avec la bonne taille :

struct polynome \*p = pn\_zero (3); /\* ce sera 4 du coup car pn\_zero

donne une taille qui est une puissance de 2 \*/

p->coeffs [0] = 1;

p->coeffs [1] = I;

p->coeffs [2] = 3;

2 Échauﬀement

Implémenter les fonctions

complexe pn\_eval(const struct polynome \*p, complexe z);

struct polynome \*pn\_produit(const struct polynome \*p, const struct ←-

polynome \*q);

en respectant les spéciﬁcations données dans le ﬁchier polynomes.h.

Tester chaque fonction en mettant à 1 (au lieu de 0) la constante symbolique associée

dans le programme de test test-pn.c : par exemple, pour tester la fonction pn\_eval, il

suﬃt, avant de compiler, de changer

#define TEST\_EVAL 0

en

2



<a name="br3"></a> 

#define TEST\_EVAL 1

3 Karatsuba

On rappelle le principe de l’algorithme de Karatsuba : Soient P et Q deux polynômes

de degré inférieur ou égal à 2<sup>n</sup> − 1, avec n ∈ N : On peut écrire P de la façon suivante :

<sup>2</sup>X<sup>n</sup>−1

α X = P (X) + X<sup>2n−1</sup> P<sub>d</sub>(X),

i

P(X) =

i

g

i=0

où P et P sont les polynômes de degré inférieur ou égal à 2<sup>n−1</sup> − 1 :

g

d

<sup>2n</sup>X <sup>1</sup>

<sup>−1</sup>−

<sup>2</sup> X <sup>1</sup>

<sup>n−1</sup>−

P<sub>g</sub>(X) =

α X<sup>i</sup>

i

et

P<sub>d</sub>(X) =

α<sub>2n−1+i</sub>X<sup>i</sup>

i=0

i=0

et de même pour Q. Le produit PQ peut alors être écrit de la façon (très astucieuse !)

suivante :

<sub>2n</sub>−1

n

R(X) = PQ(X) = P Q (X)+((P +P )(Q +Q )−P Q −P Q )X

+X<sup>2</sup> P Q (X).

g

g

g

d

g

d

g

g

d

d

d

d

et l’on voit qu’il ne faut plus calculer que 3 produits de polynômes dont la taille a été

divisée par 2, ces produits sont :

R = P Q R = P Q R = P Q ,

g

g

g

d

d

d

s

s

s

où P = (P + P ) et Q = (Q + Q ).

s

g

d

s

g

d

On peut alors récursivement calculer ces trois produits. Une fonction pn\_karatsuba est

déjà écrite pour vous (ne vous réjouissez pas trop vite, c’est aux\_karatsuba qui fait tout

le travail).

Question : La fonction pn\_karatsuba utilise une fonction

static struct polynome \*aux\_karatsuba(complexe \*p, complexe \*q, int ←-

taille);

qui prend deux adresses de tableaux de complexes de taille taille (qui est une puissance

de 2) et retourne l’adresse d’un tableau nouvellement alloué de taille 2×taille contenant

le produit des deux tableaux calculé par l’algorithme de Karatsuba.

Implémentez la fonction aux\_karatsuba. Vous pouvez introduire des fonctions supple-

mentaires selon vos besoins, par exemple, une fonction qui calcule la somme des parties

gauche et droite d’un tableau.

Testez votre fonction et sa performance à l’aide du programme de test fourni.

3



<a name="br4"></a> 

4 Produit via la FFT

4\.1 Fast Fourier Transform

Soit P un polynôme de degré inférieur à n. La transformée de Fourier discrète à n

points du polynôme P est le vecteur de C<sup>n</sup> :

ꢀ

ꢁ

0

1

n−1

n

F (P) = P(ω ), P(ω ), . . . , P(ω

) ,

n

n

n

où ω<sub>n</sub> = exp(2iπ/n).

Question : Avec une méthode « naïve », quelle est la complexité du calcul de cette

transformée de Fourier ?

L’algorithme FFT, très astucieux, utilise le principe « diviser pour régner » pour avoir

une complexité en O(n log(n)). À partir de maintenant, on suppose que n est une puis-

sance de 2 : n = 2<sup>k</sup>.

On écrit

<sup>2</sup>X<sup>k</sup>−1

i

P(X) =

a X = P<sub>pair</sub>(X<sup>2</sup>) + XP<sub>impair</sub>(X<sup>2</sup>),

i

i=0

P

2

i=0

<sup>k−1</sup>−1

a<sub>2i</sub>X<sup>i</sup> est le polynôme obtenu en ne conservant que les coeﬃcients

où P<sub>pair</sub>(X) =

P

2

<sup>k−1</sup>−1

i=0

de degré pairs de P et P<sub>impair</sub>(x) =

termes de degré impair de P. Remarquer que la taille des polynômes P

a<sub>2i+1</sub>X<sup>i</sup> est celui qu’on obtient avec les

et de P<sub>impair</sub>

pair

est deux fois moindre que celle de P. On suppose qu’on sait calculer la transformée de

Fourier discrète à n/2 points de P<sub>pair</sub> et P<sub>impair</sub>. Alors (magie...), pour tout i entre 0 et

n − 1,

i

n

i

2

i

n

((ω ) ) = P (ω<sup>i</sup> ) + ω P

i

2

i

(ω<sup>i</sup> ).

impair

n/2

P(ω ) = P ((ω ) ) + ω P

pair

impair

pair

n

n

n/2

n

On remarque que si i ≥ n/2, alors on peut écrire i = n/2 + j et

i

n/2

n/2+j

n/2

= ω<sup>j</sup>

ω

= ω

n/2

On peut donc calculer la transformée de Fourier à n points de P grâce à la transformée

de Fourier à n/2 points de P<sub>pair</sub> et P<sub>impair</sub> (qui sont de taille n/2) :

0

0

n/2

0

0

n/2

P(ω ) = P<sub>pair</sub>(ω ) + ω P<sub>impair</sub>(ω

)

)

n

n

1

1

n/2

1

n

1

n/2

P(ω ) = P<sub>pair</sub>(ω ) + ω P<sub>impair</sub>(ω

n

. . .

P(ω<sup>n/2−1</sup>) = P<sub>pair</sub>(ω<sup>n/2−1</sup>) + ω<sub>n</sub><sup>n/2−1</sup>P<sub>impair</sub>(ω

n/2−1

n/2

)

n

n/2

P(ω<sub>n</sub><sup>n/2</sup>) = P<sub>pair</sub>(ω ) + ω<sub>n</sub> P<sub>impair</sub>(ω<sub>0</sub>

n/2

0

n/2

n/2

)

P(ω<sub>n</sub><sup>n/2+1</sup>) = P<sub>pair</sub>(ω ) + ω<sub>n</sub><sup>n/2+1</sup>P<sub>impair</sub>(ω<sup>1</sup>

1

n/2

)

n/2

. . .

P(ω<sup>n−1</sup>) = P<sub>pair</sub>(ω<sup>n/2−1</sup>) + ω<sub>n</sub> P<sub>impair</sub>(ω

n−1

n/2−1

n/2

)

n

n/2

4



<a name="br5"></a> 

Description informelle de l’algorithme :

L’algorithme est récursif et calcule le résultat directement dans le tableau des entrées

tab.

1\. Case de base : Si la taille actuelle du tableau de complexes est égale à 2 (donc

P(X) = A + BX), alors tab[0] = A + B et tab[1] = A − B.

2\. Sinon :

a) On sépare le polynôme comme décrit ci-dessus en 2 polynômes (pair et impair)

de taille deux fois moindre.

b) On appelle récursivement la transformée de Fourier rapide sur chacun des deux

polynômes.

c) On recombine les résultats comme décrit auparavant.

n

2

k−

On pourra aussi exploiter le fait que ω<sup>k</sup> = −ω

.

n

n

Question : Implémenter la fonction

int fft(complexe \*tab , const complexe \*racines , int taille , int ←-

taille\_racines);

qui prend une tableau de complexes tab de taille taille, un tableau de racines de l’unités

de taille taille\_racines (qui est un multiple de taille) et met dans tab la transformée

de Fourier à taille points du polynôme dont les coeﬃcients étaient les éléments de tab.

4\.2 Transformée inverse

La transformée de Fourier inverse à n points d’un vecteur

[αˆ , αˆ , . . . , αˆ<sub>n−1</sub>]

0

1

est le tableau

ꢂ

ꢃ

1

1

1

P(ω ), P(ω<sup>−1</sup>), . . . , P(ω<sup>−(n−1)</sup>)

ˆ

−0

ˆ

ˆ

n

n

n

n

n

n

où le polynôme Pˆ est donné par

<sup>n</sup>X−1

i=0

ˆ

P(X) =

αˆ<sub>i</sub>.

En fait, presque tout le travail est déjà fait car ω−<sup>i</sup> = ω<sup>n</sup>−<sup>i</sup> donc il suﬃt de calculer

n

n

la transformée de Fourier à n points de Pˆ, de diviser ses coeﬃcients par n et de les

réordonner.

Question : Implémenter la fonction

int ifft(complexe \*tab , const complexe \*racines , int taille , int ←-

taille\_racines);

5



<a name="br6"></a> 

qui met dans le tableau tab le résultat de la transformée de Fourier inverse des coeﬃcients

initialement présents dans tab en utilisant le tableau de racines racines. Tester votre

fonction à l’aide de la partie TEST\_IFFT et vériﬁez que la transformée inverse de la

transormée redonne le polynôme (ses coeﬃcients) de départ.

4\.3 Produit par FFT

Description informelle de l’algorithme :

On suppose que les degrés des deux polynômes est une puissance de deux.

1\. La taille du tableau de complexes résultat est donc le double du maximum entre la

taille de P et la taille de Q.

2\. On calcule les racines n-ièmes de l’unité dans un tableau.

3\. On calcule la transformée de Fourier rapide sur chacun des polynômes en entrée.

4\. On calcule dans un tableau de complexes les points du polynôme résultat en mul-

tipliant simplement terme à terme les éléments des deux tableaux de complexes

obtenus lors de la transormée de Fourier.

5\. Finalement, on utilise la transormée de Fourier inverse sur le tableau obtenu aﬁn

d’obtenir les coeﬃcients du polynôme résultat.

Question : Implémenter la fonction

struct polynome \*pn\_produit\_fft(const struct polynome \*p, const struct ←-

polynome \*q);

qui retourne un pointeur vers un polynome nouvellement alloué contenant le produit des

polynômes d’adresses p et q en utilisant la FFT.

6

