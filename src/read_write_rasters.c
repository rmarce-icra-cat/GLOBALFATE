#include "lib.h"

int load_names(char **names, char *dir, char *in_ras){
     char name[200], dir2[100], *token;
     int i, j, l=0, k;//l must be set by length(dir)
     
     
     strcpy(dir2, dir);
     strcat(dir2, "data/");
     strcpy(name, dir2);
     //l=strlen(dir2);
     while (1) {
          if (dir2[l] == '\0') break;
          l++;
     }
     
     strcat(name, in_ras);
     FILE *file=fopen(name, "r");
     if(file==NULL) {printf("ERROR in file input rasters\n\n");
                    return 0;}
     for(i=0; i<15; i++){
          if (!fgets(name, sizeof(name), file))
               break;
          strcpy(names[i], dir2);
          
          j=0;
          while (1) {
               if (name[j] == '\n' || name[j]==' ') break;
               names[i][j+l] = name[j];
               j++;
          }
          names[i][j+l]='\0';
     }
     fclose(file);
     
     return 1;
}

double** read_raster(char *name, double *ref_X){
     int nrows, ncols, i, j;
     FILE *file;
     file=fopen(name, "r");
     if (!file)
        perror("fopen");
     
     if (file == NULL){
        printf("ERROR FILE %s\n", name);
        perror(name);
        exit(EXIT_FAILURE);
     }

     //Read header:
     int head=6;
     for(i=0; i<head; i++)
              ref_X[i] = read_head(file);

     //[ncols; nrows; xllcorner; yllcorner; cellsize; NODATA_value];
     ncols=ref_X[0];
     nrows=ref_X[1];

     double **X=(double**)malloc(nrows*sizeof(double*));
     for(i=0;i<nrows;++i)
     X[i]=(double*)malloc(ncols*sizeof(double));

     for(i = 0; i < nrows; i++){
      for(j = 0; j < ncols; j++){
      if (!fscanf(file, "%lf", &X[i][j]))
           break;
      //if(abs(X[i][j])<1e-300) X[i][j]=0;
      }
     }

  fclose(file);
  return X;
}

double read_head(FILE *file){
     char line[256];
     double my_val, a=1.0;
     if(!fgets(line, sizeof(line), file))
          return -1;

     char *q = line;
     while (*q) { // While there are more characters to process...
           if (isdigit(*q)) { // Upon finding a digit, ...
              double val = strtod(q, &q); // Read a number, ...
              //printf("in read %g\n", val); // and print it.
              my_val=val;
           }
           else { // Otherwise, move on to the next character.
                q++;
           }
           if((*q)==45) a=-1.0;
     }
     return my_val*a;
}

void write_raster(char *name, double *ref_X, double **X){
     int nrows, ncols, i, j;
     FILE *file;
     file = fopen(name, "w+");
     if (file == NULL){
        printf("ERROR FILE %s\n", name);
        exit(EXIT_FAILURE);
     }

     //Write header:
     fprintf(file, "ncols\t%f\n", ref_X[0]);
     fprintf(file, "nrows\t%f\n", ref_X[1]);
     fprintf(file, "xllcorner\t%f\n", ref_X[2]);
     fprintf(file, "yllcorner\t%f\n", ref_X[3]);
     fprintf(file, "cellsize\t%f\n", ref_X[4]);
     fprintf(file, "NODATA_value\t%f\n", ref_X[5]);

     //[ncols; nrows; xllcorner; yllcorner; cellsize; NODATA_value];
     ncols=ref_X[0];
     nrows=ref_X[1];

     for(i = 0; i < nrows; i++){
      for(j = 0; j < ncols; j++)
      {  //Use lf format specifier, %c is for character
       if (!fprintf(file, "%lf\t", X[i][j]))
           break;
      }if(j<(ncols-1)) printf("ERROR writing j=%d i=%d\n", j, i);
      fprintf(file, "\n");
     }if(i<(nrows-1)) printf("ERROR writing i=%d\n", i);

  fclose(file);
}

void read_parameters(char *name, double *parameters, int *I){
     int i=0;
     double new_val;
     FILE *file=fopen(name, "r");
     if(file==NULL){
          printf("ERROR FILE %s\n", name);
        exit(EXIT_FAILURE);
     }

     while((new_val=read_head(file))!=-1){
          //printf("i=%d %f\n", i, new_val);
          parameters[i]=new_val;
          i++;
          if(i>12){
               printf("Too many values in %s\n Stop reading\n\n", name);
               break;
          }
     }
     fclose(file);
     *I=i;
}

void write_vector(char *name, double *X, int nrows){
     int i;
     FILE *file;
     file = fopen(name, "w+");
     if (file == NULL){
        printf("ERROR FILE %s\n", name);
        exit(EXIT_FAILURE);
     }

     for(i = 0; i < nrows; i++){
       if (!fprintf(file, "%.20g\n", X[i]))
           break;
      }

  fclose(file);
}
