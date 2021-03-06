#ifndef SYSCALL_IPC_H_
#define SYSCALL_IPC_H_

#include <sys/types.h>
#include <mach/message.h>


/*
 * This file defines Darwin specific system calls (i.e. not POSIX).
 */
namespace Syscall {

    SYSTEM_CALL mach_port_t         mach_reply_port();
    SYSTEM_CALL mach_port_t         mach_thread_self();
    SYSTEM_CALL mach_port_t         mach_task_self();
    SYSTEM_CALL mach_port_t         mach_host_self();

    // TODO: Cannot handle more than 6 arguments currently.
    SYSTEM_CALL mach_msg_return_t   mach_msg(mach_msg_header_t *,
                                             mach_msg_options_t,
                                             mach_msg_size_t,
                                             mach_msg_size_t,
                                             mach_port_name_t,
                                             mach_msg_timeout_t,
                                             mach_port_name_t);

    #if SYSCALL_EXISTS(shmsys)
    SYSTEM_CALL int                 shmsys(int, int, int, int);
    SYSTEM_CALL int                 shmsys(int, int, const void *, int);
    SYSTEM_CALL int                 shmsys(int, const void *);
    SYSTEM_CALL int                 shmsys(int, int, int, struct shmid_ds *);
    #endif

    SYSTEM_CALL
    mach_port_t mach_reply_port()
    {
        return DO_MACH_SYSCALL(mach_reply_port);
    }

    SYSTEM_CALL
    mach_port_t mach_thread_self()
    {
        return DO_MACH_SYSCALL(thread_self_trap);
    }

    SYSTEM_CALL
    mach_port_t mach_task_self()
    {
        return DO_MACH_SYSCALL(task_self_trap);
    }

    SYSTEM_CALL
    mach_port_t mach_host_self()
    {
        return DO_MACH_SYSCALL(host_self_trap);
    }

    //
    // Common entry point for shared memory operations.
    //
    #if SYSCALL_EXISTS(shmsys)

        SYSTEM_CALL
        int shmsys(int which, int a2, int a3, int a4)
        {
            return DO_SYSCALL(shmsys, which, a2, a3, a4);
        }

        SYSTEM_CALL
        int shmsys(int which, int a2, const void *a3, int a4)
        {
            return DO_SYSCALL(shmsys, which, a2, a3, a4);
        }

        SYSTEM_CALL
        int shmsys(int which, const void *a2)
        {
            return DO_SYSCALL(shmsys, which, a2);
        }

        SYSTEM_CALL
        int shmsys(int which, int a2, int a3, struct shmid_ds *buf)
        {
            return DO_SYSCALL(shmsys, which, a2, a3, buf);
        }

        #if !SYSCALL_EXISTS(shmget)
        SYSTEM_CALL
        int shmget(key_t key, size_t size, int shmflg)
        {
            return shmsys(3, key, size, shmflg);
        }
        #endif

        #if !SYSCALL_EXISTS(shmat)
        SYSTEM_CALL
        void *shmat(int shmid, const void *shmaddr, int shmflg)
        {
            return (void *) shmsys(0, shmid, smdaddr, shmflg);
        }
        #endif

        #if !SYSCALL_EXISTS(shmdt)
        SYSTEM_CALL
        int shmdt(const void *shmaddr)
        {
            return shmsys(2, shmaddr);
        }
        #endif

        #if !SYSCALL_EXISTS(shmctl)
        SYSTEM_CALL
        int shmctl(int shmid, int cmd, struct shmid_ds *buf)
        {
            return shmsys(4, shmid ,cmd, buf);
        }
        #endif

    #endif // SYSCALL_EXISTS(shmsys)
}

#endif
