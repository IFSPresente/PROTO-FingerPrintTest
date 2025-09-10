/* This code was written by Richard Zanibbi in May 1998
 * based on code by Donald Quon and Hai Pham,
 * and Dr. R. T. Whitaker of the University of Tennesee
 * for use in Queen's University's CISC 859 course.
 *
 * These routines read PGM bitmaps (types P2 and P5)
 * and write out PGM files in binary (P5) format.
 * Note that lines in PGM files should be no longer than 70
 * characters long.
 *
 * PGM files have a maximum value of 255 for each pixel (8 bit greyscale)
 *
 * NOTE:
 * Width and height paramaters must appear on the same line separated by
 * a space in column size - number of rows order.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pgmformat.h"

/* INPUT: a filename (char*),row and column dimension variables (long), and
 *   a pointer to a 2D array of unsigned char's of size MAXROWS x MAXCOLS
 *   (row major).
 * OUTPUT: an integer is returned indicating whether or not the
 *   file was read into memory (in row major order).  1 is returned if the
 *   file is read correctly, 0 if it is not.  If there are
 *   too few pixels, the function still returns 1, but returns an error
 *   message.  Error messages are also returned if a file cannot be open,
 *   or if the specifications of the file are invalid.
 * NOTE: The case where too many pixels are in a file is not detected.
 */
int pgmRead (char *fileName, struct PgmType* pgm) {
      FILE *filePointer;    /* for file buffer */
      char line[MAXLENGTH]; /* for character input from file */
      int maximumValue = 0; /* max value from header */
      int binary;           /* flag to indicate if file is binary (P5)*/
      long numberRead = 0;  /* counter for number of pixels read */


      /* Open the file, return an error if necessary. */
      if ((filePointer = fopen(fileName,"r")) == NULL) {
	   printf ("ERROR: cannot open file\n\n");
	   fclose (filePointer);
	   return (0);
      }

      /* Initialize columnsidth, and height */
      pgm->height = 0;
      pgm->width  = 0;

      /* Check the file signature ("Magic Numbers" P2 and P5); skip comments
       * and blank lines (CR with no spaces before it).*/
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') fgets (line,MAXLENGTH,filePointer);
      if (line[0]=='P' && (line[1]=='5')) {
	   binary = 1;
	   sscanf (line+2,"%d %d %d",&(pgm->width),&(pgm->height), &maximumValue);
	  /*  printf ("\nFORMAT: P5\n"); */
      }
      else {
	   printf ("ERROR: incorrect file format\n\n");
	   fclose (filePointer);
	   return (0);
      }

      if ((pgm->width)<1 ||(pgm->height)<1 || maximumValue<0 || maximumValue>MAXVALUE){
	   printf ("ERROR: invalid file specifications (cols/rows/max value)\n\n");
	   printf ("%d %d %d\n", pgm->width, pgm->height, maximumValue);
	   fclose (filePointer);
	   return (0);
      }
      else if ((pgm->width) > MAXCOLS || (pgm->height) > MAXROWS) {
	   printf ("ERROR: increase MAXROWS/MAXCOLS in PGM.h");
	   fclose (filePointer);
	   return (0);
      }

      int bufferSize = pgm->width * pgm->height;
      pgm->bytes = malloc(bufferSize);
      numberRead = fread(pgm->bytes, 1, bufferSize, filePointer);

      if (numberRead < bufferSize) {
	   printf ("ERROR: fewer pixels than rows*cols indicates\n\n");
	   printf ("Leu %d\n", numberRead);
	   return 0;
      }

      /* close the file and return 1 indicating success */
      fclose (filePointer);
      return (1);
}

/* INPUT: a filename (char*), the dimensions of the pixmap (rows,cols of
 *   type long), and a pointer to a 2D array (MAXROWS x MAXCOLS) in row
 *   major order.
 * OUTPUT: an integer is returned indicating if the desired file was written
 *   (in P5 PGM format (binary)).  A 1 is returned if the write was completed
 *   and 0 if it was not.  An error message is returned if the file is not
 *   properly opened.
 */
int pgmWrite(char* filename, long rows,long cols,
	     unsigned char image[MAXROWS][MAXCOLS],char* comment_string) {
      FILE* file;        /* pointer to the file buffer */
      int maxval;        /* maximum value in the image array */
      long nwritten = 0; /* counter for the number of pixels written */
      long i,j;          /* for loop counters */

      /* return 0 if the dimensions are larger than the image array. */
      if (rows > MAXROWS || cols > MAXCOLS) {
           printf ("ERROR: row/col specifications larger than image array:\n");
	   return (0);
      }

      /* open the file; write header and comments specified by the user. */
      if ((file = fopen(filename, "w")) == NULL)	{
           printf("ERROR: file open failed\n");
	   return(0);
      }
      fprintf(file,"P5\n");

      if (comment_string != NULL) fprintf(file,"# %s \n", comment_string);

      /* write the dimensions of the image */
      fprintf(file,"%ld %ld \n", cols, rows);

      /* NOTE: MAXIMUM VALUE IS WHITE; COLOURS ARE SCALED FROM 0 - */
      /* MAXVALUE IN A .PGM FILE. */

      /* WRITE MAXIMUM VALUE TO FILE */
      fprintf(file, "%d\n", (int)255);

      /* Write data */

      for (i=0; i < rows; i++) {
          nwritten += fwrite((void*)&(image[i][0]),sizeof(unsigned char),
	  		   cols, file);
      }


  /*     printf ("\nNumber of pixels total(from rows*cols): %ld\n",rows*cols); */
/*       printf ("Number of pixels written: %ld\n\n",nwritten); */

      fclose(file);
      return(1);
}
