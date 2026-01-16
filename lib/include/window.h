#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow *handle;
} Window;

enum {
    WF_RESIZABLE = 1 << 0,
    WF_VISIBLE   = 1 << 2,
};

int create_window(Window *const window, const char *title, int w, int h, int windowFlags);

#endif