#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1024
#define RAND_MAX 2147483647

#define sys_hello 	545
#define sys_table 	546
#define sys_get_pname 	547

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int main()
{	 
	 srand(time(NULL));

	 char * name = "Ayokunle Adeosun";
	 char output [MAX_SIZE *2];

         long int amma = syscall(sys_hello, name, output, strlen(name));
         printf("System call sys_hello returned %ld\n", amma);
	 printf("Output: %s\n", output);

	 amma = syscall(sys_table);
	 printf("System call sys_table returned %ld\n", amma); 

	 int pid;
	 char pname [1024];
	 amma = -1;
	 while(amma == -1){
	   pid = rand_interval(1, 3000);
	   amma = syscall(sys_get_pname, pid, pname);		
	 }
	 printf("System call sys_ get_pname %ld\n", amma);
	 printf("PID: %d Process: %s\n", pid, pname);		
         
	 return 0;
}
