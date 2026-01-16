#ifndef MESH_H
#define MESH_H

#include <cglm/cglm.h>
#include <glad/glad.h>

typedef struct {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} mesh_Vertex;

typedef struct {
    mesh_Vertex *vertices;
    size_t       vertexCount;
    size_t       vertexCapacity;
    GLuint      *indices;
    size_t       indexCount;
    size_t       indexCapacity;
} MeshCPU;

void meshcpu_free(MeshCPU *const m);
int  meshcpu_reserve(MeshCPU *const m, size_t vertexCount, size_t indexCount);
int  meshcpu_push_vertex(MeshCPU *const m, vec3 pos, vec3 normal, vec2 uv);
int  meshcpu_pop_vertex(MeshCPU *const m);
int  meshcpu_push_index(MeshCPU *const m, GLuint i);
int  meshcpu_pop_index(MeshCPU *const m);

typedef struct {
    GLuint   vao;
    GLuint   vbo;
    GLuint   ibo;
    uint32_t indexCount;
} Mesh;

#endif
