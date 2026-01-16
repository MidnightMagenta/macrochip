#include <assert.h>
#include <ecs_component.h>
#include <ecs_core.h>

#define ECS_COMPONENT_MAX 64

// entity ID trackers
static ecs_EntityID freeIDs[ECS_MAX_ENTITIES];
static size_t       freeCount = ECS_MAX_ENTITIES;

// alive entity tracker
// index into this array == entityID of the entity
// 0 = not alive
// 1 = alive
static ecs_EntityID alive[ECS_MAX_ENTITIES];

// component trackers
static ecs_ComponentType componentTypes[ECS_COMPONENT_MAX];
static size_t            componentCount = 0;

// initialie the freeIDs array with valid entity id's (values between 0 and ECS_MAX_ENTITIES)
int ecs_init() {
    for (size_t i = 0; i < ECS_MAX_ENTITIES; ++i) {
        freeIDs[i] = i;
        alive[i]   = 0;
    }
    return 0;
}

void ecs_destroy() {
    for (ecs_EntityID i = 0; i < ECS_MAX_ENTITIES; ++i) { ecs_entity_destroy(i); }
}

int ecs_component_register(void *store, ecs_ComponentRemoveFn removeFn) {
    assert(store != NULL);
    assert(removeFn != NULL);
    if (componentCount >= ECS_COMPONENT_MAX) { return 1; }

    componentTypes[componentCount++] = (ecs_ComponentType) {.store = store, .removeFn = removeFn};
    return 0;
}

ecs_EntityID ecs_entity_create() {
    if (freeCount == 0) { return ECS_ENTITY_ID_INVALID; }

    ecs_EntityID e = freeIDs[--freeCount];
    alive[e]       = 1;
    return e;
}

void ecs_entity_destroy(ecs_EntityID e) {
    if (!alive[e]) { return; }

    alive[e] = 0;

    for (size_t i = 0; i < componentCount; ++i) {
        componentTypes[i].removeFn(componentTypes[i].store, e);
    }

    freeIDs[freeCount++] = e;
}
