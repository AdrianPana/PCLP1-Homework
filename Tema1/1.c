#include <stdio.h>
#include <string.h>

int is_anagram(char *s1, char *s2)
{
    int v[30], i;
    for (i = 0; i < 26; i++)
        v[i] = 0;

    if (strlen(s1) != strlen(s2))
        return 0;

    for (i = 0; i < strlen(s1); i++)
        v[s1[i] - 'a']++;

    for (i = 0; i < strlen(s2); i++)
        v[s2[i] - 'a']--;

    for (i = 0; i < 26; i++)
        if (v[i] != 0)
            return 0;

    return 1;
}

double compute_grade(char *s1, char *s2)
{
    if (is_anagram(s1, s2) == 0)
        return -1;

    int num = strlen(s1);
    int i;
    double nr = 0;

    for (i = 0; i < strlen(s1); i++)
    {
        if (s1[i] != s2[i])
            nr = nr + 1.0;
    }

    nr /= num;

    return nr;
}

int main()
{
    struct pereche
    {
        char s1[51], s2[51];
        double grad;
    } v[1001];

    int i, j, n;

    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        scanf("%s%s", v[i].s1, v[i].s2);
        v[i].grad = compute_grade(v[i].s1, v[i].s2);
    }

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
        {
            if ((v[i].grad < v[j].grad) || (v[i].grad == v[j].grad && strcmp(v[i].s1, v[j].s1) == -1) ||
                (v[i].grad == v[j].grad && strcmp(v[i].s1, v[j].s1) == 0 && strcmp(v[i].s2, v[j].s2) == -1))

            {
                char aux[51];
                double auxgrad;

                strcpy(aux, v[i].s1);
                strcpy(v[i].s1, v[j].s1);
                strcpy(v[j].s1, aux);

                strcpy(aux, v[i].s2);
                strcpy(v[i].s2, v[j].s2);
                strcpy(v[j].s2, aux);

                auxgrad = v[i].grad;
                v[i].grad = v[j].grad;
                v[j].grad = auxgrad;
            }
        }

    for (i = 0; i < n; i++)
        printf("%s %s\n", v[i].s1, v[i].s2);

    return 0;
}