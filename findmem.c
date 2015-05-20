#include <sys/uio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mach/mach.h> 
#include <mach/mach_vm.h>
#include <stdio.h>
#include <unistd.h>

#define TICK 5 //in seconds for read spead reporting

int main() {
    int pid;
    int gcount;
    long address;
    unsigned char * bytes;
    mach_port_t task;
    thread_act_port_array_t threadList;
    mach_msg_type_number_t threadCount;

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

    printf("Address to start reading from: ");
    scanf("%ld", &address);

    printf("Number of bytes to read:: ");
    scanf("%d", &gcount);

    printf("[i] Staring... \n");

    bytes = malloc(gcount); //Allocate memory for reading

    time_t temptime = time(NULL);
    long tempaddr = address;

    while(address < pow(2, 63)){
        retval = mach_vm_write(task, (mach_vm_address_t)address, (vm_offset_t)*bytes, gcount);
        if(retval == KERN_SUCCESS){
            printf("Succesfull Read from @0x%016lx: %s\n", address, bytes);
        }

        if(time(NULL) - temptime > TICK){ //probably a load of overhead in calling time()
            long bytes_read = address - tempaddr;
            int seconds_elapsed = time(NULL) - temptime;

            float read_rate = bytes_read/seconds_elapsed/(1024*1024);

            printf("Tick... currently at 0x%016lx (%f MB/sec)\n", address, read_rate);
            temptime = time(NULL);
            tempaddr = address;
        }

        address += gcount; //move to next chunk
    }

}
