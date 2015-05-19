#include <sys/uio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mach/mach.h> 
#include <mach/mach_vm.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    int gcount;
    long address;
    unsigned char * bytes;
    mach_port_t task;
    thread_act_port_array_t threadList;
    mach_msg_type_number_t threadCount;

printf("  __  __            __  __                                 __  __             _           \n");
printf(" |  \\/  |          |  \\/  |                               |  \\/  |           | |          \n");
printf(" | \\  / | __ _  ___| \\  / | ___ _ __ ___   ___  _ __ _   _| \\  / | __ _ _   _| | ___ _ __ \n");
printf(" | |\\/| |/ _` |/ __| |\\/| |/ _ \\ '_ ` _ \\ / _ \\| '__| | | | |\\/| |/ _` | | | | |/ _ \\ '__|\n");
printf(" | |  | | (_| | (__| |  | |  __/ | | | | | (_) | |  | |_| | |  | | (_| | |_| | |  __/ |   \n");
printf(" |_|  |_|\\__,_|\\___|_|  |_|\\___|_| |_| |_|\\___/|_|   \\__, |_|  |_|\\__,_|\\__,_|_|\\___|_|   \n");
printf("                                                      __/ |                               \n");
printf("                                                     |___/                                \n");

   
    printf("PID to query, or 0 for this process: ");
    scanf("%d", &pid);

    if(pid == 0){
        pid = getpid();
    }

    printf("[i] OK, using PID: %d\n", pid);

    int retval = task_for_pid(mach_task_self(), pid, &task);

    if(retval!=KERN_SUCCESS){
        fprintf(stderr, "[!] Failed to get task. Do you have perms?\n");
        fprintf(stderr, "Error: %s\n", mach_error_string(retval));
        return 1;
    }

    printf("[i] Querying thread list\n");
    retval = task_threads(task, &threadList, &threadCount);

    if(retval!=KERN_SUCCESS){
        fprintf(stderr, "[!] Failed to read thread list\n");
        fprintf(stderr, "Error: %s\n", mach_error_string(retval));
        return 1;
    }

    printf("[+] Thread Count: %d\n", threadCount);

    printf("Address to write to: ");
    scanf("%ld", &address);

    printf("Number of bytes: ");
    scanf("%d", &gcount);

    printf("[i] OK. Generating %d bytes of pseudorandom data...\n", gcount);

    bytes = malloc(gcount); //allocate memory for pseudo-random bytes
    srand(time(NULL)); //seed with current time

    int i;
    for(i = 0; i < gcount; i++){
        bytes[i] = rand();
    }

    printf("[i] Brace yourselves, writing %d bytes of gibberish to address %ld\n", gcount, address);

    retval = mach_vm_write(task, (mach_vm_address_t)address, (vm_offset_t)*bytes, gcount);

    if(retval != KERN_SUCCESS){
        fprintf(stderr, "[!] Failed to write %d bytes to address %ld\n", gcount, address);
        fprintf(stderr, "Error: %s\n", mach_error_string(retval));
        return 1;
    }else{
        printf("[+] Success, wrote %d bytes to address %ld\n", gcount, address);
        return 0;
    }
}
