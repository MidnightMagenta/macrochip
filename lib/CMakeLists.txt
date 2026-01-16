find_package(Vulkan REQUIRED)

add_library("macrochip-lib" STATIC
    include/ecs_component.h
    include/ecs_core.h
    include/term.h
    include/utils.h
    include/vulkan_context.h
    include/window.h

    src/ecs_core.c
    src/vulkan_context.c
    src/window.c
)

target_include_directories("macrochip-lib" PUBLIC
    include
)
target_link_libraries("macrochip-lib" PUBLIC
    Vulkan::Vulkan
    submodules
)
