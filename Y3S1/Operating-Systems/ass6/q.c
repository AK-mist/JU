/***********************************************************************
 *
 * Name:
 * Roll:
 * 
 * Date:
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
 * Compilation Command: gcc 100_6.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------

Processor Information:
	Vendor ID: GenuineIntel
	CPU Model: Intel(R) Xeon(R) CPU E5-2609 v4 @ 1.70GHz
	Cache Size: 20.00 MB

Kernel Information:
	Linux version 3.10.0-514.21.1.el7.x86_64 (mockbuild@sl7-uefisign.fnal.gov) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-11) (GCC) ) #1 SMP Thu May 25 12:04:35 CDT 2017

Memory Information:
	MemTotal: 64013.00 MB
	MemFree: 38419.93 MB
	MemAvailable: 59885.86 MB
	Buffers: 0.94 MB
	Cached: 21634.56 MB
	SwapCached: 0.00 MB

System Uptime:
	Uptime: 336 days, 3 hours, 26 minutes, 21 seconds
 -----------------------------------/
***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE         256
#define CPUINFO_FILEPATH    "/proc/cpuinfo"
#define VERSION_FILEPATH    "/proc/version"
#define MEMINFO_FILEPATH    "/proc/meminfo"
#define UPTIME_FILEPATH     "/proc/uptime" 

void error_exit(const char* error_msg);
void get_processor_info();
void get_kernel_info();
void get_memory_info();
void get_uptime();

int main (int argc, char **argv) {
    get_processor_info();
    get_kernel_info();
    get_memory_info();
    get_uptime();

    return 0;
}

void error_exit(const char* error_msg) {
    perror(error_msg);
    exit(EXIT_FAILURE);
}

void get_processor_info() {
    FILE *fd;
    char buffer[BUFFER_SIZE];
    char vendor_id[BUFFER_SIZE] = {0};
    char cpu_model[BUFFER_SIZE] = {0};
    int cache_size_kb = 0;

    if (!(fd = fopen(CPUINFO_FILEPATH, "r"))) {
        error_exit("Error: opening /proc/cpuinfo");
    }

    printf("\nProcessor Information:\n");

    while (fgets(buffer, sizeof(buffer), fd)) {
        if (strncmp(buffer, "vendor_id", 9) == 0) {
            char *temp = strchr(buffer, ':') + 2;
            strcpy(vendor_id, temp);
            vendor_id[strcspn(vendor_id, "\n")] = '\0';
        }
        if (strncmp(buffer, "model name", 10) == 0) {
            char *temp = strchr(buffer, ':') + 2;
            strcpy(cpu_model, temp);
            cpu_model[strcspn(cpu_model, "\n")] = '\0';
        }
        if (strncmp(buffer, "cache size", 10) == 0) {
            char *cache_size = strchr(buffer, ':') + 2;
            cache_size_kb = atoi(cache_size);
        }
    }

    fclose(fd);

    if (!vendor_id || !cpu_model || !cache_size_kb) {
        error_exit("Error: reading /proc/cpuinfo");
    }

    printf("\tVendor ID: %s\n", vendor_id);
    printf("\tCPU Model: %s\n", cpu_model);
    printf("\tCache Size: %.2f MB\n", cache_size_kb / 1024.0);
}

void get_kernel_info() {
    FILE *fd;
    char buffer[BUFFER_SIZE];

    if (!(fd = fopen(VERSION_FILEPATH, "r"))) {
        error_exit("Error: opening /proc/version");
    }

    printf("\nKernel Information:\n");

    while (fgets(buffer, sizeof(buffer), fd)) {
        printf("\t%s", buffer);
    }

    fclose(fd);
}

void get_memory_info() {
    FILE *fd;
    char buffer[BUFFER_SIZE];

    if (!(fd = fopen(MEMINFO_FILEPATH, "r"))) {
        error_exit("Error: opening /proc/meminfo");
    }

    printf("\nMemory Information:\n");

    while (fgets(buffer, sizeof(buffer), fd)) {
        if ((strncmp(buffer, "MemTotal", 8) == 0) ||
            (strncmp(buffer, "MemFree", 7) == 0) || 
            (strncmp(buffer, "MemAvailable", 12) == 0) ||
            (strncmp(buffer, "Buffers", 7) == 0) ||
            (strncmp(buffer, "Cached", 6) == 0) ||
            (strncmp(buffer, "SwapCached", 10) == 0)
            ) {
            //printf("\t%s", buffer);
            
            char *mem_type_start = buffer;
            char *mem_type_end = strchr(buffer, ' ');
            char *mem_size = strchr(buffer, ':') + 2;
            
            printf("\t");
            while (mem_type_start < mem_type_end && *mem_type_start != '\0') {
                putchar(*mem_type_start);
                mem_type_start++;
            }
            int mem_size_kb = atoi(mem_size);
            printf(" %.2f MB\n", mem_size_kb / 1024.0);
        }
    }

    fclose(fd);
}

void get_uptime() {
    FILE *fd;
    double uptime;

    if (!(fd = fopen(UPTIME_FILEPATH, "r"))) {
        error_exit("Error: opening /proc/uptime");
    }

    printf("\nSystem Uptime:\n");

    if (fscanf(fd, "%lf", &uptime) == 1) {
        int days = (int)(uptime / 86400);
        int hours = (int)((uptime - days * 86400) / 3600);
        int minutes = (int)((uptime - days * 86400 - hours * 3600) / 60);
        int seconds = (int)(uptime - days * 86400 - hours * 3600 - minutes * 60);

        printf("\tUptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
    } else {
        error_exit("Error: reading /proc/uptime");
    }

    fclose(fd);
}
