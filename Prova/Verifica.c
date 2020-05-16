#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int work_array[] = {};

typedef struct {

   int splitter_value; // valore "separatore"

   int * less_than_array; // array dei valori <= splitter_value

   unsigned int less_than_array_len; // lunghezza di less_than_array

   int * larger_than_array; // array dei valori > splitter_value

   unsigned int larger_than_array_len; // lunghezza di larger_than_array

} split_result;


//Dichiarazioni di funzioni
split_result splitter(int * param_array, unsigned int array_len, int splitter_value);
int minimo(int * array, unsigned int array_len);
int massimo(int * array, unsigned int array_len);



int main() {

	//Lunghezza work_array
	int work_array_length = (int)(sizeof(work_array)/sizeof(int));
	printf("dimensione di work_array: %d\n", work_array_length);

	//Split di work_array
	split_result splitted_array = splitter(work_array, work_array_length, 0);

	//Lunghezza dei due array
	printf("less_than_len: %d, larger_than_len: %d\n", splitted_array.less_than_array_len, splitted_array.larger_than_array_len);

	//Primo processo figlio
	pid_t child_pid = fork();

	if(child_pid == 0) {
		//Sono il processo figlio!
		printf("esecuzione primo processo:\n");
		int min = minimo(splitted_array.less_than_array, splitted_array.less_than_array_len);
		int max = massimo(splitted_array.less_than_array, splitted_array.less_than_array_len);
		printf("min: %d, max: %d\n", min, max);
		exit(EXIT_SUCCESS);
	} else if(child_pid == -1) {
		//Errore nel fork()!
		perror("fork() error!");
		exit(EXIT_FAILURE);
	} else {
		//Sono il processo padre!
		if (wait(NULL) == -1) {
			perror("wait error!");
		}

		//Secondo processo figlio
		child_pid = fork();

		if(child_pid == 0) {
			//Sono il processo figlio!
			printf("esecuzione secondo processo:\n");
			int min = minimo(splitted_array.larger_than_array, splitted_array.larger_than_array_len);
			int max = massimo(splitted_array.larger_than_array, splitted_array.larger_than_array_len);
			printf("min: %d, max: %d\n", min, max);
			exit(EXIT_SUCCESS);
		} else if(child_pid == -1) {
			//Errore nel fork()!
			perror("fork() error!");
			exit(EXIT_FAILURE);
		} else {
			//Sono il processo padre!
			if (wait(NULL) == -1) {
				perror("wait error!");
			}
			printf("bye!\n");
			exit(EXIT_SUCCESS);
		}

	}


	return 0;
}

int minimo(int * array, unsigned int array_len) {
	if(array == NULL || array_len <= 0) {
		perror("Parametri non validi!");
		exit(EXIT_FAILURE);
	}
	int min = array[0];
	for(unsigned int k = 0; k < array_len; k++) {
		if(array[k] < min) {
			min = array[k];
		}
	}
	return min;
}

int massimo(int * array, unsigned int array_len) {
	if(array == NULL || array_len <= 0) {
		perror("Parametri non validi!");
		exit(EXIT_FAILURE);
	}
	int max = array[0];
	for(unsigned int k = 0; k < array_len; k++) {
		if(array[k] > max) {
			max = array[k];
		}
	}
	return max;
}

split_result splitter(int * param_array, unsigned int array_len, int splitter_value) {
	split_result output;
	int counter = 0;
	for(int k = 0; k < array_len; k++) {
		if(param_array[k] > splitter_value) {
			counter++;
		}
	}
	output.splitter_value = splitter_value;
	output.less_than_array_len = array_len - counter;
	output.larger_than_array_len = counter;
	output.less_than_array = malloc(sizeof(int)*(output.less_than_array_len));
	if(output.less_than_array == NULL) {
		perror("malloc error!");
		exit(EXIT_FAILURE);
	}
	output.larger_than_array = malloc(sizeof(int)*(output.larger_than_array_len));
	if(output.larger_than_array == NULL) {
		perror("malloc error!");
		exit(EXIT_FAILURE);
	}
	counter = 0;
	for(int k = 0; k < array_len; k++) {
		if(param_array[k] > splitter_value) {
			output.larger_than_array[counter] = param_array[k];
			counter++;
		} else {
			output.less_than_array[k-counter] = param_array[k];
		}
	}
	return output;
}
