#include <window.h>

int create_window(Window *const window, const char *title, int w, int h, int windowFlags) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, (windowFlags & WF_RESIZABLE) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, (windowFlags & WF_VISIBLE) ? GLFW_TRUE : GLFW_FALSE);

    window->handle = glfwCreateWindow(w, h, title, NULL, NULL);
    if (window->handle == NULL) { return -1; }
    return 0;
}