# pthread mutex in shared library
## shm

* [shmget()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmget.2.html)
* [shmat()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmop.2.html)
* [shmdt()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmop.2.html)
* [shmctl()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/shmctl.2.html)

## Run

```
nabeshin@iMacNabeshin test2 % make      
gcc -shared -o libtest.so libtest.c
gcc -L./ -o main.out main.c -ltest
nabeshin@iMacNabeshin test2 % ./main.out
in_init called.
[parent]: [lib] called(1) #################### 
[parent]: before pthread_mutex_lock()
[parent]: press enter to unlock...

        [child]: [lib] called(1) #################### 
        [child]: before pthread_mutex_lock()

[parent]: after pthread_mutex_unlock() ==== 
[parent]: [lib] end    #################### 
[parent]: [lib] called(2) #################### 
        [child]: press enter to unlock...
[parent]: before pthread_mutex_lock()


        [child]: after pthread_mutex_unlock() ==== 
        [child]: [lib] end    #################### 
        [child]: [lib] called(2) #################### 
[parent]: press enter to unlock...
        [child]: before pthread_mutex_lock()


[parent]: after pthread_mutex_unlock() ==== 
[parent]: [lib] end    #################### 
[parent]: waiting...
        [child]: press enter to unlock...


        [child]: after pthread_mutex_unlock() ==== 
        [child]: [lib] end    #################### 
[parent]: after wait()
terminate called.
nabeshin@iMacNabeshin test2 % 
```