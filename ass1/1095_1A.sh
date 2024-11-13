/******************************

Name: Chirag Chowdhury
Roll: 002211001095

Assignment: 1A [Familiarity with User and System level OS Commands]

Date: 29.07.2024
*******************************/


Q1. who
 <Answer >
This command displayes all the users who have logged into the system currently
 < Output >
(unknown) :0           2023-10-30 11:07 (:0)
be2284   pts/0        2024-07-29 12:20 (172.16.4.139)
root     pts/1        2024-07-29 11:39 (172.16.5.159)
be2298   pts/2        2024-07-29 11:57 (172.16.4.142)
be22L06  pts/3        2024-07-29 11:57 (172.16.4.144)
be22105  pts/4        2024-07-29 12:14 (172.16.4.133)
be2288   pts/5        2024-07-29 11:58 (172.16.4.140)
be2297   pts/6        2024-07-29 11:58 (172.16.4.160)
be22103  pts/7        2024-07-29 12:05 (172.16.4.155)
be2276   pts/8        2024-07-29 11:59 (172.16.4.137)
be22100  pts/9        2024-07-29 12:00 (172.16.4.158)
be22L04  pts/10       2024-07-29 11:50 (172.16.4.216)
be22108  pts/11       2024-07-29 12:01 (172.16.4.159)
be22L04  pts/12       2024-07-29 12:21 (172.16.4.161)
be2296   pts/13       2024-07-29 12:04 (172.16.4.176)
be2294   pts/14       2024-07-29 12:05 (172.16.4.153)
be22104  pts/15       2024-07-29 12:06 (172.16.4.135)
be22109  pts/17       2024-07-29 12:06 (172.16.4.148)
be2289   pts/19       2024-07-29 12:07 (172.16.4.147)
be2286   pts/20       2024-07-29 12:09 (172.16.4.129)
be2279   pts/21       2024-07-29 12:09 (172.16.4.145)
be2292   pts/22       2024-07-29 12:10 (172.16.4.146)
be2295   pts/23       2024-07-29 12:12 (172.16.4.136)
be2279   pts/24       2024-07-29 12:13 (172.16.4.145)
be22106  pts/25       2024-07-29 12:18 (172.16.4.162)
be2298   pts/26       2024-07-29 12:16 (172.16.4.143)
root     pts/27       2024-07-29 12:19 (172.16.4.192)

# ........

Q2. whoami
 < Answer for Q2 >
It displays current User name, Terminal number, date and time at which user logged into the system.
 < Output for Q2>
be22---

# ........

Q3. pwd
 < Answer >
It displays the currnet working directory
 < Output >
/home/usr/student/ug/yr22/be22---/OS_LAB/assignment-1A

# ........

Q4. date
 < Answer >
It displays the system date and time.
 < Output >
Mon Jul 29 12:44:58 IST 2024

# ........

Q5. ls
 < Answer >
It displays the files and directories stored in the current directory.
To list the files and directries stored in a directory use syntax
    $ls <dirname>
 < Output >
another-demo-file.md
submission-file.txt

# ........

Q6. mkdir
 < Answer >
Used to create directories
 < Syntax >
$mkdir <dirtectory_name>

# ........

Q7. clear
 < Answer >
It clears the screen.

# ........

Q8. cd
 < Answer >
It is used to change the current directory to any other specified.
 < Syntax >
$cd <directory_name>

# ........

Q9. cd..
 < Answer >
Used to come out of the current directory.

# ........

Q10. rmdir
 < Answer >
Used to delete directories, -i -r flags can be used with this.
 < Syntax >
$rmdir <directory_name>

# ........

Q11. cat
 < Answer >
Displays the content of a file.
 < Syntax >
$cat <file_name>

# ........

Q12. cp
 < Answer >
It is used to copy a file.
 < Syntax >
$cp <source_file> <destination_file>

# ........

Q13. mv
 < Answer >
It is used to change the name of a file.
Which means it can be used to change a file location also.
 < Syntax >
$mv <old_file> <new_file>

# ........

Q14. rm
 < Answer >
It is used to delete an existing file.
Can be used to delete directories too, using -r flag meaning recursive deletion.
 < Syntax >
$rm <file_name>

# ........

Q15. stat
 < Answer >
It is used to display file or file system status.
 < Syntax >
$stat <file_name>
 < Output >
  File: ‘submission-file.txt’
  Size: 3078            Blocks: 8          IO Block: 4096   regular file
Device: fd02h/64770d    Inode: 4862552104  Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 3083/ be22100)   Gid: ( 2924/  ugyr22)
Context: unconfined_u:object_r:user_home_t:s0
Access: 2024-07-29 13:22:17.137455298 +0530
Modify: 2024-07-29 13:21:21.182454577 +0530
Change: 2024-07-29 13:21:21.200454578 +0530
 Birth: -

# ........

Q16. stty
 < Answer >
Change and print terminal line settings.
Using  –a flag prints all settings in a readable form.
 < Output >
speed 38400 baud; rows 44; columns 168; line = 0;
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = <undef>; eol2 = <undef>; swtch = <undef>; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R; werase = ^W;
lnext = ^V; flush = ^O; min = 1; time = 0;
-parenb -parodd -cmspar cs8 -hupcl -cstopb cread -clocal -crtscts
-ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl ixon -ixoff -iuclc -ixany -imaxbel -iutf8
opost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
isig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke

# ........

Q17. tty
 < Answer >
It prints the filename of the terminal connected to standard input.
Can be used to change between terminals in a virtual environment.
 < Output >
/dev/pts/9

# ........

Q18. uname
 < Answer >
It prints system information.
'-a'(all) prints all available system info
 < Output >

# ........

Q19. umask
 < Answer >
- It specifies user file creation mask, implying which of the 3 permissions are to be denied 
to the different classes of users (owner, group and others).
- The bits which are set in the umask value, refer to the permissions, which are not assigned 
by default, as these values are subtracted from the maximum permission for files/directories.
- File -> The full permission set is 666 (read/write permission for all)
- Directory -> The full permission set is 777 (read/write/execute)
- The three -rwx- permissions (Read-Write-Execute) values are converted into three-bit binary values and 
represented by a single octal value.
- 0 (no permission), 1 (execute), 2 (write), 3 (write and execute), 
4 (read), 5 (read and execute), 6 (read and write), 7 (permission to do all three)
- The permissions will be calculated as (full permissions) – (umask value)
 < Output >
0022

# ........

Q20. find
 < Answer >
It searches for files in a directory hierarchy
 < Syntax >
$find [path] [expression]
-name <file_name> : searches for all files with this name
-type [f|d]: finds all files or directories 

# ........

Q21. sort
 < Answer >
It sorts the lines of text files
-r: reverse order
-n: numerically
 < Output >

# ........

Q22. ps
 < Answer >
It displays information about the current processes.
 < Output >

# ........

Q23. chmod 777 file1
 < Answer >
Gives full permission to owner, group and others
Use '$ls -l file1' to check
 < Output >

# ........

Q24. chmod o-w file1
 < Answer >
Removes write permission for 'o' ie. others.
owner (u), group (g), others (o)
Use '$ls -l file1' to check
 < Output >

# ........