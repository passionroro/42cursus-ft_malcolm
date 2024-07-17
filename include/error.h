#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>

int	handle_error(const char *message);

/* most frequent errno */
/*
EINVAL: Invalid argument
EACCES: Permission denied
ENOENT: No such file or directory
ENOMEM: Out of memory
EEXIST: File exists
EAGAIN: Resource temporarily unavailable
EBUSY: Device or resource busy
EPERM: Operation not permitted
ERANGE: Result too large
ENOSPC: No space left on device
EINTR: Interrupted system call
EBADF: Bad file descriptor
EFAULT: Bad address
EIO: Input/output error
ENODEV: No such device
EISDIR: Is a directory
ENOTDIR: Not a directory
EMFILE: Too many open files
EOVERFLOW: Value too large for defined data type
*/
