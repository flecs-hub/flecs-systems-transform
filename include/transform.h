#ifndef REFLECS_SYSTEMS_TRANSFORM_H
#define REFLECS_SYSTEMS_TRANSFORM_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EcsSystemsTransformHandles {
   EcsEntity Transform2D;
} EcsSystemsTransformHandles;

void EcsSystemsTransform(
    EcsWorld *world,
    int flags,
    void *handles_out);

#define EcsSystemsTransform_DeclareHandles(handles)\
    EcsDeclareHandle(handles, Transform2D);

#endif
