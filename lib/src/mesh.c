#include <mesh.h>
#include <stdlib.h>
#include <string.h>

void meshcpu_free(MeshCPU *const m) {
    if (m->vertices) { free(m->vertices); }
    m->vertices       = NULL;
    m->vertexCount    = 0;
    m->vertexCapacity = 0;

    if (m->indices) { free(m->indices); }
    m->indices       = NULL;
    m->indexCount    = 0;
    m->indexCapacity = 0;
}

static int meshcpu_grow_vert(MeshCPU *const m, size_t newCount) {
    if (newCount > m->vertexCapacity) {
        mesh_Vertex *buffer = malloc(newCount * sizeof(mesh_Vertex));
        if (!buffer) { return -1; }
        if (m->vertices) {
            memcpy(buffer, m->vertices, m->vertexCount * sizeof(mesh_Vertex));
            free(m->vertices);
        }
        m->vertices = buffer;
    }

    return 0;
}

static int meshcpu_grow_ind(MeshCPU *const m, size_t newCount) {
    if (newCount > m->indexCount) {
        GLuint *buffer = malloc(newCount * sizeof(GLuint));
        if (!buffer) { return -1; }
        if (m->indices) {
            memcpy(buffer, m->indices, m->indexCount * sizeof(GLuint));
            free(m->indices);
        }
        m->indices = buffer;
    }

    return 0;
}

int meshcpu_reserve(MeshCPU *const m, size_t vertexCount, size_t indexCount) {
    int res = 0;

    res = meshcpu_grow_vert(m, vertexCount);
    if (res != 0) { return res; }

    res = meshcpu_grow_ind(m, indexCount);
    if (res != 0) { return res; }

    return res;
}

int meshcpu_push_vertex(MeshCPU *const m, vec3 pos, vec3 normal, vec2 uv) {
    if (m->vertexCount + 1 >= m->vertexCapacity) { meshcpu_grow_vert(m, m->vertexCapacity * 2); }
    mesh_Vertex *v = &m->vertices[m->vertexCount++];
    memcpy(v->pos, pos, sizeof(vec3));
    memcpy(v->normal, normal, sizeof(vec3));
    memcpy(v->uv, uv, sizeof(vec2));

    return 0;
}

int meshcpu_pop_vertex(MeshCPU *const m) {
    m->vertexCount--;
    return 0;
}

int meshcpu_push_index(MeshCPU *const m, GLuint i) {
    if (m->indexCount + 1 >= m->vertexCapacity) { meshcpu_grow_ind(m, m->indexCapacity * 2); }
    m->indices[m->indexCount++] = i;
    return 0;
}

int meshcpu_pop_index(MeshCPU *const m) {
    m->indexCount--;
    return 0;
}
