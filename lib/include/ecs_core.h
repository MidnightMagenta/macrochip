#ifndef ECS_CORE_H
#define ECS_CORE_H

#include <stddef.h>
#include <stdint.h>

#define ECS_ENTITY_ID_MAX     INT32_MAX
#define ECS_ENTITY_ID_INVALID -1
#define ECS_MAX_ENTITIES      1024 * 1024

typedef int32_t ecs_EntityID;

int          ecs_init();
void         ecs_destroy();
ecs_EntityID ecs_entity_create();
void         ecs_entity_destroy(ecs_EntityID id);

#endif
