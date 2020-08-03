#include "ksimple_pstree.h"

static pid_t ppid;

module_param(ppid,int,0644);
void itos(int pid)
{
    int i=0;
    do {
        tmp[i]=pid%10-0+'0';
        pid=pid/10;
        i++;
    } while(pid>0);
    tmp[i]='\0';
    int j;
    i--;
    for(j=0; tmp[j]!='\0'; j++,i--) {
        i2s[j]=tmp[i];
    }
    i2s[j]='\0';
}
char * strcat(char *dest, const char *src)
{
    int i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}
static void child(struct task_struct *p,int d)
{
    struct list_head *list=NULL;
    struct task_struct *psibling;
    int i;
    for(i=0; i<d; i++)
        strcat(msg,"    ");

    strcat(msg,p->comm);
    strcat(msg,"(");
    printk("p-pid:%d\n",p->pid);
    itos(p->pid);
    printk("i2s:%s\n",i2s);
    strcat(msg,i2s);
    strcat(msg,")\n");

    list_for_each(list, &p->children) {
        psibling = list_entry(list, struct task_struct, sibling);
        child(psibling,d+1);
    }
}
static void trace_parent(void)
{
    struct task_struct *p;
    int i;
    pid_t pid=1;
    p = pid_task(find_vpid(pid), PIDTYPE_PID);
    strcat(msg,p->comm);
    strcat(msg,"(");
    //itos(p->pid);
    // strcat(msg,i2s);
    strcat(msg,"1");
    strcat(msg,")\n");
    for(i=0; i<top_id; i++) {
        pid=parent_id[i];
        p = pid_task(find_vpid(pid), PIDTYPE_PID);
        int j=0;
        for(j=0; j<=i; j++)
            strcat(msg,"    ");

        strcat(msg,p->comm);
        strcat(msg,"(");
        itos(p->pid);
        strcat(msg,i2s);
        strcat(msg,")\n");
    }
    top_id=0;
}
static void find_parent(struct task_struct *p)
{
    if(p->pid==1)
        return;
    if(p->parent->pid!=1)
        find_parent(p->parent);
    parent_id[top_id++]=p->pid;

}

static void nl_recv_msg(struct sk_buff *skb)
{
    //module code
    struct task_struct *p;
    struct list_head *list=NULL;
    struct task_struct *psibling;
    msg = (char *)kmalloc(131072, GFP_KERNEL);
    strcpy(msg,"\n");
    //netlink code
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char rec_msg[20];
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Netlink received msg payload:%s\n", (char *)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; /*pid of sending process */
    strcpy(rec_msg,nlmsg_data(nlh));
    printk("rec_msg:%s\n",rec_msg);

    if(rec_msg[1]=='c') {
        if(rec_msg[2]=='\0') {
            ppid=1;
            p = pid_task(find_vpid(ppid), PIDTYPE_PID);
            child(p,0);
        } else {
            int i=2,ppid=0;
            while(rec_msg[i]!='\0') {
                ppid*=10;
                ppid+=rec_msg[i]-'0';
                i++;
            }
            p = pid_task(find_vpid(ppid), PIDTYPE_PID);
            child(p,0);
        }
    } else if(rec_msg[1]=='s') {
        if(rec_msg[2]=='\0')
        {}
        else {
            int i=2,ppid=0;
            while(rec_msg[i]!='\0') {
                ppid*=10;
                ppid+=rec_msg[i]-'0';
                i++;
            }

            p = pid_task(find_vpid(ppid), PIDTYPE_PID);

            list_for_each(list, &p->parent->children) {
                psibling = list_entry(list, struct task_struct, sibling);
                strcat(msg,psibling->comm);
                strcat(msg,"(");
                itos(psibling->pid);
                strcat(msg,i2s);
                strcat(msg,")\n");
            }
        }
    } else if(rec_msg[1]=='p') {
        if(rec_msg[2]=='\0') {
            ppid=pid;
            p = pid_task(find_vpid(ppid), PIDTYPE_PID);
            find_parent(p);
            trace_parent();
        } else {
            int i=2,ppid=0;
            while(rec_msg[i]!='\0') {
                ppid*=10;
                ppid+=rec_msg[i]-'0';
                i++;
            }
            printk("ppid=%d",ppid);
            p = pid_task(find_vpid(ppid), PIDTYPE_PID);
            find_parent(p);
            trace_parent();
        }
    }
    printk("%s\n",msg);
    msg_size = strlen(msg);
    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending bak to user\n");
}

static int __init start_init(void)
{
    printk("Entering: %s\n", __FUNCTION__);
    struct netlink_kernel_cfg cfg = {
        .input = nl_recv_msg,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_protocol, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }
    return 0;
}

static void __exit leave_exit(void)
{
    printk(KERN_INFO "exiting module\n");
    netlink_kernel_release(nl_sk);
}

module_init(start_init);
module_exit(leave_exit);
MODULE_LICENSE("GPL");
