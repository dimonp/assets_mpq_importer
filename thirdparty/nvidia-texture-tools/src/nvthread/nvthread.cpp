// This code is in the public domain -- Ignacio Castaño <castano@gmail.com>

#include <thread>

#include "nvthread.h"

#if NV_OS_WIN32
#include "Win32.h"
#elif NV_OS_UNIX
#include <sys/types.h>
#if !NV_OS_LINUX
#include <sys/sysctl.h>
#endif
#include <unistd.h>
#elif NV_OS_DARWIN
#import <stdio.h>
#import <string.h>
#import <mach/mach_host.h>
#import <sys/sysctl.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#endif

#include "nvcore/Debug.h"

using namespace nv;

uint nv::processorCount() {
    return std::thread::hardware_concurrency();
}
