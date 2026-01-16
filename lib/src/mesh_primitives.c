#include <mesh_primitives.h>

static Mesh upload_mesh(const MeshCPU *const m) {
    Mesh res = {0};
}

Mesh primitive_plane(vec2 size) {
    MeshCPU m = {0};
    meshcpu_reserve(&m, 4, 6);

    float h = size[0], w = size[1];

    meshcpu_push_vertex(&m, (vec3) {-w, +h, 0.f}, (vec3) {0.f, 0.f, 1.f}, (vec2) {0.f, 1.f});
    meshcpu_push_vertex(&m, (vec3) {+w, +h, 0.f}, (vec3) {0.f, 0.f, 1.f}, (vec2) {1.f, 1.f});
    meshcpu_push_vertex(&m, (vec3) {-w, -h, 0.f}, (vec3) {0.f, 0.f, 1.f}, (vec2) {0.f, 0.f});
    meshcpu_push_vertex(&m, (vec3) {+w, -h, 0.f}, (vec3) {0.f, 0.f, 1.f}, (vec2) {1.f, 0.f});

    meshcpu_push_index(&m, 0);
    meshcpu_push_index(&m, 1);
    meshcpu_push_index(&m, 2);
    meshcpu_push_index(&m, 0);
    meshcpu_push_index(&m, 2);
    meshcpu_push_index(&m, 3);

    Mesh m_res = upload_mesh(&m);
    meshcpu_free(&m);
    return m_res;
}

Mesh primitive_cube(float s) {
    Mesh m = {0};

    return m;
}
