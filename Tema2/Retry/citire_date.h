#ifndef CITIRE
#define CITIRE

typedef struct
{
    char nume[100],url[50], title[50], *content;
    long long nr_acc, checksum;
} site;

void citire_master(FILE *master, site *v,int *file_nr, int *cap);

#endif