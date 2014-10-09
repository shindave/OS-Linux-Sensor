#include<linux/syscalls.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/kfifo.h>
#include<linux/wait.h>
#define ACC_M struct acc_motion
struct event_unit {
	ACC_M mBaseline;
	wait_queue_t mWaitQueue;
	int event_id;
	int event_count; 
	struct event_unit *next;
};
static struct event_unit *event_list;
static int motion_count = 0;

asmlinkage long sys_accevt_create(struct acc_motion __user *acceleration)
{
	ACC_M *tmpAcc;
	int ret = 0;

	if(acceleration == NULL)
		return -1;
	tmpAcc = (ACC_M *)kmalloc(sizeof(ACC_M), GFP_KERNEL);
	ret = copy_from_user(tmpAcc, acceleration, sizeof(ACC_M));
	if (ret != 0)
		return -1;
	motion_count++;
	struct event_unit *new_event;
	new_event = (struct event_unit *)kmalloc(sizeof(struct event_unit),GFP_KERNEL);
	new_event->event_id = motion_count;
	new_event->event_count = 1;
	new_event->mBaseline = *tmpAcc;
	new_event->next = NULL;
	
	if (event_list==NULL) {
		event_list=new_event;
	} else {
		new_event->next = event_list;
		event_list = new_event;	
	}
	if (motion_count==10) {
		struct event_unit * p = event_list;
		while(p!= NULL) {
			printk("event: eventid=%d",p->event_id);
			p = p->next;
		}
	}
	
	kfree(tmpAcc);
	return motion_count;
}
asmlinkage long sys_accevt_wait(int event_id)
{
	return 0;
}
asmlinkage long sys_accevt_signal(struct dev_acceleration __user *acceleration)
{
	return 0;
}
asmlinkage long sys_accevt_destroy(int event_id)
{
	return 0;
}