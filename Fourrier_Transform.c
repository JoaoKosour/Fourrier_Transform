#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>


/*
typedef struct {
	double real;
	double imaginario;
	int pos;
}img;
complex.h             double complex            
*/


void DFT(double complex *output, unsigned char *, int );// -- ok
void calc_amplitude(int , double *, double complex *);// -- ok
void ord_greater(double complex *, double complex *, double *, int *, int , int);// -- ok
void inverse_DFT(double complex *, int);// -- ok


int main(void){

	// LEITURA DO ARQUIVO.
	unsigned char *input = calloc(1, sizeof(unsigned char));
  int obs = 0, C = 0, k;
	char *name = calloc(20, sizeof(char));
  scanf("%s", name);
	scanf("%d", &C );
  FILE *f;
  f = fopen (name,"rb");
	free(name);
  if(!f){
	   printf("Error opening file\n");
  }
  while(fread(&input[obs], sizeof(unsigned char), 1, f)){
    obs++;
    input = realloc(input,(obs+1)*sizeof(unsigned char));
  }
	printf("%d\n", obs);
  input = realloc(input, obs * sizeof(unsigned char));
	
	// DISCRETE FOURRIER TRANSFORM.
	double complex *output = calloc(((obs/2)+1), sizeof(double complex));
	DFT(output, input, obs);
  
	// AMPLITUDE.
	double *amplitude = calloc( ((obs/2)+1), sizeof(double));
	calc_amplitude(obs, amplitude, output);
  
  // ORDENAÇÃO.
	double complex *temp = calloc(C, sizeof(double complex));
	int *freq = calloc(C, sizeof(int));
	ord_greater(output, temp, amplitude, freq, C, obs);

	// POSIÇÃO ORIGINAL.
	double complex *newoutput = calloc((obs/2)+1, sizeof(double complex));
  for(k = 0; k < C; k++){
  	newoutput[freq[k]] = temp[k];
	}

	// INVERSE DISCRETE FOURRIER TRANSFORM.
	inverse_DFT(newoutput, obs);

	// FREE.
	free(input);
  free(output);
  free(amplitude);
  free(temp);
	free(newoutput);
  free(freq);

  return 0;
}


void DFT(double complex *output, unsigned char *input, int obs){// -- ok
	int k = 0, t = 0, E;
	double complex aux = 0;

	for(k = 0; k <= obs/2; k++){
		aux = 0;
		for(t = 0; t < obs; t++){
				if(k == 0) E = 1;
				else E = 2;
				aux += input[t] / (cos(2 * M_PI * k * t/obs)  +  sin(2 * M_PI * k * t/obs) * I);
		}
      output[k] = E * aux/obs;
	}
}


void calc_amplitude(int obs, double *amplitude , double complex *coefficients){
	int k, count = 0;

  for(k = 0; k < obs/2 + 1; k++){
    amplitude[k] = sqrt(pow(creal(coefficients[k]), 2)  +  pow(cimag(coefficients[k]), 2));
    if(amplitude[k] > 0.1) count++;
  }
	printf("%d\n", count);
}


void ord_greater(double complex *output, double complex *temp,
								 double *amplitude, int *freq, int C, int obs){
	int k, t;
	double greater = 0;
  for(k = 0; k < C; k++){
		greater = amplitude[k];
    freq[k] = k;
    for(t = 0; t < (obs / 2)+1; t++){
      if(amplitude[t] > greater){
				freq[k] = t;
        greater = amplitude[t];  
      }
    }
		amplitude[freq[k]] = 0;
		temp[k] = output[freq[k]];
    printf("%d ", (int)greater);
  }
	printf("\n");
}


void inverse_DFT(double complex *input, int obs){// -- ok
		int t = 0, k = 0; 
		unsigned char result = 0;
		double complex aux;
		
	for(t = 0; t < obs; t++){
    aux = 0;
    for(k = 0; k < (obs/2) + 1; k++){
      aux += input[k] * (cos(2 * M_PI * k * t/obs)  +  sin(2 * k * M_PI * t/obs) * I);
    }
    result = round(aux);
    printf("%d\n", result);
  }
}