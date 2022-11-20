#include <stdio.h>

int main()
{
    int m, n, i, j, nr = 0, x, y, dir, nrdiag;

    struct diagonale
    {
        int p, s;
    } v[201];

    struct puncte
    {
        int x, y, dp, ds;
    } w[10001];

    scanf("%d %d", &m, &n);

    for (i = 0; i <= 2 * m; i++)
        v[i].p = v[i].s = 0;

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &x, &y);

        //numar cati nebuni sunt pe fiecare diagonala
        v[m + y - x].p++;
        v[x + y + 1].s++;

        //memorez diagonalele pe care se afla fiecare nebun
        w[i].x = x;
        w[i].y = y;
        w[i].dp = m + y - x;
        w[i].ds = x + y + 1;

    }

    //numar cate diagonale au mai mult de un nebun
    for (i = 1; i <= 2 * m; i++)
    {
        if (v[i].p > 1)
        {
            nr+=(v[i].p-1)*v[i].p/2;
            dir = 1;
            nrdiag = i;
        }
        if (v[i].s > 1)
        {
            nr+=(v[i].s-1)*v[i].s/2;
            dir = -1;
            nrdiag = i;
        }
    }

    printf("%d\n", nr);
    if (nr != 1)
        printf("NU\n");
    else
    {
        int c1 = -1, c2, ok = 0;

        //aflu care doi nebuni se ataca
        for (i = 0; i < n; i++)
        {
            if ((dir == 1 && w[i].dp == nrdiag) || (dir == -1 && w[i].ds == nrdiag))
            {
                if (c1 == -1)
                    c1 = i;
                else
                    c2 = i;
            }
        }

        //sterg primul nebun din pozitia in care se afla si caut o pozitie cu diagonale libere
        v[w[c1].dp].p--;
        v[w[c1].ds].s--;

        for (i = 0; i < m; i++)
            for (j = 0; j < m; j++)
                if (v[m + j - i].p == 0 && v[j + i + 1].s == 0)
                {
                    ok = 1;
                    break;
                }

        //daca nu am gasit o solutie, sterg al doilea nebun si caut analog
        if (ok == 0)
        {
            v[w[c1].dp].p++;
            v[w[c1].ds].s++;

            v[w[c2].dp].p--;
            v[w[c2].ds].s--;

            for (i = 0; i < m; i++)
                for (j = 0; j < m; j++)
                    if (v[m + j - i].p == 0 && v[j + i + 1].s == 0)
                    {
                        ok = 1;
                        break;
                    }
        }

        if (ok)
            printf("DA\n");
        else
            printf("NU\n");
    }

    return 0;
}