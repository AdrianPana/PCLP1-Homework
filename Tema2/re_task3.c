#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char url[50],title[50], *content;
    int nr_acc, checksum,size;
} site;

int crit3_cmp (site x, site y)
{
    return x.nr_acc<y.nr_acc;
}

void sortare(site *v, int n,int (*crit_cmp) (site x, site y))
{
    int i,j;
    for (i=0;i<n-1;i++)
    {
        for (j=i+1;j<n;j++)
        if (crit_cmp(v[i],v[j])) 
        {
            site aux;
            aux=v[i];
            v[i]=v[j];
            v[j]=aux;
        }
    }
}

int main()
{
    FILE *master;
    int i,j,file_nr=0,cap,f_cap = 3, f_count = 0,key_nr=0,exista_keyw=0,exista_ex=0,exista_secv=0;
    char s[100],keywords[100][100],exclus[101],secventa[101];

    cap=3;
    site *v = malloc (cap * sizeof(site));
    site *found = malloc (f_cap * sizeof(site));

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
        fgets(line,100,f);//<html>
        fgets(line,100,f);//titlu

        char *p;
        int field_nr=0;

        //scot titlul dintre taguri
        p=strtok(line,"<>"); 
        while(p)
        {   p[strlen(p)]='\0';
            field_nr++;

            if (field_nr==2)
            {
                strcpy(v[file_nr].title,p);
                break;
            }
            p=strtok(NULL,"<>");
        }

        //citesc continutul linie cu linie
        int no_content_added=1;

        while(fgets(line,100,f))
        {
            if (strstr(line,"</html>")) break;

            int poz=0,content_cap;
            char *str;

            if (line[0]=='<') //paragraful incepe pe aceasta linie
            {
                poz=1;//ma situez pe "p" din tag
                while (line[poz]!='>')
                    poz++;//parcurg linia pana la finalul tagului
                
                strcpy(aux1,line+poz+1);
                strcpy(line,aux1);//scot tagul din linie
            }

            for (j=0;j<strlen(line);j++)
            {
                if (line[j]=='<') {
                    line[j]='\0'; break;
                }
            }

            if(no_content_added)
            {
                no_content_added=0;
                content_cap=strlen(line)+1;
                v[file_nr].content=malloc(content_cap*sizeof(char));
                strcpy(v[file_nr].content,line);
            }
            else
            {
                content_cap+=strlen(line)+1;
                v[file_nr].content=realloc(v[file_nr].content,content_cap*sizeof(char));
                strcat(v[file_nr].content,line);

            }

            v[file_nr].content[strlen(v[file_nr].content)]='\0';

        }

        fclose(f);

        file_nr++;
    }
        fclose(master);

    //citire cuvinte de la tastatura si adaugare in dictionarul keywords
    char aux[201],*p;
    int in_secv=0;

    fgets(aux,100,stdin);
    if (aux[strlen(aux)-1]=='\n') aux[strlen(aux)-1]='\0';

    p=strtok(aux," ");
    while(p)
    {   
        if (p[0] == '-')
            {strcpy(exclus, p+1); exista_ex=1;}

        else
        {
            if(in_secv)
            {
                if (p[strlen(p) - 1] == '"')
                {
                   
                    in_secv = 0;
                    p[strlen(p) - 1] = '\0';
                    strcat(secventa, " ");
                    strcat(secventa, p);
                    
                }
                else
                {
                    strcat(secventa, " ");
                    strcat(secventa, p);
                }
            }
            else if (p[0] == '"')
            {
                in_secv = 1;
                strcpy(secventa, p + 1);
                exista_secv=1;
            }
            else
                {strcpy(keywords[key_nr++],p);exista_keyw=1;}
        }

        p=strtok(NULL," ");
    }
    keywords[key_nr-1][strlen(keywords[key_nr-1])]='\0';
    exclus[strlen(exclus)]='\0';
    secventa[strlen(secventa)]='\0';

    //parcurg fiecare site si caut pe rand exclus, cuvintele din keywords si secventa

    for (i=0;i<file_nr;i++)
    {
        int ok=0,poz;

        //verific keywords
        if (exista_keyw)
        for (j=0;j<key_nr;j++)
        {
            
            p=strstr(v[i].content,keywords[j]);
            while(p)
            {
                poz=p-v[i].content+1;
                //printf("%s\n\n",p);

                if ((p-v[i].content == 0 || !isalpha(v[i].content[p-v[i].content-1]))&&
            (strlen(p)==strlen(keywords[j]) || !isalpha(p[strlen(keywords[j])])))

                {ok=1;
                break;}
                
                p=strstr(v[i].content+poz,keywords[j]);
                
            }
        }

        //verific secventa
        if (exista_secv)
        {p=strstr(v[i].content,secventa);
            while(p)
            {
                poz=p-v[i].content+1;
                //printf("%s\n\n",p);

                if ((p-v[i].content == 0 || !isalpha(v[i].content[p-v[i].content-1]))&&
            (strlen(p)==strlen(secventa) || !isalpha(p[strlen(secventa)])))

                {ok=1;
                break;}
                
                p=strstr(v[i].content+poz,secventa);
                
            }
        }
        //verific exclus
        if (exista_ex)
        {p=strstr(v[i].content,exclus);
            while(p)
            {
                poz=p-v[i].content+1;
                

                if ((p-v[i].content == 0 || !isalpha(v[i].content[p-v[i].content-1]))&&
            (strlen(p)==strlen(exclus) || !isalpha(p[strlen(exclus)])))

                {ok=0;
                break;}
                
                p=strstr(v[i].content+poz,exclus);
                
            }
        }
        if (ok) 
        {
            found[f_count]=v[i];
            f_count++;
            if (f_count==f_cap)
            {
                f_cap+=3;
                found = realloc(found,f_cap*sizeof(site));
            }
        }
    }

    //sortare site-uri gasite
    sortare(found,f_count,crit3_cmp);

    for (i=0;i<f_count;i++)
    printf("%s\n",found[i].url);

  //  for (i=0;i<cap;i++)
  //  free(v[i].content);
  //  free(v);
  //  for (i=0;i<f_cap;i++)
  //  free(found[i].content);
  //  free(found);
    
    return 0;
}