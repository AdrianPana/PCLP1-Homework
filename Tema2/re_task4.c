#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char url[50],title[50], *content;
    int nr_acc, checksum,size;
} site;

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

int rotr(char x,int k)
{
    int cod=x,i,j=0;
    char s[9],aux[9];
    k%=8;

    dec_to_bin(cod,s);
    for (i=8-k;i<8;i++)
    {
        aux[j]=s[i];
        j++;
    }
    for (i=0;i<8-k;i++)
    {
        aux[j]=s[i];
        j++;
    }
    aux[j]='\0';

    return bin_to_dec(aux);
}

int rotl(char x,int k)
{
    int cod=x,i,j=0;
    char s[9],aux[9];
    k%=8;

    dec_to_bin(cod,s);
    for (i=k;i<8;i++)
    {
        aux[j]=s[i];
        j++;
    }
    for (i=0;i<k;i++)
    {
        aux[j]=s[i];
        j++;
    }
    aux[j]='\0';

    return bin_to_dec(aux);
}


int calculare_checksum(site v)
{
    int aux=v.content[0];
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
    int i,j,file_nr=0,cap;
    char s[100];

    cap=3;
    site *v = malloc (cap * sizeof(site));

    master = fopen("master.txt","r");
    if (master==NULL) 
        {
            printf("Nu s-a putut deschide master.txt");
            return 0;
        }

    while(fgets(s,31,master))
    {
        if (s[strlen(s)-1]=='\n') 
            s[strlen(s)-1]='\0';

        if (file_nr==cap) 
        {
            cap+=3;
            v = realloc(v,cap*sizeof(site));
        }

        FILE *f=fopen(s, "r");

         fscanf(f, "%s%d%d%d", v[file_nr].url, &v[file_nr].size,&v[file_nr].nr_acc, &v[file_nr].checksum);

        char aux[20], line[200], aux1[100],ch;

        ch=fgetc(f);//newline
        
        v[file_nr].content=calloc(v[file_nr].size,sizeof(char));
        //citesc continutul linie cu linie
        

        while(fgets(line,100,f))
        {
            strcat(v[file_nr].content,line);

            v[file_nr].content[strlen(v[file_nr].content)]='\0';

            if (strstr(line,"</html>")) break;
        }

        //printf("%s\n\n",v[file_nr].content);

        fclose(f);

        file_nr++;
    }
        fclose(master);


    while (fgets(s,100,stdin))
    {
        if (s[strlen(s)-1]=='\n')
            s[strlen(s)-1]='\0'; 

        int gasit=0;
        for (i=0;i<file_nr;i++)
        {
            
            if (strcmp(s,v[i].url)==0)
            {   
                gasit=1;
                if(v[i].checksum==calculare_checksum(v[i])) printf("Website safe!\n");
                else printf("Malicious website! Official key: %d. Found key: %d\n",v[i].checksum,calculare_checksum(v[i]));
            }
            
        }
        if (!gasit) printf("Website not found!\n");
        
    }

    
    //printf("%s %d %d\n",v[i].url,v[i].checksum,calculare_checksum(v[i]));
    //printf("%d",109<<1);

    free(v);
    
    return 0;
}