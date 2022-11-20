#include <stdio.h>
#include <string.h>

void citire(int a[4][4], char culori[7][11])
{
    int i, j, k;
    char s[34], *p;

    //codific fiecare culoare de pe fata si o inlocuiesc cu un numar, pentru o implementare mai usoara
    for (i = 0; i < 3; i++)
    {
        scanf("%s", s);
        k = 0;
        p = strtok(s, ".");
        while (p)
        {
            for (j = 0; j < 6; j++)
                if (strcmp(p, culori[j]) == 0)
                    a[i][k] = j;
            k++;
            p = strtok(NULL, ".");
        }
    }
    return;
}

void afisare(int a[4][4], char culori[7][11])
{
    int i;

    //decodific fiecare numar si afisez culoarea corespunzatoare
    for (i = 0; i < 3; i++)
    {
        printf("%s.%s.%s\n", culori[a[i][0]], culori[a[i][1]], culori[a[i][2]]);
    }
}

void rotire(int a[4][4]) // rotirea unei matrici
{
    int aux;

    //rotesc piesele din colt
    aux = a[0][0];
    a[0][0] = a[2][0];
    a[2][0] = a[2][2];
    a[2][2] = a[0][2];
    a[0][2] = aux;

    //rotesc piesele din mijlocul marginilor
    aux = a[0][1];
    a[0][1] = a[1][0];
    a[1][0] = a[2][1];
    a[2][1] = a[1][2];
    a[1][2] = aux;
}

//Urmatoarele 6 functii functioneaza pe acelasi principiu, deci sunt aproape identice.
//Pentru o miscare de baza (F,B,U,D,L,R), se roteste fata cu numele miscarii, apoi se 
//modifica si elementele adiacente cu fata respectiva care fac parte din celelalte fete
//Observam ca miscarile complementare (F',B',U',D',L',R'), pot fi obtinute si prin repetitia
//de trei ori a celor de baza, astfel ca nu sunt implementate (pentru usurinta).

void F(int f[4][4], int u[4][4], int r[4][4], int d[4][4], int l[4][4])
{
    int i, aux;
    rotire(f);

    for (i = 0; i < 3; i++)
    {
        aux = u[2][i];
        u[2][i] = l[2 - i][2];
        l[2 - i][2] = d[0][2 - i];
        d[0][2 - i] = r[i][0];
        r[i][0] = aux;
    }
}

void B(int b[4][4], int u[4][4], int l[4][4], int d[4][4], int r[4][4])
{
    int i, aux;

    rotire(b);

    for (i = 0; i < 3; i++)
    {
        aux = u[0][i];
        u[0][i] = r[i][2];
        r[i][2] = d[2][2-i];
        d[2][2-i] = l[2-i][0];
        l[2-i][0] = aux;
    }
}

void U(int u[4][4], int f[4][4], int l[4][4], int b[4][4], int r[4][4])
{
    int i, aux;
    rotire(u);

    for (i = 0; i < 3; i++)
    {
        aux = f[0][i];
        f[0][i] = r[0][i];
        r[0][i] = b[0][i];
        b[0][i] = l[0][i];
        l[0][i] = aux;
    }
}

void D(int d[4][4], int f[4][4], int r[4][4], int b[4][4], int l[4][4])
{
    int i, aux;

    rotire(d);

    for (i = 0; i < 3; i++)
    {
        aux = f[2][i];
        f[2][i] = l[2][i];
        l[2][i] = b[2][i];
        b[2][i] = r[2][i];
        r[2][i] = aux;
    }
}

void L(int l[4][4], int f[4][4], int d[4][4], int b[4][4], int u[4][4])
{
    int i, aux;
    rotire(l);

    for (i = 0; i < 3; i++)
    {
        aux = f[i][0];
        f[i][0] = u[i][0];
        u[i][0] = b[2 - i][2];
        b[2 - i][2] = d[i][0];
        d[i][0] = aux;
    }
}

void R(int r[4][4], int f[4][4], int u[4][4], int b[4][4], int d[4][4])
{
    int i, aux;
    rotire(r);

    for (i = 0; i < 3; i++)
    {
        aux = f[i][2];
        f[i][2] = d[i][2];
        d[i][2] = b[2 - i][0];
        b[2 - i][0] = u[i][2];
        u[i][2] = aux;
    }
}

int main()
{

    int i, n;
    char culori[7][11], s[11];
    int f[4][4], b[4][4], l[4][4], r[4][4], u[4][4], d[4][4]; //6 matrici denumite intuitiv pentru usurinta implementarii

    for (i = 0; i < 6; i++)
    {
        scanf("%s", culori[i]);
    }

    //se citesc cele 6 fete in ordinea corespunzatoare si se codifica
    citire(f, culori);
    citire(b, culori);
    citire(u, culori);
    citire(d, culori);
    citire(l, culori);
    citire(r, culori);

    //pentru fiecare miscare citita, daca este de baza (F,B,U,D,L,R), se apeleaza o fata functia corespunzatoare.
    //daca miscarea este complementara (F',B',U',D',L',R'), atunci se apeleaza functia corespunzatoare de 3 ori
    //pentru a simula acelasi rezultat.
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        scanf("%s", s);
        if (strcmp(s, "F") == 0)
            F(f, u, r, d, l);
        else if (strcmp(s, "F'") == 0)
        {
            F(f, u, r, d, l);
            F(f, u, r, d, l);
            F(f, u, r, d, l);
        }
        else if (strcmp(s, "B") == 0)
            B(b, u, l, d, r);
        else if (strcmp(s, "B'") == 0)
        {
            B(b, u, l, d, r);
            B(b, u, l, d, r);
            B(b, u, l, d, r);
        }
        else if (strcmp(s, "U") == 0)
            U(u, f, l, b, r);
        else if (strcmp(s, "U'") == 0)
        {
            U(u, f, l, b, r);
            U(u, f, l, b, r);
            U(u, f, l, b, r);
        }
        else if (strcmp(s, "D") == 0)
            D(d, f, r, b, l);
        else if (strcmp(s, "D'") == 0)
        {
            D(d, f, r, b, l);
            D(d, f, r, b, l);
            D(d, f, r, b, l);
        }
        else if (strcmp(s, "L") == 0)
            L(l, f, d, b, u);
        else if (strcmp(s, "L'") == 0)
        {
            L(l, f, d, b, u);
            L(l, f, d, b, u);
            L(l, f, d, b, u);
        }
        else if (strcmp(s, "R") == 0)
            R(r, f, u, b, d);
        else if (strcmp(s, "R'") == 0)
        {
            R(r, f, u, b, d);
            R(r, f, u, b, d);
            R(r, f, u, b, d);
        }
    }

    //se decodifica fiecare fata in ordinea corespunzatoare si se afiseaza
    afisare(f, culori);
    afisare(b, culori);
    afisare(u, culori);
    afisare(d, culori);
    afisare(l, culori);
    afisare(r, culori);

    return 0;
}