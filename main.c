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

    // Create a file named "Data.txt"
    FILE *file = fopen("Data.txt", "w");
    
    // In Data.txt, write 10 random numbers between 0 and 100
    for (int i = 0; i < 1000; i++) {
        int num = rand() % 101;
        fprintf(file, "%d\n", num);
    }

    // Close the file
    fclose(file);    
}

void *thread_func_a(void *arg) {

    // Define the variables used to measure the time taken by the thread, and variables to calculate the average of the numbers copied in the Data_Thread.txt file
    clock_t start, end;
    double total_time;
    long long sum = 0;
    int count = 0;
    int num;

    // Start the clock to measure the time
    start = clock();

    // Open the file "Data.txt" for reading and "Data_Thread_A.txt" for writing
    FILE *source_file = fopen("Data.txt", "r");
    FILE *dest_file = fopen("Data_Thread_A.txt", "w");

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
    printf("Thread A Average = %.2f\n", average);

    // End the clock to measure the time
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    double *time_ptr = (double *)arg;
    *time_ptr = total_time;

    // Close the both of the file
    fclose(source_file);
    fclose(dest_file);

    return NULL;  // Exit the thread
}

void *thread_func_b(void *arg) {

    // Define the variables used to measure the time taken by the thread, and variables to calculate the average of the numbers copied in the Data_Thread.txt file
    clock_t start, end;
    double total_time;
    long long sum = 0;
    int count = 0;
    int num;

    // Start the clock to measure the time
    start = clock();

    // Open the file "Data.txt" for reading and "Data_Thread_A.txt" for writing
    FILE *source_file = fopen("Data.txt", "r");
    FILE *dest_file = fopen("Data_Thread_B.txt", "w");

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
    printf("Thread B Average = %.2f\n", average);

    // End the clock to measure the time
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    double *time_ptr = (double *)arg;
    *time_ptr = total_time;


    // Close the both of the file
    fclose(source_file);
    fclose(dest_file);

    return NULL;  // Exit the thread
}

void *thread_func_c(void *arg) {

    // Define the variables used to measure the time taken by the thread, and variables to calculate the average of the numbers copied in the Data_Thread.txt file
    clock_t start, end;
    double total_time;
    long long sum = 0;
    int count = 0;
    int num;

    // Start the clock to measure the time
    start = clock();

    // Open the file "Data.txt" for reading and "Data_Thread_A.txt" for writing
    FILE *source_file = fopen("Data.txt", "r");
    FILE *dest_file = fopen("Data_Thread_C.txt", "w");

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
    printf("Thread C Average = %.2f\n", average);

    // End the clock to measure the time
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    double *time_ptr = (double *)arg;
    *time_ptr = total_time;

    // Close the both of the file
    fclose(source_file);
    fclose(dest_file);

    return NULL;  // Exit the thread
}

int main() {
    clock_t start, end;
    double total_time;
    double timeA, timeB, timeC;

    // Start the clock to measure the time of the main function
    start = clock();
    srand(time(NULL));  // Seed the random number generator with current time
    random_number();
    pthread_t threadA, threadB, threadC;

    pthread_create(&threadA, NULL, thread_func_a, &timeA);
    pthread_create(&threadB, NULL, thread_func_b, &timeB);
    pthread_create(&threadC, NULL, thread_func_c, &timeC);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // End the clock to measure the time of the main function
    end = clock();

    // Calculate the total time taken by the thread in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("\nThread A time = %.2f ms\n", timeA);
    printf("Thread B time = %.2f ms\n", timeB);
    printf("Thread C time = %.2f ms\n\n", timeC);
    printf("Total time taken by all threads = %.2f ms\n", timeA + timeB + timeC);
    printf("Total time taken by the program = %.2f ms\n", total_time);
    return 0;  // Exit the program
}