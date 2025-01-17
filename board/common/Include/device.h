#ifndef _SMART_PLATFORM_DEVICE_H
#define _SMART_PLATFORM_DEVICE_H

struct devopt {
    void (*open)(void);
    void (*close)(void);
    int (*read)(char *buf, int len);
    int (*write)(char *buf, int len);
    int (*lseek)(int ptr, int dir);
    int (*ioctl)(int request, void *args);
};

struct device {
    int id;
    struct devopt opt;
};

#define DEVICE_DEFINE(DEV, ID)                                      \
    static void DEV##_open(void);                                   \
    static void DEV##_close(void);                                  \
    static int DEV##_write(char *buf, int len);                     \
    static int DEV##_read(char *buf, int len);                      \
    static int DEV##_lseek(int ptr, int dir);                       \
    static int DEV##_ioctl(int request, void *args);                \
    static struct device DEV = {                                    \
      .id = ID,                                                     \
      .opt.open = DEV##_open,                                       \
      .opt.close = DEV##_close,                                     \
      .opt.read = DEV##_read,                                       \
      .opt.write = DEV##_write,                                     \
      .opt.lseek = DEV##_lseek,                                     \
      .opt.ioctl = DEV##_ioctl,                                     \
    };                                                              \
    static const struct device *DEV##_dev __attribute__((__used__)) \
    __attribute__((section(".device"))) = &DEV


#define DEVICE_FUNC_DEFINE_OPEN(DEV) \
    static void DEV##_open(void)
#define DEVICE_FUNC_DEFINE_CLOSE(DEV) \
    static void DEV##_close(void)
#define DEVICE_FUNC_DEFINE_WRITE(DEV) \
    static int DEV##_write(char *buf, int len)
#define DEVICE_FUNC_DEFINE_READ(DEV) \
    static int DEV##_read(char *buf, int len)
#define DEVICE_FUNC_DEFINE_LSEEK(DEV) \
    static int DEV##_lseek(int ptr, int dir)
#define DEVICE_FUNC_DEFINE_IOCTL(DEV) \
    static int DEV##_ioctl(int request, void *args)

extern void device_open(void);

extern void device_close(void);

#endif