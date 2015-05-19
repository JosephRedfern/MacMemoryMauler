#include <sys/uio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mach/mach.h> 
#include <mach/mach_vm.h>
#include <stdio.h>

#define COUNT 100000000

int main() {
    int pid;
    int gcount;
    int address;
    unsigned char * bytes;
    mach_port_t task;
    thread_act_port_array_t threadList;
    mach_msg_type_number_t threadCount;

   
    printf("PID to query: ");
    scanf("%d", &pid);

    printf("[+] got current PID, %d\n", pid);

    int retval = task_for_pid(mach_task_self(), pid, &task);

    printf("[+] retval == %d\n", retval);
    printf("Response: %s\n", mach_error_string(retval));

    if(retval!=KERN_SUCCESS){
        printf("Dying.\n");
        return 1;
    }

    printf("[~] Querying thread list\n");
    task_threads(task, &threadList, &threadCount);

    printf("[+] Thread Count: %d\n", threadCount);

    printf("Address to write to: ");
    scanf("%d", &address);

    printf("Number of bytes: ");
    scanf("%d", &gcount);

    printf("[+] Brace yourselves, writing %d bytes of gibberish to address %d\n", gcount, address);


    bytes = malloc(gcount); //allocate memory for pseudo-random bytes

    srand(time(NULL)); //seed with current time


    int i;
    for(i = 0; i < gcount; i++){
        if(i%100 == 0)
            printf(".");
        bytes[i] = rand();
    }

    printf("\n[i] Bytes: %s\n", bytes);

    int wretval;

    long n = 0;

    while((wretval = mach_vm_write(task, (vm_address_t)address, '0', 1)) != KERN_SUCCESS && address < pow(2,32)){
        address+= 1;
        if(n%1000 == 0){
            printf("@%d\n", address);
        }
        n++;
    }

    printf("\nResponse: %s", mach_error_string(wretval));

    return 0;

}
