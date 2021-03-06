

#include "pygame.h"
#include "doc/camera_doc.h"

#if defined(__unix__)
    #include <structmember.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>

    #include <fcntl.h>              
    #include <unistd.h>
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <sys/mman.h>
    #include <sys/ioctl.h>

   
    #ifdef linux
        #include <asm/types.h>          
    #endif

    #include <linux/videodev2.h>
#elif defined(__APPLE__)
    #include <AvailabilityMacros.h>
    #if __MAC_OS_X_VERSION_MAX_ALLOWED <= 1060
        #define PYGAME_MAC_CAMERA_OLD 1
    #endif
#endif

#if defined(PYGAME_MAC_CAMERA_OLD)
        #include <QuickTime/QuickTime.h>
        #include <QuickTime/Movies.h>
        #include <QuickTime/ImageCompression.h>
#endif

#ifndef V4L2_PIX_FMT_RGB24
    #define V4L2_PIX_FMT_RGB24 'RGB3'
#endif
#ifndef V4L2_PIX_FMT_RGB444
    #define V4L2_PIX_FMT_RGB444 'R444'
#endif
#ifndef V4L2_PIX_FMT_YUYV
    #define V4L2_PIX_FMT_YUYV 'YUYV'
#endif

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define SAT(c) if (c & (~255)) { if (c < 0) c = 0; else c = 255; }
#define SAT2(c) ((c) & (~255) ? ((c) < 0 ? 0 : 255) : (c))
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define RGB_OUT 1
#define YUV_OUT 2
#define HSV_OUT 4
#define CAM_V4L 1 
#define CAM_V4L2 2

struct buffer {
    void * start;
    size_t length;
};

#if defined(__unix__)
typedef struct pgCameraObject {
    PyObject_HEAD
    char* device_name;
    int camera_type;
    unsigned long pixelformat;
    unsigned int color_out;
    struct buffer* buffers;
    unsigned int n_buffers;
    int width;
    int height;
    int size;
    int hflip;
    int vflip;
    int brightness;
    int fd;
} pgCameraObject;
#elif defined(PYGAME_MAC_CAMERA_OLD)
typedef struct pgCameraObject {
    PyObject_HEAD
    char* device_name;              
    OSType pixelformat;
    unsigned int color_out;
    SeqGrabComponent component;     
    SGChannel channel;              
    GWorldPtr gworld;               
    Rect boundsRect;                
    long size;                     
    int hflip;
    int vflip;
    short depth;
    struct buffer pixels;
} pgCameraObject;

#else


typedef struct pgCameraObject {
    PyObject_HEAD
    char* device_name;
    int camera_type;
    unsigned long pixelformat;
    unsigned int color_out;
    struct buffer* buffers;
    unsigned int n_buffers;
    int width;
    int height;
    int size;
    int hflip;
    int vflip;
    int brightness;
    int fd;
} pgCameraObject;
#endif

void colorspace (SDL_Surface *src, SDL_Surface *dst, int cspace);
void rgb24_to_rgb (const void* src, void* dst, int length, SDL_PixelFormat* format);
void rgb444_to_rgb (const void* src, void* dst, int length, SDL_PixelFormat* format);
void rgb_to_yuv (const void* src, void* dst, int length,
                 unsigned long source, SDL_PixelFormat* format);
void rgb_to_hsv (const void* src, void* dst, int length,
                 unsigned long source, SDL_PixelFormat* format);
void yuyv_to_rgb (const void* src, void* dst, int length, SDL_PixelFormat* format);
void yuyv_to_yuv (const void* src, void* dst, int length, SDL_PixelFormat* format);
void uyvy_to_rgb (const void* src, void* dst, int length, SDL_PixelFormat* format);
void uyvy_to_yuv (const void* src, void* dst, int length, SDL_PixelFormat* format);
void sbggr8_to_rgb (const void* src, void* dst, int width, int height,
                    SDL_PixelFormat* format);
void yuv420_to_rgb (const void* src, void* dst, int width, int height,
                    SDL_PixelFormat* format);
void yuv420_to_yuv (const void* src, void* dst, int width, int height,
                    SDL_PixelFormat* format);

#if defined(__unix__)
char** v4l2_list_cameras (int* num_devices);
int v4l2_get_control (int fd, int id, int *value);
int v4l2_set_control (int fd, int id, int value);
PyObject* v4l2_read_raw (pgCameraObject* self);
int v4l2_xioctl (int fd, int request, void *arg);
int v4l2_process_image (pgCameraObject* self, const void *image,
                               unsigned int buffer_size, SDL_Surface* surf);
int v4l2_query_buffer (pgCameraObject* self);
int v4l2_read_frame (pgCameraObject* self, SDL_Surface* surf);
int v4l2_stop_capturing (pgCameraObject* self);
int v4l2_start_capturing (pgCameraObject* self);
int v4l2_uninit_device (pgCameraObject* self);
int v4l2_init_mmap (pgCameraObject* self);
int v4l2_init_device (pgCameraObject* self);
int v4l2_close_device (pgCameraObject* self);
int v4l2_open_device (pgCameraObject* self);

#elif defined(PYGAME_MAC_CAMERA_OLD)
char** mac_list_cameras(int* num_devices);
int mac_open_device (pgCameraObject* self);
int mac_init_device(pgCameraObject* self);
int mac_close_device (pgCameraObject* self);
int mac_start_capturing(pgCameraObject* self);
int mac_stop_capturing (pgCameraObject* self);

int mac_get_control(pgCameraObject* self, int id, int* value);
int mac_set_control(pgCameraObject* self, int id, int value);

PyObject* mac_read_raw(pgCameraObject *self);
int mac_read_frame(pgCameraObject* self, SDL_Surface* surf);
int mac_camera_idle(pgCameraObject* self);
int mac_copy_gworld_to_surface(pgCameraObject* self, SDL_Surface* surf);

void flip_image(const void* image, void* flipped_image, int width, int height,
                short depth, int hflip, int vflip);

#endif
