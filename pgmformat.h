#ifndef PGMFORMAT_H_INCLUDED
#define PGMFORMAT_H_INCLUDED

#define MAXLENGTH 256
#define MAXVALUE  255
#define MAXROWS 2000
#define MAXCOLS 2000


#ifndef _PGMTYPE
#define _PGMTYPE
struct PgmType {
    int width;
    int height;
    char* bytes;
};
#endif // _PGMTYPE
int pgmRead (char *fileName, struct PgmType* pgm);

#endif // PGMFORMAT_H_INCLUDED
