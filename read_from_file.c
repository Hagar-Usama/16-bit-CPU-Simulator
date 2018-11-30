#include <stdio.h>
#include <stdlib.h>

int read_from_file(char* filename , int* array , int size);

int main(){
	
	FILE *myFile;
    myFile = fopen("program.txt", "r");

    //read file into array
    int numberArray[16];
    int i;

    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    for (i = 0; i < 16; i++){
        fscanf(myFile, "%x\n", &numberArray[i] );
    }

    for (i = 0; i < 16; i++){
        printf("Number is: %i\n\n", numberArray[i]);
    }

    fclose(myFile);

int array[5];
read_from_file("program.txt" , array , 5);	
	
	
for (i = 0; i < 5; i++){
        printf("Num is: %i\n\n", array[i]);
    }	
	
	
	
return 0;	
	
}

int read_from_file(char* filename , int* array , int size){
	
	
	
	FILE *fp;
    fp = fopen(filename, "r");

    int i;

    if (fp == NULL){
        printf("File not found\n");
        exit (0);
    }

    for (i = 0; i < size; i++){
        fscanf(fp, "%i\n", &array[i] );
    }


    fclose(fp);
	return 1;
	
	
	}
