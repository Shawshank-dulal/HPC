#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "lodepng.cpp"
#include<cuda_runtime_api.h>
// #include<cuda_runtime_api.h>
/*This Code uses gaussian blurring technique to blurr the image. In order to compile the code, 
run "gcc task4.c -o <output filename> -pthread lodepng.c"
and to run the code, run "./<output filename>"*/
unsigned int error, encError;
unsigned char *imageInput, *imageOutput;
unsigned int width;
unsigned int height;

__global__  void cudaBlur(unsigned char *imageInput)
{
    float t_r = 0, t_g = 0, t_b = 0, t_a = 0, counter = 1;
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int pixel = idx*4;
    t_r += imageInput[pixel];
    t_g += imageInput[1 + pixel];
    t_b += imageInput[2 + pixel];
    t_a += imageInput[3 + pixel];
    counter++;
    if (i + 1 && j - 1)
    {
        int pos = pixel;
        t_r += imageInput[pixel];
        t_g += imageInput[1 + pixel];
        t_b += imageInput[2 + pixel];
        t_a += imageInput[3 + pixel];
        counter++;
    }
    if (j + 1)
    {
        t_r += imageInput[pixel];
        t_g += imageInput[1 + pixel];
        t_b += imageInput[2 + pixel];
        t_a += imageInput[3 + pixel];
        counter++;
    }
    if (i + 1 && j + 1)
    {

        t_r += imageInput[pixel];index
        counter++;
    }
    if (i + 1)
    {

        t_r += imageInput[pixel];
        t_g += imageInput[1 + pixel];
        t_b += imageInput[2 + pixel];
        t_a += imageInput[3 + pixel];
        counter++;
    }
    if (j - 1)
    {

        t_r += imageInput[pixel];
        t_g += imageInput[1 + pixel];
        t_b += imageInput[2 + pixel];
        t_a += imageInput[3 + pixel];
        counter++;
    }
    if (i - 1)
    {

        t_r += imageInput[pixel];
        t_g += imageInput[1 + pixel];
        t_b += imageInput[2 + pixel];
        t_a += imageInput[3 + pixel];
        counter++;
    }
    int current_pixel = idx * 4;
    imageOutput[current_pixel] = t_r / counter;
    imageOutput[current_pixel + 1] = t_g / counter;
    imageOutput[current_pixel + 2] = t_b / counter;
    imageOutput[current_pixel + 3] = imageInput[3 + current_pixel];
}


void *blurring(void *p)
{
    unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	const char* filename = "hck.png";
	const char* newFileName = "generated.png";

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error){
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	const int ARRAY_SIZE = width*height*4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++) {
		host_imageInput[i] = image[i];
	}

	// declare GPU memory pointers
	unsigned char * d_in;
	unsigned char * d_out;

	// allocate GPU memory
	cudaMalloc((void**) &d_in, ARRAY_BYTES);
	cudaMalloc((void**) &d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel
	cudaBlur<<<height, width>>>(d_out, d_in);

	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);
	
	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}

	//free(image);
	//free(host_imageInput);
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}