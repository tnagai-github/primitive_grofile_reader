/*
 * Very simple reader of gro file with C language.
 * Copyright (c) 2021, Tetsuro Nagai
 */


#include <stdio.h>
#include <stdlib.h>
#define BUFMAX 1000
// Increase the NATOM_MAX for larger systems. 
// To be more efficient, arrays should be allocated dynamically.
#define NATOM_MAX 100000


int main (argc, argv)
     int argc;
     char *argv[];
{

  //char filename[]="benzene.gro" ;
  char* filename ;
  if(argc ==2){
    filename=argv[1];
  }
  else{
    fprintf(stderr, "I need exactly one argument for file name.\n");
    fprintf(stderr, "Useage: \n");
    fprintf(stderr, "this_command finename.gro\n");
    fprintf(stderr, "I will quit!\n");
    return EXIT_FAILURE;
  }
  FILE *fp;
  char buf[BUFMAX];
  
  double x[NATOM_MAX];
  double y[NATOM_MAX];
  double z[NATOM_MAX];
  int resid[NATOM_MAX];
  int atomid[NATOM_MAX];
  char title[BUFMAX];
  char resname[NATOM_MAX][6];
  char atomname[NATOM_MAX][6];
  double cellx,celly,cellz;
  

  fp = fopen(filename, "r");
  
  if (fp == NULL){
    fprintf(stderr, "Failed to open file %s\n", filename);
    fprintf(stderr, "I will quit\n");
    return EXIT_FAILURE;
  }
  
  //first line is comment
  fgets(buf, BUFMAX, fp);
  sscanf(buf, "%s",title);
  //second line is # of atoms
  fgets(buf, BUFMAX, fp);
  const int NATOM=atoi(buf);
  
  if(NATOM>NATOM_MAX){
    fprintf(stderr, "Too many atoms in %s\n", filename);
    fprintf(stderr, "Please recompile me with large NATOM_MAX\n");
    fprintf(stderr, "I will quit!\n");
    return EXIT_FAILURE;
  }
  
  
  char dummy[6];
  char dummy_coord[3][9];
  
  
  for(int i=0;i<NATOM; i++){
    fgets(buf, BUFMAX, fp);
    
    // first resid
    for(int il=0; il<5; il++){
      dummy[il]=buf[il];
    }
    dummy[5]='\0';
    resid[i]=atoi(dummy);
  
    // second resname
    for(int il=0; il<5; il++){
      dummy[il]=buf[il+5];
    }
    dummy[5]='\0';
    sscanf(dummy, "%s",resname[i]);
  
    // third atomname
    for(int il=0; il<5; il++){
      dummy[il]=buf[il+10];
    }
    dummy[5]='\0';
    sscanf(dummy, "%s",atomname[i]);
  
    // fourth atomnid
    for(int il=0; il<5; il++){
      dummy[il]=buf[il+15];
    }
    dummy[5]='\0';
    atomid[i]=atoi(dummy);
  
    // finally coordiante; velocities are ommited here. But the same trick should work!
    for(int il=0; il<8; il++){
      dummy_coord[0][il]=buf[20+il];
      dummy_coord[1][il]=buf[28+il];
      dummy_coord[2][il]=buf[36+il];
    }
    dummy_coord[0][8]='\0';
    dummy_coord[1][8]='\0';
    dummy_coord[2][8]='\0';
    x[i]=atof(dummy_coord[0]);
    y[i]=atof(dummy_coord[1]);
    z[i]=atof(dummy_coord[2]);
  }
  
  // last line is cell
  fgets(buf, BUFMAX, fp);
  sscanf(buf, "%lf %lf %lf", &cellx, &celly, &cellz);
  
  printf("check\n");
  printf("%s\n", title);
  printf(" %d\n", NATOM);
  for(int i=0; i<NATOM; i++){
    printf("%5d %-5s %5s %5d %8.3f %8.3f %8.3f\n", resid[i], resname[i], atomname[i], atomid[i], x[i], y[i], z[i]);
  }
  printf("%10f %10f %10f\n", cellx, celly, cellz);
  
  return EXIT_SUCCESS;
}

