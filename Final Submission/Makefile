all: Portfolio_Task_1 Portfolio_Task_2 Portfolio_Task_3 Portfolio_Task_4

Portfolio_Task_1: Portfolio_Task_1.c
		   gcc Portfolio_Task_1.c -o task1 -pthread

Portfolio_Task_2: Portfolio_Task_2.c
		   gcc Portfolio_Task_2.c -o task2 -pthread -lcrypt
		   
Portfolio_Task_3: Portfolio_Task_3.cu
		   nvcc Portfolio_Task_3.cu -o task3
		   
Portfolio_Task_4: Portfolio_Task_4.cu
		   nvcc Portfolio_Task_4.cu -o task4 -lodepng.h

clean:
	rm -f Portfolio_Task_1 Portfolio_Task_2 Portfolio_Task_3 Portfolio_Task_4
