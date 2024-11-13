/***********************************************************************
 *
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 29.10.2024
 *
 * Assignment: 6, Observing the OS through the /proc file system
 *
 *      Determine the performance and other behaviour of the OS by inspecting its state - the values stored in its data structures.
 *      Study some aspects of the organization and behaviour of a Linux system by observing values of kernel data structures exposed through the /proc virtual file system.
 *
 *   The /proc virtual file system:
 *      Linux uses the /proc file system to collect information from kernel data structures.
 *      The /proc implementation provided with Linux can read many different kernel data structures. Files in /proc/ subtree each correspond to some kernel data structure.
 *      The subdirectories with numeric names contain virtual files with information about the process whose process ID is the same as the directory name.
 *      Files in /proc can be opened and read like ordinary ASCII files, using library routines such as fgets() or fscanf().
 *
 *   Objective:
 *      Write a C program which should print the following values on terminal
 *      1. Processor Informations (Vendor ID, Model Name and Cache Size in MB)
 *      2. Kernel Name, Kernel Release and Operating System Name.
 *      3. The amount of memory configured into this computer and Free Memory
 *      4. Amount of time since the system was last booted (In Hours, Minutes, Seconds)
 *
 *
 *
 * Input Description:
 *
 * Output Description:
 *
 *
 * Compilation Command: gcc 1095_6.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------
Processor Information:
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB
Vendor ID       : GenuineIntel
Model Name      : 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz
Cache Size      : 8 MB

Kernel Information:
Kernel Version Info: Linux version 6.6.54-2-MANJARO (builduser@fv-az1985-536) (gcc (GCC) 14.2.1 20240910, GNU ld (GNU Binutils) 2.43.0) #1 SMP PREEMPT_DYNAMIC Tue Oct  8 03:11:08 UTC 2024

Memory Information:
Total Memory: 15793 MB
Free Memory: 7855 MB

Uptime Information:
System Uptime: 0 hours, 34 minutes, 30 seconds
 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to print processor information */
void print_processor_info()
{
    char buffer[256];

    /* Read CPU info */
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }

    /* Read each line of /proc/cpuinfo */
    while (fgets(buffer, sizeof(buffer), cpuinfo)) {
        /* Check for vendor_id */
        if (strstr(buffer, "vendor_id")) {
            printf("Vendor ID %s", buffer + strlen("vendor_id"));
        }
        /* Check for model name */
        else if (strstr(buffer, "model name")) {
            printf("Model Name %s", buffer + strlen("model name"));
        }
        /* Check for cache size */
        else if (strstr(buffer, "cache size")) {
            long cache_size   = 0;
            char const* field = buffer + strlen("cache size");
            char* loc         = strstr(field, ":") + 1;
            sscanf(loc, "%ld", &cache_size);
            cache_size /= 1024; /* Convert to MB*/
            long len    = sprintf(loc, " %ld MB\n", cache_size);
            loc[len]    = 0;
            printf("Cache Size %s", field);
        }
    }
    fclose(cpuinfo);
}

/* Function to print kernel information */
void print_kernel_info()
{
    char buffer[256];

    /* Read kernel info */
    FILE* uname = fopen("/proc/version", "r");
    if (uname == NULL) {
        perror("Failed to open /proc/version");
        return;
    }

    /* Read kernel version info */
    fgets(buffer, sizeof(buffer), uname);
    printf("Kernel Version Info: %s", buffer);
    fclose(uname);
}

/* Function to print memory information */
void print_memory_info()
{
    long total_memory = 0;
    long free_memory  = 0;
    /* Open /proc/meminfo to read memory stats */
    FILE* meminfo = fopen("/proc/meminfo", "r");
    if (meminfo == NULL) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char buffer[256];
    /* Read each line of /proc/meminfo */
    while (fgets(buffer, sizeof(buffer), meminfo)) {
        /* Parse total memory */
        if (sscanf(buffer, "MemTotal: %ld kB", &total_memory) == 1) {
            total_memory /= 1024; /* Convert to MB */
        }
        /* Parse free memory */
        else if (sscanf(buffer, "MemFree: %ld kB", &free_memory) == 1) {
            free_memory /= 1024; /* Convert to MB */
        }
    }
    fclose(meminfo);

    /* Print total and free memory */
    printf("Total Memory: %ld MB\n", total_memory);
    printf("Free Memory: %ld MB\n", free_memory);
}

/* Function to print system uptime */
void print_uptime()
{
    long uptime_seconds = 0;
    /* Open /proc/uptime to read uptime */
    FILE* uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        perror("Failed to open /proc/uptime");
        return;
    }

    /* Read uptime in seconds */
    fscanf(uptime_file, "%ld", &uptime_seconds);
    fclose(uptime_file);

    /* Calculate hours, minutes, and seconds */
    long hours   = uptime_seconds / 3600;
    long minutes = (uptime_seconds % 3600) / 60;
    long seconds = uptime_seconds % 60;

    /* Print the system uptime */
    printf("System Uptime: %ld hours, %ld minutes, %ld seconds\n", hours, minutes, seconds);
}

/* Main function to execute the program */
int main()
{
    printf("Processor Information:\n");
    print_processor_info();

    printf("\nKernel Information:\n");
    print_kernel_info();

    printf("\nMemory Information:\n");
    print_memory_info();

    printf("\nUptime Information:\n");
    print_uptime();

    return 0;
}
