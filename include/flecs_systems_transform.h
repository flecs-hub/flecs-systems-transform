#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include <flecs-systems-transform/bake_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsSystemsTransform {
   int dummy;
} FlecsSystemsTransform;

FLECS_SYSTEMS_TRANSFORM_EXPORT
void FlecsSystemsTransformImport(
    ecs_world_t *world);

#define FlecsSystemsTransformImportHandles(handles)

#ifdef __cplusplus
}
#endif
#endif
