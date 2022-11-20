#include "citire_date.h"
#include <stdio.h>
#include <stdlib.h>

void citire_master(FILE *master, site *v,int *file_nr, int *cap)
{
    char *s;

    while(fgets(s,100,master))
    {
        if (s[strlen(s)-1]=='\n') 
            s[strlen(s)-1]='\0';

        strcpy(v[*file_nr].nume,s);
        file_nr++;

        if (file_nr==*cap) marire_struct(v,cap);
    }
}