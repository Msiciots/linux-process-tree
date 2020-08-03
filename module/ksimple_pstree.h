#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

#define NETLINK_protocol 17

struct sock *nl_sk = NULL;
static pid_t ppid;
module_param(ppid,int,0644);

int parent_id[100];
int top_id=0;
char *msg=NULL;
char i2s[100];
char tmp[100];

void itos(int pid);
char * strcat(char *dest, const char *src);
static void child(struct task_struct *p,int d);
static void trace_parent(void);
static void find_parent(struct task_struct *p);
static void nl_recv_msg(struct sk_buff *skb);
static int __init start_init(void);
static void __exit leave_exit(void);