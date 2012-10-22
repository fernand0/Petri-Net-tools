#include <stdio.h>
#include <stdlib.h>

# This program generates the Petri Net of a Flexible Manufacturing System
# as described in:
# Computing minimal siphons in Petri net models of resource allocation 
# systems: a parallel solution.
# Tricas, F.; Ezpeleta, J.; 
# Systems, Man and Cybernetics, Part A, IEEE Transactions 
# Volume 36, Issue 3, May 2006 Page(s):532 - 539 
# It takes as argument the length and the number of processes and 
# generates the net 
# using the paths of nodes in PTN format.
# There is a third optional parameter to give a name to the Petri Net



int main(int argc, char *argv[])
{
	int i,j,ir,jr, length, numProc;
	char name[30];

	if (argc < 3) {
		fprintf(stderr,"Insufficient arguments. Usage %s <length> <num> [name]\n", argv[0]);
		exit(1);
	}

	length = atoi(argv[1]);
	numProc = atoi(argv[2]);

	if (argc > 3) 
		 sprintf(name, "%s", argv[3]);
	else {
		 sprintf(name, "FMS_%1d", length);
	}
		

	fprintf(stdout,"PTN %s\n", name);
	fprintf(stdout,"{\n");
	fprintf(stdout,"PLACES\n");

	ir=1;

	/*	State Places */
	for (i=1; i<= numProc; i++) {
		 for (j=1; j<= length; j++) {
		 	fprintf(stdout,"P%1d_%1d ", i, j);
		 }
		 fprintf(stdout,"\n");
	}

	/*	Idle Places */
	for (i=1; i<=numProc; i++ )
		 fprintf(stdout, "P%1d_0 ", i);
	fprintf(stdout,"\n");
	
	/*	Resource Places */
	for (i=1; i <= numProc; i++) {
		 for (j=1; j <= length; j++) {
			  fprintf(stdout, "R%1d_%1d ", i,j);
			  fprintf(stdout,"\n");
		 }
	}
	fprintf(stdout,"\n");


	fprintf(stdout,"\n");
	fprintf(stdout,"TRANS\n");

	for (i=1; i<= numProc; i++) {
		 for (j=1; j<= length ; j++) {
			  printf("T_%1d_%1d ", i, j);
		 }
		 fprintf(stdout,"\n");
	}
	for (i=1; i<= numProc; i++) {
		 printf("T_%1d_%1d ", i, 0); 

	}

	fprintf(stdout,"\n");
	fprintf(stdout, "MARKING ");
	fprintf(stdout,"\n");

	for (i=1; i<=numProc; i++ )
		 fprintf(stdout, "%d * P%1d_0 ", 4*length, i);
	fprintf(stdout,"\n");

	for (i=1 ; i<= numProc; i++) {
		 for (j=1; j <= length; j++) {
			  fprintf(stdout, "1*R%1d_%1d ", i,j);
			  fprintf(stdout,"\n");
		 }
	}

	fprintf(stdout,"\n");
	fprintf(stdout,"\n");
	fprintf(stdout,"PATHS\n");
	for (i=1; i<= numProc ; i++) {
		 fprintf(stdout,"T_%1d_%1d->P%1d_0->T_%1d_0->",i, length,i,i);
		 for (j=1; j<= length; j++) {
			  printf("P%1d_%1d->T_%1d_%1d", i,j,i,j);
			  if (j != length) fprintf(stdout, "->");
		 }
		 fprintf(stdout,"\n");
	}


	for (i = 1; i <= numProc; i++) {
		 for (j = 1;  j<= length; j++) {
		 	fprintf(stdout,"T_%1d_%1d->R%1d_%1d->T_%1d_%1d",
					  i, j,
					  i, j, 						/*		The resource */
					  i, j-1);
			fprintf(stdout,"\n");
			if (i < numProc) {
				 ir = i;
			}
			else {
				 ir = 0;
			}
			fprintf(stdout,"T_%1d_%1d->R%1d_%1d->T_%1d_%1d",
					  ir + 1, length + 1 - j,
					  i, j,  				/*    the resource */
					  ir + 1, length + 1 - (j+1));
			fprintf(stdout,"\n");
		 }
	}

	fprintf(stdout,"\n}\n");
}
