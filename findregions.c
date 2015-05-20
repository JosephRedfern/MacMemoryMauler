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
    mach_port_t task;

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
    }else{
        printf("[+] Successfully acquired task\n");
    }
    
    printf("[i] Attempting to read memory maps...\n");

    natural_t count;
    mach_vm_read_entry_t entries;

    retval = mach_vm_read_list(task, entries, count);

    if(retval != KERN_SUCCESS){
        printf("Error reading ranges: %s\n", mach_error_string(retval));
//        return 1;
    }

    printf("Entry Count: %d", count);
    
}
