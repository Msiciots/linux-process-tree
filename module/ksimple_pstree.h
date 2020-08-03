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
int parent_id[100];
int top_id=0;
char *msg=NULL;
char i2s[100];
char tmp[100];