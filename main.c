/** Title: Threads Assignment
 * Author: Wendy Arenas Rosas
 * CSC 3350, Andy Cameron
 * Date: 2025-05-26
 * 
 * Description: 
 * Implement a program (run from the shell command prompt) that creates a file with one 
 * million random numbers between 0 and 100. Then, 3 threads execute concurrently to copy 
 * the numbers line-by-line to 3 new files and calculate the average of the numbers in the 
 * data file. The time (in milliseconds) each thread takes, and the total time the main 
 * routine takes to run, is measured and output to the console.
 * */
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

/**random_number is a function that  generates a file named "Data.txt" with 1,000,000 random numbers between 0 and 100.**/
void random_number() {

    // Seed the random number generator with the current time
    // This ensures that the random numbers generated are different each time 
    // the program is run
    srand(time(NULL));

    // Create a file named "Data.txt"
    FILE *file = fopen("Data.txt", "w");
    
    // In Data.txt, write 1,000,000 random numbers between 0 and 100
    for (int i = 0; i < 1000000; i++) {
        int num = rand() % 101;
        fprintf(file, "%d\n", num);
    }

    // Close the file
    fclose(file);    
}

// The struct thread_data is used to pass data to each thread.
// It contains a label for the thread (either A,B, or C later described in the program), 
// a pointer to a double variable to store the time taken by the thread, and a pointer to
// the filename where the thread will copy and write its output from the Data.txt file.
typedef struct {
    char label;
    double *time;
    const char* filename;
} thread_data;

// The thread_func function is executed by each thread where it reads numbers from 
// "Data.txt", and copies them to their respective output files refered to as "Data_Thread_X.txt", where X is the letter A, B, or C, depending on which thread created the file. As it copies the numbers,
// it also calculates the sum and average of the numbers read.
void *thread_func(void *arg) {

    // Cast the argument to thread_data type to access the thread-specific data
    thread_data* t = (thread_data*)arg;

    // The variables start and end are used to measure the starting and ending times of the thread.
    clock_t start, end;

    // These variables will be used to calculate the sum and count of the numbers read from
    // Data.txt, which will be used to calculate the average later.
    long long sum = 0;
    int count = 0;

    // The variable num will be used to read each number from the Data.txt file.
    int num;

    // Start the clock to measure the time
    start = clock();

    // Open the file "Data.txt" for reading and the thread's file "Data_Thread_X.txt" for writing. Source file is "Data.txt" and destination file is "Data_Thread_X.txt" 
    FILE *source_file = fopen("Data.txt", "r");
    FILE *dest_file = fopen(t->filename, "w");

    // Check if the files were opened successfully
    if (source_file == NULL) {
        // If the source file cannot be opened, print an error message and exit the thread
        perror("Error opening Data.txt");

        pthread_exit(NULL);
    } else if (dest_file == NULL) {
        perror("Error opening output file");

        // Close the source file before exiting
        fclose(source_file);

        // Exit the thread if the destination file cannot be opened
        pthread_exit(NULL);
    }

    // If files have been opened successfully, read the numbers from "Data.txt" 
    // and copy them to "Data_Thread_X.txt".
    // While reading, calculate the sum and count of the numbers in the "Data.txt" file
    while (fscanf(source_file, "%d", &num) == 1) {
        fprintf(dest_file, "%d\n", num);
        sum += num;
        count++;
    }
    
    // If the file is empty, then the average is 0.0, but if it is not empty, then calculate the average
    double average = (count > 0) ? (double)sum / count : 0.0;

    // Print the average calculated by each thread to the console
    printf("Thread %c Average = %.2f\n", t-> label, average);

    // End the clock to measure the time
    end = clock();

    // Calculate the total time taken by the thread in milliseconds, and store the time
    // taken by the thread in the provided pointer
    *(t->time) = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    // Close the both of the files
    fclose(source_file);
    fclose(dest_file);

    return NULL;
}

int main() {

    // Set up variables that will keep track of the starting and ending time, and a variable
    // that will keep track of the total time taken by the main function.
    clock_t start, end;
    double total_time;

    // Create an array of thread identifiers and an array to store the time taken by each thread
    pthread_t threads[3];
    double times[3];

    // Create an array of thread_data structures to pass data to each thread
    // Each thread will have a label, a pointer to its time variable, and a filename for output
    thread_data args[3] = {
        {'A', &times[0], "Data_Thread_A.txt"},
        {'B', &times[1], "Data_Thread_B.txt"},
        {'C', &times[2], "Data_Thread_C.txt"}
    };

    // Start the clock to measure the time of the main function. Then call 
    // the random_number function to generate random numbers to the Data.txt file.
    start = clock();

    random_number();

    // After Data.txt has been created, create three threads, each executing the thread_func function with its corresponding thread_data structure.
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    // Wait for all threads to finish execution
    // This ensures that the main function does not exit before the threads
    // complete their work.
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // End the clock to measure the time of the main function
    end = clock();

    // Display the total time taken by the threads in milliseconds
    total_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("\nThread A time = %.2f ms\n", times[0]);
    printf("Thread B time = %.2f ms\n", times[1]);
    printf("Thread C time = %.2f ms\n\n", times[2]);

    // Calculate and display the total time taken by all threads and the total time taken by the program
    printf("Total time taken by all threads = %.2f ms\n", times[0] + times[1] + times[2]);
    printf("Total time taken by the program = %.2f ms\n", total_time);

    return 0;
}