/** Title: Threats Assignment
 * Author: Wendy Arenas Rosas
 * CSC 3350, Andy Cameron
 * Date: 2025-05-26
 * 
 * Description: 
 * */
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

void random_number() {

    srand(time(NULL));

    // Create a file named "Data.txt"
    FILE *file = fopen("Data.txt", "w");
    
    // In Data.txt, write 10 random numbers between 0 and 100
    for (int i = 0; i < 1000000; i++) {
        int num = rand() % 101;
        fprintf(file, "%d\n", num);
    }

    // Close the file
    fclose(file);    
}

typedef struct {
    char label;
    double *time;
    const char* filename;
}thread_data;

void *thread_func_a(void *arg) {

    thread_data* t = (thread_data*)arg;

    clock_t start, end;

    long long sum = 0;
    int count = 0;
    int num;

    // Start the clock to measure the time
    start = clock();

    // Open the file "Data.txt" for reading and "Data_Thread_A.txt" for writing
    FILE *source_file = fopen("Data.txt", "r");
    FILE *dest_file = fopen(t->filename, "w");

    // Check if the files were opened successfully
    /***TODO check if there is a better way to close the files*/
    if (source_file == NULL || dest_file == NULL) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    // If files have been opened successfully, read the numbers from "Data.txt" and copy them to "Data_Thread_A.txt"
    // While reading, calculate the sum and count of the numbers in the Data.txt file
    while (fscanf(source_file, "%d", &num) == 1) {
        fprintf(dest_file, "%d\n", num);
        sum += num;
        count++;
    }
    
    // If the file is empty, then the average is 0.0, but if it is not empty, then calculate the average
    double average = (count > 0) ? (double)sum / count : 0.0;
    printf("Thread %c Average = %.2f\n", t-> label, average);

    // End the clock to measure the time
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    // total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    *(t->time) = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    //t->time = total_time;  // Store the time taken by the thread in the provided pointer

    // Close the both of the file
    fclose(source_file);
    fclose(dest_file);

    return NULL;  // Exit the thread
}

int main() {
    clock_t start, end;
    double total_time;

    pthread_t threads[3];
    double times[3];

    thread_data args[3] = {
        {'A', &times[0], "Data_Thread_A.txt"},
        {'B', &times[1], "Data_Thread_B.txt"},
        {'C', &times[2], "Data_Thread_C.txt"}
    };

    // Start the clock to measure the time of the main function
    start = clock();

    random_number();

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_func_a, &args[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // End the clock to measure the time of the main function
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("\nThread A time = %.2f ms\n", times[0]);
    printf("Thread B time = %.2f ms\n", times[1]);
    printf("Thread C time = %.2f ms\n\n", times[2]);

    printf("Total time taken by all threads = %.2f ms\n", times[0] + times[1] + times[2]);
    printf("Total time taken by the program = %.2f ms\n", total_time);

    return 0;
}