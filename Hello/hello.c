#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#define MAX_SIZE 1024

asmlinkage long sys_hello(char __user *input, char __user *output, int len)
{
	char tmp[MAX_SIZE]; 

	if ( (len < 1) || (len >  MAX_SIZE) ) {
		printk(KERN_ALERT "helloworld() failed: illegal len (%d)!", len);
		return(-1);
	}

	/* copy buff from user space into a kernel buffer */
	if (copy_from_user(tmp, input, len)) {
		printk(KERN_ALERT "helloworld() failed: copy_from_user() error");
		return(-1);
	}
	
	tmp[len] = '\0';

	char temp[MAX_SIZE *2];
	strcpy(temp,  "Hello World from ");
	strcat(temp, tmp);

	
	printk("%s\n", temp);
	int i = 0;
	for(i = 0; i < strlen(temp); i++){
		//printk(KERN_ALERT"%c\n", temp[i]);
	}

	if (copy_to_user(output, temp, strlen(temp))) {
		printk(KERN_ALERT "sys_hello() failed: copy_to_user() error");
		return(-1);
	}
	 printk("%s\n", temp);
        
        return 0;
}

asmlinkage long sys_process_table(void){

	struct task_struct *task;
	printk(KERN_ALERT "==============PROCESS TABLE================\n");
	for_each_process(task){
printk(KERN_ALERT "[%d]\t %d\t %d\t %s\t \n",task->pid, task->real_parent->pid, task->state, task->comm);
	}
	printk(KERN_ALERT "===========================================\n");
	return 0;
}


asmlinkage long sys_get_pname(unsigned int __user *pid, char __user *pname){
	
	if(pid <= 0){
	  char * str = "Error: pid can't be a negative number.\n";
	  printk(KERN_ALERT "%s", str);
	  if (copy_to_user(pname, str, strlen(str))) {
	  	printk(KERN_ALERT "sys_get_process() failed: copy_to_user() error");
		return(-1);
	  }
	  return(-1);
	}

	int fd;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	char name[1024];
	char temp[1024];
 
    	if(true){
          snprintf(name, 1024, "/proc/%d/cmdline",pid);
	  strcpy(temp, name);
	  fd = sys_open(name, O_RDONLY, 0);
	  while (sys_read(fd, name, 1024) == 1){} 
          if('\n' == name[1024-1]){
            name[1024-1]='\0';
	  }
	  sys_close(fd);
	  set_fs(old_fs);
	  if(strcmp(name, temp) == 0){
	    if(copy_to_user(pname, "File not found", strlen(name))) {
		printk(KERN_ALERT "sys_get_process() failed: copy_to_user() error");
		return(-1);
	    }
//	    printk(KERN_ALERT "File failed to open - pid: %d\n", pid);
	    return (-1);
	  }else{
	    if (copy_to_user(pname, name, strlen(name))) {
		printk(KERN_ALERT "sys_get_process() failed: copy_to_user() error");
		return(-1);
	    }
	  }
//	  printk("%s\n", name);
	}
	return 0;
}


