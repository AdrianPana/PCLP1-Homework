#include <stdio.h>

void dec_to_bin(int n, char *s) //descompunere in baza 2, trec rezultatele resturilor direct in s in ordine inversa
{
    int nr = 0;

    while (n > 0)
    {
        s[7 - nr] = n % 2 + '0';
        n = n / 2;
        nr++;
    }

    for (; nr < 8; nr++)
        s[7 - nr] = '0';
}

int bin_to_dec(char *s)//trecere a sirului de caractere din baza 2 inapoi intr-un numar in baza 10
{
    int n = 0, i, p = 1;

    for (i = 7; i >= 0; i--)
    {
        n += p * (s[i] - '0');
        p *= 2;
    }

    return n;
}

void multip_matrix(int a[8][8], int b[8][8], int c[8][8]) //inmultesc cele doua matrice intre ele
{
    int k, i, j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            c[i][j] = 0;
            for (k = 0; k < 8; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
                if (c[i][j] == 2)
                    c[i][j] = 1;
            }
        }
}

void transp_matrix(int mat[8][8], int result[8][8]) //calculez transpusa matricei
{
    int i, j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            result[i][j] = mat[j][i];
        }
}

int quadrant_sum(int m[][8], int i1, int j1, int i2, int j2) //calculare scor submatrice
{
    int i, j, ok, sum = 0;

    //numarul liniilor doar cu 1
    for (i = i1; i <= i2; i++)
    {
        ok = 1;
        for (j = j1; j <= j2; j++)
        {
            if (m[i][j] == 0)
                ok = 0;
        }
        if (ok)
            sum++;
    }

    //numarul coloanelor doar cu 1
    for (j = j1; j <= j2; j++)
    {
        ok = 1;
        for (i = i1; i <= i2; i++)
        {
            if (m[i][j] == 0)
                ok = 0;
        }
        if (ok)
            sum++;
    }

    //verificare diagonala principala a submatricei
    ok = 1;
    for (i = 0; i < 4; i++)
        if (m[i1 + i][j1 + i] == 0)
            ok = 0;
    if (ok)
        sum++;

    //verificare diagonala secundara a submatricei
    ok = 1;
    for (i = 0; i < 4; i++)
        if (m[i1 + i][j2 - i] == 0)
            ok = 0;
    if (ok)
        sum++;

    return sum;
}

int sum_X_0(int m[][8])
{   
    //pentru a scrie mai putin, folosesc o functie auxiliara care repeta aceeasi pasi pentru toate 4 partile matricei
    return quadrant_sum(m, 0, 0, 3, 3) + quadrant_sum(m, 0, 4, 3, 7) + quadrant_sum(m, 4, 0, 7, 3) + quadrant_sum(m, 4, 4, 7, 7);
}

void print_matrix(int a[8][8])
{
    int i, j;
    char s[8];

    //iau fiecare linie din matrice si o transform in string-ul corespunzator scrierii binare
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
            s[j]=a[i][j]+'0';

        printf("%d\n",bin_to_dec(s));
    }

    
}

int main()
{
    int i, j, x;
    char s[10];

    int a[8][8], at[8][8], aat[8][8], a2[8][8];

    for (i = 0; i < 8; i++)
    {
        scanf("%d", &x);
        dec_to_bin(x, s);

        //trec fiecare numar scris in baza 2 intr-o linie a matricii a
        for (j = 0; j < 8; j++)
        {
            a[i][j] = s[j] - '0';
        }
    }

    //calculez a transpus, a*a transpus si a^2
    transp_matrix(a, at);
    multip_matrix(a, at, aat);
    multip_matrix(a, a, a2);

    int scor[3],Max=-1,indice;

    //calculez scorul fiecarei matrice
    scor[0] = sum_X_0(a);
    scor[1] = sum_X_0(aat);
    scor[2] = sum_X_0(a2);

    //aflu scorul maxim si matricea corespunzatoare
    for (i=0;i<3;i++) 
        if(Max<scor[i]) {Max=scor[i]; indice=i;}

    //printez elementele matricei cu scor maxim
    switch (indice)
    {
    case 0:
        print_matrix(a);
        break;
    
    case 1:
        print_matrix(aat);
        break;

    case 2:
        print_matrix(a2);
        break;
    }

    
    return 0;
}