#include <vulkan_context.h>
#include <window.h>

int main() {
    glfwInit();

    Window w = {0};
    create_window(&w, "Test window", 800, 800, WF_VISIBLE);

    RendererState rs;
    renderer_init(&rs, &w);

    while (!glfwWindowShouldClose(w.handle)) { glfwPollEvents(); }

    renderer_terminate(&rs);
    glfwDestroyWindow(w.handle);
    glfwTerminate();

    return 0;
}
