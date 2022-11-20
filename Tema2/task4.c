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
                strcpy(v->content,p);
                no_content=0;
            }
            else
                strcat(v->content,p);     
    }

    fclose(f);
}

int rotr(char x, int k)
{
    int i,aux=0,unu=1;

    for (i=1;i<=k;i++)//masca pentru bitii din coada care vor ajunge in fata
    {
        aux |= unu;
        unu<<=1;
    }
    int part1=aux&(int)x;//filtrez doar acei biti si ii aduc in fata
    part1<<=8-k;

    aux=0;
    unu=1<<k;
    for (i=1;i<=8-k;i++)//masca pentru ceilalti biti
    {
        aux |= unu;
        unu<<=1;
    }
    int part2=aux&(int)x;//aduc bitii in spate
    part2>>=k;

    return part1|part2;
}
int rotl(char x, int k)
{
    int i,aux=0,unu;

    unu=1<<8-k;

    for (i=1;i<=k;i++)//masca pentru bitii din fata care vor ajunge in spate
    {
        aux |= unu;
        unu<<=1;
    }
    int part2=aux&(int)x;
    part2>>=8-k;
     

    unu=1;
    aux=0;
    for (i=1;i<=8-k;i++)//masca pentru ceilalti biti
    {
        aux |= unu;
        unu<<=1;
    }
    int part1=aux&(int)x;
    part1<<=k;

    return part1|part2;
}

int calculare_checksum(site v)
{
    int aux=rotl(v.content[0],0);
    int i;

    for (i=1;i<strlen(v.content);i++)
    {
        if (i%2==1)
           aux^= rotr(v.content[i],i);
        else 
           aux^= rotl(v.content[i],i);
    }

    return aux;
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

    for (i=0;i<file_nr;i++)
    {
        printf("%s %lld %d\n",v[i].url,v[i].checksum,calculare_checksum(v[i]));
    }

    fclose(master);

   // free(v);

    return 0;
}

//3 ore
//nu ia al 4lea fisier