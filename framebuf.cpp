#include <fcntl.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "framebuf.h"

Framebuf::Framebuf() {
    LoadFd();
    LoadInfo();
    MmapFb();
}

Framebuf::~Framebuf() {
    if ((intptr_t) fb != -1) {
        printf("Unmaping fb\n");
        munmap(fb, size);
    }

    if (fd != -1) {
        printf("Closing fd\n");
        close(fd);
    }
}

int Framebuf::Index(int x, int y) const {
    return (x + var_info.xoffset) * (var_info.bits_per_pixel / 8) +
           (y + var_info.yoffset) * fix_info.line_length;
}

char *&Framebuf::Fb() {
    return fb;
}

void Framebuf::Set(int x, int y, Color color) {
    if (flip_y) {
        y = var_info.yres - y;
    }

    int index = Index(x, y);

    // Linux framebuffer is in BGR format for some reason
    operator[](index) = color.b;
    operator[](index + 1) = color.g;
    operator[](index + 2) = color.r;
    operator[](index + 3) = color.a;
}

void Framebuf::FlipY(bool flip) {
    flip_y = flip;
}

char& Framebuf::operator[](int index) {
    return fb[index];
}

void Framebuf::LoadFd() {
   fd = open("/dev/fb0", O_RDWR);

   if (fd == -1)
       throw std::runtime_error(strerror(errno));
}

void Framebuf::LoadInfo() {
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix_info) == -1)
        throw std::runtime_error(strerror(errno));
    
    if (ioctl(fd, FBIOGET_VSCREENINFO, &var_info) == -1)
        throw std::runtime_error(strerror(errno));

    size = var_info.xres * var_info.yres * var_info.bits_per_pixel / 8;
}

void Framebuf::MmapFb() {
    fb = (char*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if ((intptr_t) fb == -1)
        throw std::runtime_error(strerror(errno));
}
