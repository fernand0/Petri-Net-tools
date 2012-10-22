#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int i,j,ir,jr, numPhil;
	char name[30];
	

	if (argc < 2) {
		fprintf(stderr,"Insufficient arguments. Usage %s <num> [name]\n", argv[0]);
		exit(1);
	}

	numPhil = atoi(argv[1]);

	if (numPhil < 2) {
		fprintf(stderr,"The number of philosophers must be at least 2\n");
		exit(1);
	}


	if (argc > 2) 
		 sprintf(name, "%s", argv[2]);
	else {
		 sprintf(name, "Phil_%1d", numPhil);
	}

	fprintf(stdout,"PTN %s\n", name);
	fprintf(stdout,"{\n");

	fprintf(stdout,"PLACES\n");
	for (i=1; i <= numPhil ; i++) {
		 printf("P_%1d_0 PI_%1d PD_%1d PC_%1d RT_%1d \n", i, i, i, i, i);
	}
	fprintf(stdout,"\n");

	fprintf(stdout,"TRANS\n");

	for (i=1; i <= numPhil ; i++) {
		 printf("TC_%1d TI_%1d TD_%1d TIC_%1d TDC_%1d \n", i, i, i, i, i);
	}
	fprintf(stdout,"\n");

	fprintf(stdout,"MARKING ");
	for (i=1; i <= numPhil ; i++) {
		 fprintf(stdout, "1*P_%1d_0 1*RT_%1d ", i, i);
	}
	fprintf(stdout,"\n");
	fprintf(stdout,"\n");

	fprintf(stdout,"PATHS\n");

	for (i=1; i <= numPhil ; i++) {
		 printf("TC_%1d->P_%1d_0->TD_%1d->PD_%1d->TDC_%1d->PC_%1d->TC_%1d\n",
					i,i,i,i,i,i,i);
		 printf("P_%1d_0->TI_%1d->PI_%1d->TIC_%1d->PC_%1d \n", i, i, i, i, i);

		 printf("TC_%1d->RT_%1d->TI_%1d \n", i, i, i); 
		 printf("RT_%1d->TDC_%1d \n", i, i); 

		 if (i > 1) 
			  j = i - 1;
		 else
			  j = numPhil;
		 printf("TC_%1d->RT_%1d->TD_%1d \n", i, j, i); 
		 printf("RT_%1d->TIC_%1d \n", j, i); 
	}

	fprintf(stdout,"\n}\n");
}
