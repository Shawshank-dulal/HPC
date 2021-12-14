    #include <stdio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "lodepng.cpp"
    #include <cuda_runtime_api.h>


    __global__ void cudaBlur(unsigned char *imageInput, unsigned char *imageOutput, unsigned int height, unsigned int width)
    {
        int idx = blockDim.x * blockIdx.x + threadIdx.x;
        int pixel = idx * 4;
        float red = 0;
        float green = 0;
        float blue = 0;
        float alpha = 0;
         int count = 1;

        int top=(idx/width)-1;
        int left=(idx%width)-1;
        int right=(idx%width)+1;
        int bottom=(idx/width)+1;

        red += imageInput[pixel];
        green += imageInput[1 + pixel];
        blue += imageInput[2 + pixel];
        alpha += imageInput[3 + pixel];
        count++;

        if(bottom && bottom<=height && left>0 ){
            int currentPos = width * (idx / width + 1) + idx % width - 1;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }
        if(right && right<=width){
            int currentPos = width * (idx / width) + idx % width + 1;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }
        if(bottom && bottom<=height && right<=width){
            int currentPos = width * (idx / width) + idx % width + 1;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }
        if(bottom && bottom<=height){
            int currentPos = width * (idx / width + 1) + idx % width;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }
        if(left && left>0){
            int currentPos = width * (idx / width) + idx % width - 1;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }
        if(top && top>0){
            int currentPos = width * (idx / width - 1) + idx % width;
            int pixel = currentPos * 4;
            red += imageInput[pixel];
            green += imageInput[1 + pixel];
            blue += imageInput[2 + pixel];
            alpha += imageInput[3 + pixel];
            count++;
        }

        int currentIdx = idx * 4;
        imageOutput[currentIdx] = red / count;
        imageOutput[currentIdx + 1] = green / count;
        imageOutput[currentIdx + 2] = blue / count;
        imageOutput[currentIdx + 3] = imageInput[3 + currentIdx];
    }

    int main()
    {
        unsigned int error;
        unsigned int encError;
        unsigned char* image;
        unsigned int width;
        unsigned int height;
        unsigned int *d_height, *d_width;
        const char *filename = "hck.png";
        const char *newFileName = "generated.png";

        error = lodepng_decode32_file(&image, &width, &height, filename);
        if (error)
        {
            printf("error %u: %s\n", error, lodepng_error_text(error));
        }

        const int ARRAY_SIZE = width * height * 4;
        const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

        unsigned char host_imageInput[ARRAY_SIZE * 4];
        unsigned char host_imageOutput[ARRAY_SIZE * 4];

        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            host_imageInput[i] = image[i];
        }

        unsigned char *d_in;
        unsigned char *d_out;

        cudaMalloc((void **)&d_in, ARRAY_BYTES);
        cudaMalloc((void **)&d_out, ARRAY_BYTES);

        cudaMalloc(&d_height, sizeof(height));
        cudaMalloc(&d_width, sizeof(width));

        cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

        cudaMemcpy(d_height, &height, sizeof(unsigned int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_width, &width, sizeof(unsigned int), cudaMemcpyHostToDevice);

        cudaBlur<<<height, width>>>(d_in, d_out, height, width);

        cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);

        encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
        if (encError)
        {
            printf("error %u: %s\n", error, lodepng_error_text(encError));
        }

        cudaFree(d_in);
        cudaFree(d_out);
        printf("completed");
        return 0;
    }