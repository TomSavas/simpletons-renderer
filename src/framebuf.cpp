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

    CalculateViewportMatrix();
}

Framebuf::~Framebuf() {
    if ((intptr_t) fb != -1) {
        printf("Unmaping fb\n");
        munmap(fb, size_in_bytes);
    }

    if (fd != -1) {
        printf("Closing fd\n");
        close(fd);
    }
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

    size_in_bytes = var_info.xres * var_info.yres * var_info.bits_per_pixel / 8;
}

void Framebuf::MmapFb() {
    fb = (char*) mmap(0, size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if ((intptr_t) fb == -1)
        throw std::runtime_error(strerror(errno));
}

void Framebuf::CalculateViewportMatrix() {
    // Translate from unit cube into viewport
    Mat4f translation = Mat4f::Identity();
    translation[0][3] = (Width() - 1) / 2.0;
    translation[1][3] = (Height() - 1) / 2.0;
    translation[2][3] = 0.5;

    // Scale to viewport dimensions
    Mat4f scaling = Mat4f::Identity();
    scaling[0][0] = (Width() - 1) / 2.0;
    scaling[1][1] = (Height() - 1) / 2.0;
    scaling[2][2] = 0.5;

    viewport_matrix = translation * scaling;
}

int Framebuf::Index(int x, int y) const {
    return (x + var_info.xoffset) * (var_info.bits_per_pixel / 8) +
           (y + var_info.yoffset) * fix_info.line_length;
}

int Framebuf::Width() const {
    return var_info.xres;
}

int Framebuf::Height() const {
    return var_info.yres;
}

int Framebuf::Size() const {
    return Width() * Height();
}

const Mat4f &Framebuf::ViewportMatrix() const {
    return viewport_matrix;
}

void Framebuf::Set(int x, int y, Color color) {
    if (x < 0 || y < 0 || x >= var_info.xres || y >= var_info.yres) {
        fprintf(stderr, "Invalid write to fb at x: %d, y: %d\n", x, y);
        return;
    }

    if (flip_y) {
        y = var_info.yres - 1 - y;
    }

    int index = Index(x, y);

    // Linux framebuffer is in BGR format for some reason
    operator[](index) = color.b;
    operator[](index + 1) = color.g;
    operator[](index + 2) = color.r;
    operator[](index + 3) = color.a;
}

void Framebuf::Clear(Color color) {
    for (int y = 0; y < Height(); y++) 
        for (int x = 0; x < Width(); x++)
            Set(x, y, color);
}

void Framebuf::FlipY(bool flip) {
    flip_y = flip;
}

char& Framebuf::operator[](int index) {
    return fb[index];
}