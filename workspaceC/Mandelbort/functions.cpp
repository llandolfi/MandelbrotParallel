#include "functions.h"

int free2duchar(unsigned char ***array) {
	/* free the memory - the first element of the array is at the start */
	free(&((*array)[0][0]));

	/* free the pointers into the memory */
	free(*array);

	return 0;
}

int malloc2duchar(unsigned char ***array, int n, int m) {

	/* allocate the n*m contiguous items */
	unsigned char *p = (unsigned char *)malloc(n*m*sizeof(unsigned char));
	if (!p) return -1;

	/* allocate the row pointers into the memory */
	(*array) = (unsigned char **)malloc(n*sizeof(unsigned char*));
	if (!(*array)) {
		free(p);
		return -1;
	}

	/* set up the pointers into the contiguous memory */
	for (int i=0; i<n; i++)
		(*array)[i] = &(p[i*m]);

	return 0;
}

int malloc3duchar(unsigned char ****array, int n, int m, int j){

	(*array) = (unsigned char***)malloc(j*sizeof(unsigned char*));

	for (int i=0;i<j;i++){
		malloc2duchar(&(*array)[i],n,m);
	}

	return 0;
}

double RandomGen (double rangea, double rangeb){

	double offset = (double)rand()/RAND_MAX;

	return rangea+(offset*(rangeb-rangea));

}



