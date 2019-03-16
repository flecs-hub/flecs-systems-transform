#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EcsSystemsTransformHandles {
   ECS_DECLARE_COMPONENT(EcsTransform2D);
} EcsSystemsTransformHandles;

void EcsSystemsTransform(
    EcsWorld *world,
    int flags,
    void *handles_out);

#define EcsSystemsTransform_ImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsTransform2D);

#endif
