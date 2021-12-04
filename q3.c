#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
#include <crypt.h>


//cuda encrypter
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
			if(newPassword[i] > 122){
				newPassword[i] = (newPassword[i] - 122) + 97;
			}else if(newPassword[i] < 97){
				newPassword[i] = (97 - newPassword[i]) + 97;
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
    for(int i=0;i<sizeof(s1);i++){
        if(s1[i]!=s2[i])
            return false;
    }
    return true;

}



//crack kernel
__global__ void CudaCrack(char *character, char *number){
    char generatedPass[4];

    generatedPass[0]=character[blockIdx.x];
    generatedPass[1]=character[blockIdx.y];
    generatedPass[2]=number[threadIdx.x];
    generatedPass[3]=number[threadIdx.y];

    char *enc=CudaCrypt(generatedPass);
    if(cudaCmp(enc,pass))
        printf("%s\n",generatedPass);
}

//main function
int main(int argc,char ** argv){
    char h_Alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char h_Numbers[26] = {'0','1','2','3','4','5','6','7','8','9'};

    char * d_Alphabet;
    cudaMalloc( (void**) &d_Alphabet, sizeof(char) * 26); 
    cudaMemcpy(d_Alphabet, h_Alphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);

    char * d_Numbers;
    cudaMalloc( (void**) &d_Numbers, sizeof(char) * 10); 
    cudaMemcpy(d_Numbers, h_Numbers, sizeof(char) * 10, cudaMemcpyHostToDevice);

    CudaCrack<<<dim3(26,26,1),dim3(10,10,1)>>>(d_Alphabet,d_Numbers);
    cudaDeviceSynchronize();
    return 0;
}