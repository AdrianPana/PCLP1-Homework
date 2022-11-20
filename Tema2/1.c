#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char url[50], title[50], *content;
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
    length -= strlen(line);
    fgets(line, 100, f); //<title>
    length -= strlen(line);

    if (line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = '\0';
    if (line[strlen(line) - 1] != '>') //daca titlul se intinde pe 2 linii, o citesc si pe urmatoarea
    {
        fgets(aux1, 100, f);
        length -= strlen(aux1);
        line[strlen(line) - 1] = '\0';
        strcat(line, aux1);
    }

    char *p;
    int field_nr = 0,cap;
    p = strtok(line, "<>");
    while (p)
    {
        field_nr++;

        if (field_nr == 2)
            strcpy(v->title, p);

        p = strtok(NULL, "<>");
    }

    length -= 7; //scad si </html>

    //v->content = malloc(length*sizeof(char));
    int no_content=1;

    while (1) //citesc continutul linie cu linie
    {

        fgets(line, 100, f);

        if (strstr(line, "</html>") != 0)
            break;

        int poz;

        if (line[0] == '<')
        {
            poz=1;
            while(line[poz]!='>')
                poz++;
            strcpy(aux1, line + poz + 1);
            strcpy(line, aux1);
        }

        p=strtok(line,"<");
        if (no_content)
            {
                cap = strlen(p);
                v->content = malloc(cap*sizeof(char));
                strcpy(v->content,p);
                no_content=0;
            }
            else
            {
                cap+=strlen(p);
                v->content = realloc(v->content,cap*sizeof(char));
                strcat(v->content,p);
            }   

        v->content[strlen(v->content)-1]='\0';
    }

    fclose(f);
}

int main()
{
    FILE *master;

    master = fopen("master.txt", "r");

    char ch, ch1, s[30];
    int nr = 0, file_nr = 0, i, j, cap = 3;

    site *v;

    v = malloc(cap * sizeof(site));

    while ((ch = fgetc(master)) != EOF)
    {
        if (ch == '\n')
        {
            s[nr] = '\0';

            if (file_nr+1 > cap)
                marire_struct(v, &cap);

            citire(s, &v[file_nr]);

            file_nr++;
            nr = 0;
        }
        else
            s[nr++] = ch;
    }
    s[nr] = '\0';
    if (file_nr+1 > cap)
        marire_struct(v, &cap);
    citire(s, &v[file_nr]);
    file_nr++;

    for (i = 0; i < file_nr; i++)
    {
        printf("%s %lld %s\n", v[i].url, v[i].nr_acc, v[i].title);
    }

    fclose(master);

    free(v);

    return 0;
}

//4 ore