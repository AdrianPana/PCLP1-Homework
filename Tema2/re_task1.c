#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char url[50],title[50], *content,color[10],bgcolor[10];
    int nr_acc, checksum,size;
} site;

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
        fgets(line,100,f);//<html>
        fgets(line,100,f);//titlu

        char *p;
        int field_nr=0;

        //scot titlul dintre taguri
        p=strtok(line,"<>"); 
        while(p)
        {
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

    for (i=0;i<file_nr;i++)
    printf("%s %d %s\n",v[i].url,v[i].nr_acc,v[i].title);

    for (i=0;i<file_nr;i++)
    free(v[i].content);

    free(v);
    
    return 0;
}