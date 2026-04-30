#ifndef LOG_H
#define LOG_H

void klog(const char *level, const char *msg);

#define KINFO(msg) klog("INFO", msg)
#define KWARN(msg) klog("WARN", msg)
#define KERR(msg) klog("ERROR", msg)
#define KDEBUG(msg) klog("DEBUG", msg)

#endif
