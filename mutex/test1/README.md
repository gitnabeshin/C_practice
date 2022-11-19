# pthread mutex 
## shm

* [shmget()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmget.2.html)
* [shmat()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmop.2.html)
* [shmdt()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmop.2.html)
* [shmctl()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmctl.2.html)

## Run

```
nabeshin@iMacNabeshin test1 % gcc main.c
nabeshin@iMacNabeshin test1 % ./a.out   
[parent] before pthread_mutex_lock()
    [parent] press enter to unlock...
[child] before pthread_mutex_lock()

[parent] after pthread_mutex_unlock()
    [child] press enter to unlock...
    [parent] waiting...

[child] after pthread_mutex_unlock()
[parent] after wait()
nabeshin@iMacNabeshin test1 % 
```