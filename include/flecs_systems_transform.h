#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include <flecs-systems-transform/bake_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsSystemsTransform {
   ECS_DECLARE_ENTITY(EcsTransform2D);
} FlecsSystemsTransform;

void FlecsSystemsTransformImport(
    ecs_world_t *world,
    int flags);

#define FlecsSystemsTransformImportHandles(handles)\
    ECS_IMPORT_ENTITY(handles, EcsTransform2D);

#endif
