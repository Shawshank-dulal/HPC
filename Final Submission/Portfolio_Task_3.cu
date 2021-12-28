#include <stdio.h>
#include <stdlib.h>
#include<cuda_runtime_api.h>

#define bool int
#define true 1
#define false 0

/* This program runs on CUDA platform to decrypt the encrypted password that we get from the CUDACrypt program
after the CUDA processor finally finds the password for the provided encryption, it returns the password to the main program and
it is printed on the screen from there.
In order to run the program first compile it by command: nvcc Portfolio_Task_3.cu -o <output_filename>
then run it by the command: ./<output_filename <encrypted_password>
you can find some encrypted passwords below

 sample passwords:
E X 5 3 = GCFBUW7371
B Y 2 1 = DBCCVX4053
W X 4 2 = YUXBUW6262
N X 0 0 = PLOBUW2244
X Y 4 3 = ZVYCVX6271
X Y 9 4 = ZVYCVX2780
 */


//__global__ --> GPU function which can be launched by many blocks and threads
//__device__ --> GPU function or variables
//__host__ --> CPU function or variables

__device__ char* CudaCrypt(char* rawPassword){

	char * newPassword = (char *) malloc(sizeof(char) * 11);
 
	newPassword[0] = rawPassword[0] + 2;
	newPassword[1] = rawPassword[0] - 2;
	newPassword[2] = rawPassword[0] + 1;
	newPassword[3] = rawPassword[1] + 3;
	newPassword[4] = rawPassword[1] - 3;
	newPassword[5] = rawPassword[1] - 1;
	newPassword[6] = rawPassword[2] + 2;
	newPassword[7] = rawPassword[2] - 2;
	newPassword[8] = rawPassword[3] + 4;
	newPassword[9] = rawPassword[3] - 4;
	newPassword[10] = '\0';

	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ //checking all lower case letter limits
			if(newPassword[i] > 90){
				newPassword[i] = (newPassword[i] - 90) + 65;
			}else if(newPassword[i] < 65){
				newPassword[i] = (65 - newPassword[i]) + 65;
			}
		}else{ //checking number section
			if(newPassword[i] > 57){
				newPassword[i] = (newPassword[i] - 57) + 48;
			}else if(newPassword[i] < 48){
				newPassword[i] = (48 - newPassword[i]) + 48;
			}
		}
	}
	return newPassword;
}
__device__ bool cudaCmp(char* s1,char* s2){
    for(int i=0;i<10;i++){
        if(s1[i]!=s2[i])
            return false;
    }
    return true;

}

__global__ void crack(char * alphabet, char * numbers, char* encrypted,char * d_crackedPass){
char genRawPass[4];
genRawPass[0] = alphabet[blockIdx.x];
genRawPass[1] = alphabet[blockIdx.y];
genRawPass[2] = numbers[threadIdx.x];
genRawPass[3] = numbers[threadIdx.y];

char* generatedEncryption=CudaCrypt(genRawPass);
//printf("Pass: %s enc:%s\n",genRawPass, generatedEncryption);

if(cudaCmp(encrypted,generatedEncryption)){
	for(int i=0;i<4;i++){
		d_crackedPass[i]=genRawPass[i];
	}
}
// printf("%c %c %c %c = %s\n", genRawPass[0],genRawPass[1],genRawPass[2],genRawPass[3], CudaCrypt(genRawPass));

}

int main(int argc, char ** argv){
char encryptedPassword[10];
strcpy(encryptedPassword,argv[1]);
char cpuAlphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char cpuNumbers[26] = {'0','1','2','3','4','5','6','7','8','9'};
char *d_encryptedPass;
char * gpuAlphabet;
char *d_crackedPass;
char *h_crackedPass=(char *)malloc(sizeof(char)*4); 
cudaMalloc( (void**) &gpuAlphabet, sizeof(char) * 26); 
cudaMalloc((void**) &d_crackedPass, sizeof(char) * 4);
cudaMalloc( (void**) &d_encryptedPass, sizeof(encryptedPassword)); 
cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);
cudaMemcpy(d_encryptedPass, encryptedPassword, sizeof(encryptedPassword), cudaMemcpyHostToDevice);

char * gpuNumbers;
cudaMalloc( (void**) &gpuNumbers, sizeof(char) * 26); 
cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(char) * 26, cudaMemcpyHostToDevice);

crack<<< dim3(26,26,1), dim3(10,10,1) >>>( gpuAlphabet, gpuNumbers,d_encryptedPass,d_crackedPass);
cudaDeviceSynchronize();
cudaMemcpy(h_crackedPass, d_crackedPass, sizeof(char) * 4, cudaMemcpyDeviceToHost);
printf("Password Found: %s\n",h_crackedPass);
return 0;
}
