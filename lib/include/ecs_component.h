#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <ecs_core.h>

#define DEFINE_COMPONENT_INIT(t, n)                                                                \
    static inline void n##_init(n##Store *s) {                                                     \
        s->count = 0;                                                                              \
        for (size_t i = 0; i < ECS_MAX_ENTITIES; ++i) { s->index_of[i] = ECS_ENTITY_ID_INVALID; }  \
    }

#define DEFINE_COMPONENT_ADD(t, n)                                                                 \
    static inline void n##_add(n##Store *s, ecs_EntityID e, t v) {                                 \
        size_t idx       = s->count++;                                                             \
        s->data[idx]     = v;                                                                      \
        s->entities[idx] = e;                                                                      \
        s->index_of[e]   = (int32_t) idx;                                                          \
    }

#define DEFINE_COMPONENT_REMOVE(t, n)                                                              \
    static inline void n##_remove(void *store, ecs_EntityID e) {                                   \
        n##Store *s   = (n##Store *) store;                                                        \
        int32_t   idx = s->index_of[e];                                                            \
        if (idx == -1) return;                                                                     \
                                                                                                   \
        size_t last = --s->count;                                                                  \
        if ((size_t) idx != last) {                                                                \
            s->data[idx]                  = s->data[last];                                         \
            s->entities[idx]              = s->entities[last];                                     \
            s->index_of[s->entities[idx]] = idx;                                                   \
        }                                                                                          \
        s->index_of[e] = -1;                                                                       \
    }

#define DECLARE_COMPONENT_STORE(t, n)                                                              \
    typedef struct {                                                                               \
        t            data[ECS_MAX_ENTITIES];                                                       \
        ecs_EntityID entities[ECS_MAX_ENTITIES];                                                   \
        int32_t      index_of[ECS_MAX_ENTITIES];                                                   \
        size_t       count;                                                                        \
    } n##Store;                                                                                    \
    DEFINE_COMPONENT_INIT(t, n)                                                                    \
    DEFINE_COMPONENT_ADD(t, n)                                                                     \
    DEFINE_COMPONENT_REMOVE(t, n)

typedef void (*ecs_ComponentRemoveFn)(void *store, ecs_EntityID e);

typedef struct ecs_ComponentStruct {
    void                 *store;
    ecs_ComponentRemoveFn removeFn;
} ecs_ComponentType;

int ecs_component_register(void *store, ecs_ComponentRemoveFn removeFn);
#endif
