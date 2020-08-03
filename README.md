# linux-process-tree
An exercise on transferring data between user and kernel processes via Socket. And the implementation of linux pstree command, which is shown in Figure 2. 

Figure 1. shows the communication between user and kernel processes.
1. simple_pstree packages the option and PID into a message and sends it to ksimple_pstree via the socket.
2. ksimple_pstree packages the tree of processes into a message and sends it to simple_pstree via the socket.
3. simple_pstree displays the received message.
<center>
<img src="https://github.com/Msiciots/linux-process-tree/raw/master/img/process.png" width="50%">
</center>
<center>Figure 1. The processes communication</center>

<center>
<img src="https://github.com/Msiciots/linux-process-tree/raw/master/img/Execution_Ex1.png" width="80%">
</center>
<center>Figure 2. An example for executions</center>

## Build
Download source files from Github repository. And move to the source file directory.
```
$ git clone https://github.com/Msiciots/linux-process-tree.git
$ cd linux-process-tree
```
Change the build.sh to an executable file and build.
```
$ chmod +x build.sh
$ ./build.sh
```
## Run
Show all processes by default.
```
$ ./simple_pstree
```
Given a process pid, show all **child** processes. 
```
$ ./simple_pstree -c1234 # 1234 is the process pid.
```
Given a process pid, show all **parent** processes. 
```
$ ./simple_pstree -p1234 # 1234 is the process pid.
```
Given a process pid, show all **siblings** processes. 
```
$ ./simple_pstree -s1234 # 1234 is the process pid.
```
Once the program is no longer in use, remember to remove the kernel module.
```
$ cd ./module
$ make rm
```
## Reference
- [Kernel Space, User Space Interfaces](http://wiki.tldp.org/kernel_user_space_howto)
- [NETLINK(7)](https://man7.org/linux/man-pages/man7/netlink.7.html)
- [Kernel Korner - Why and How to Use Netlink Socket](https://www.linuxjournal.com/article/7356)

- [Linux Device Drivers (chapter 2)](https://lwn.net/Kernel/LDD3/)
- [struct task_struct](https://elixir.bootlin.com/linux/latest/source/include/linux/sched.h#L592)