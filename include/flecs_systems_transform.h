#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include <flecs-systems-transform/bake_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsSystemsTransform {
   int dummy;
} FlecsSystemsTransform;

FLECS_SYSTEMS_TRANSFORM_API
void FlecsSystemsTransformImport(
    ecs_world_t *world);

#define FlecsSystemsTransformImportHandles(handles)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace systems {

class transform : FlecsSystemsTransform {
public:
    transform(flecs::world& ecs) {
        FlecsSystemsTransformImport(ecs.c_ptr());

        ecs.module<flecs::systems::transform>();
    }
};

}
}

#endif
#endif

#endif
