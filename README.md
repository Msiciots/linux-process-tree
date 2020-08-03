# linux-process-tree
An exercise on transferring data between kernel and user processes by Socket. And the implementation of showing linux process tree.
1. simple_pstree packages the option and PID into a message and sends it to ksimple_pstree via the socket.
2. ksimple_pstree packages the tree of processes into a message and sends it to simple_pstree via the socket.
3. simple_pstree displays the received message.
<center>
<img src="https://github.com/Msiciots/linux-process-tree/raw/master/img/processes.png" width="50%">
</center>

## Reference
