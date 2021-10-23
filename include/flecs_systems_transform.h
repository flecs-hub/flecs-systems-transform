#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

#include <flecs-systems-transform/bake_config.h>

#ifdef __cplusplus
extern "C" {
#endif

FLECS_SYSTEMS_TRANSFORM_API
void FlecsSystemsTransformImport(
    ecs_world_t *world);

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
        FlecsSystemsTransformImport(ecs);
        ecs.module<flecs::systems::transform>();
    }
};

}
}

#endif // FLECS_NO_CPP
#endif // __cplusplus

#endif
