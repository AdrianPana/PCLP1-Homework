#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char url[200], title[200], *content;
    long long nr_acc, checksum;
} site;

void marire_struct(site *v, int *cap)
{
    *cap += 3;
    v = realloc(v, *cap * sizeof(site));
}

void citire(char *s, site *v)
{
    FILE *f;

    f = fopen(s, "r");

    fscanf(f, "%200s", v->url);

    char aux[20], line[200], aux1[100];
    fscanf(f, "%s", aux);
    long long length = atoi(aux);
    fscanf(f, "%s", aux);
    v->nr_acc = atoi(aux);
    fscanf(f, "%s", aux);
    v->checksum = atoi(aux);

    //printf("%s %lld ", v->url, v->nr_acc);

    //check title

    fgets(line, 100, f); // caracterul newline
    fgets(line, 100, f); // <html>
    length -= strlen(line) * sizeof(char);
    fgets(line, 100, f); //<title>
    length -= strlen(line) * sizeof(char);

    if (line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = '\0';
    if (line[strlen(line) - 1] != '>') //daca titlul se intinde pe 2 linii, o citesc si pe urmatoarea
    {
        fgets(aux1, 100, f);
        length -= strlen(aux1) * sizeof(char);
        line[strlen(line) - 1] = '\0';
        strcat(line, aux1);
    }

    char *p;
    int field_nr = 0;
    p = strtok(line, "<>");
    while (p)
    {
        field_nr++;

        if (field_nr == 2)
            strcpy(v->title, p);

        p = strtok(NULL, "<>");
    }

    length -= 7 * sizeof(char); //scad si </html>

    v->content = malloc(length);
    int no_content = 1;

    while (1) //citesc continutul linie cu linie
    {

        fgets(line, 100, f);

        if (strstr(line, "</html>") != 0)
            break;

        int poz;

        if (line[0] == '<')
        {
            poz = 1;
            while (line[poz] != '>')
                poz++;
            strcpy(aux1, line + poz + 1);
            strcpy(line, aux1);
        }

        p = strtok(line, "<");
        if (no_content)
        {
            strcpy(v->content, p);
            no_content = 0;
        }
        else
            strcat(v->content, p);
    }

    fclose(f);
}

int crit2_cmp(site x, site y)
{
    if (strcmp(x.title, y.title) == 0)
    {
        return x.nr_acc < y.nr_acc;
    }
    else if (strcmp(x.title, y.title) == 1)
        return 1;
    else
        return 0;
}

void sortare(site *v, int n, int (*crit_cmp)(site x, site y))
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
            if (crit_cmp(v[i], v[j])) //swap(&v[i],&v[j]);
            {
                site aux;
                aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
    }
}

void parcurgere_query(char keywords[][100], int *key_nr, char *exclus, char secvente[][100], int *nr_secv)
{
    char aux[101];
    int in_secv = 0;

    gets(aux);

    char *p;

    p = strtok(aux, " ");

    while (p)
    {
        if (p[0] == '-')
            strcpy(exclus, p+1);

        else
        {   
            
            if (in_secv)
            {
                if (p[strlen(p) - 1] == '"')
                {
                   
                    in_secv = 0;
                    p[strlen(p) - 1] = '\0';
                    strcat(secvente[*nr_secv], " ");
                    strcat(secvente[*nr_secv], p);
                    *nr_secv=*nr_secv+1;
                    
                }
                else
                {
                    strcat(secvente[*nr_secv], " ");
                    strcat(secvente[*nr_secv], p);
                }
            }

            else if (p[0] == '"')
            {
                in_secv = 1;
                strcpy(secvente[*nr_secv], p + 1);
            }

            else
            {
                strcpy(keywords[*key_nr], p);
                *key_nr += 1;
            }
        }

        p = strtok(NULL, " ");
    }
}

int convine_query(site v, char keywords[][100], int key_nr,char *exclus, char secvente[][100], int nr_secv)
{
    int j;
    char *p;

    int gasit_cuvinte=0;
    int gasit_secvente=0;

    p = strtok(v.content, " \n");

    while (p)//verific cuvinte simple si cuvantul exclus
    {
        for (j = 0; j < key_nr; j++)
            if (strcmp(p, keywords[j]) == 0)
                gasit_cuvinte=1;

        if (strcmp(p,exclus)==0) return 0;

        p = strtok(NULL, " \n");
    }

    for (j=0;j<nr_secv;j++)//verific fiecare secventa
    {
        //mi e sila rn, sa vedem ce zice si juriul
    }

    return gasit_cuvinte*gasit_secvente;
}

int main()
{
    FILE *master;

    master = fopen("master.txt", "r");

    char ch, ch1, s[30], keywords[100][100], aux[101], exclus[101], secvente[100][100];
    int nr = 0, file_nr = -1, i, j, cap = 3, key_nr = 0, f_cap = 3, f_count = 0, nr_secv = 0;

    site *v, *found;

    v = malloc(cap * sizeof(site));
    found = malloc(f_cap * sizeof(site));

    strcpy(exclus,"");
    nr_secv=0;
    parcurgere_query(keywords, &key_nr, exclus, secvente, &nr_secv);

    while ((ch = fgetc(master)) != EOF)
    {
        if (ch == '\n')
        {
            file_nr++;
            s[nr] = '\0';

            if (file_nr > cap)
                marire_struct(v, &cap);

            citire(s, &v[file_nr]);

            nr = 0;
        }
        else
            s[nr++] = ch;
    }
    s[nr] = '\0';
    file_nr++;

    citire(s, &v[file_nr]);

    for (j = 0; j < file_nr; j++)
    {
        if (convine_query(v[j], keywords, key_nr,exclus,secvente,nr_secv))
        {
            strcpy(found[f_count].url, v[j].url);
            found[f_count].nr_acc = v[j].nr_acc;

            found[f_count].content = malloc((strlen(v[j].content) + 1) * sizeof(char));
            strcpy(found[f_count].content, v[j].content);

            f_count++;
        }
    }

  // sortare(found, f_count, crit2_cmp);

  /*  for (i = 0; i < f_count; i++)
        printf("%s\n", found[i].url);*/

    return 0;
}

//3 ore
//nu ia al 4lea fisier
//strtok nu ia ultimul cuvant din fisierul 2