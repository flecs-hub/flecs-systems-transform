#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EcsSystemsTransformHandles {
   ECS_DECLARE_ENTITY(EcsTransform2D);
} EcsSystemsTransformHandles;

void EcsSystemsTransform(
    ecs_world_t *world,
    int flags,
    void *handles_out);

#define EcsSystemsTransform_ImportHandles(handles)\
    ECS_IMPORT_ENTITY(handles, EcsTransform2D);

#endif
