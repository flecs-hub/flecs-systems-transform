#include <flecs_systems_transform.h>

void EcsAddTransform3(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    ecs_entity_t comp = ecs_column_entity(it, 1);

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_add_entity(world, it->entities[i], comp);
    }
}

void EcsApplyTransform3(ecs_iter_t *it) {
    EcsTransform3 *m = ecs_column(it, EcsTransform3, 1);
    EcsTransform3 *m_parent = ecs_column(it, EcsTransform3, 2);
    EcsPosition3 *p = ecs_column(it, EcsPosition3, 3);
    EcsRotation3 *r = ecs_column(it, EcsRotation3, 4);
    EcsScale3 *s = ecs_column(it, EcsScale3, 5);
    int i;

    if (!m_parent) {
        for (i = 0; i < it->count; i ++) {
            glm_translate_make(m[i], *(vec3*)&p[i]);
        }
    } else {
        for (i = 0; i < it->count; i ++) {
            glm_translate_to(*m_parent, *(vec3*)&p[i], m[i]);
        }
    }

    if (r) {
        for (i = 0; i < it->count; i ++) {
            glm_rotate(m[i], r[i].x, (vec3){1.0, 0.0, 0.0});
            glm_rotate(m[i], r[i].y, (vec3){0.0, 1.0, 0.0});
            glm_rotate(m[i], r[i].z, (vec3){0.0, 0.0, 1.0});
        }
    }

    if (s) {
        for (i = 0; i < it->count; i ++) {
            glm_scale(m[i], *(vec3*)&s[i]);
        }
    }
}

void FlecsSystemsTransformImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsSystemsTransform);

    ECS_IMPORT(world, FlecsComponentsTransform);

    /* System that adds transform matrix to every entity with transformations */
    ECS_SYSTEM(world, EcsAddTransform3, EcsPostLoad,
        !flecs.components.transform.Transform3,
        flecs.components.transform.Position3 || 
        flecs.components.transform.Rotation3 || 
        flecs.components.transform.Scale3,
        SYSTEM:EcsHidden);

    ECS_SYSTEM(world, EcsApplyTransform3, EcsOnValidate, 
        flecs.components.transform.Transform3,
        CASCADE:flecs.components.transform.Transform3,
        flecs.components.transform.Position3,
        ?flecs.components.transform.Rotation3,
        ?flecs.components.transform.Scale3,
        SYSTEM:EcsHidden);
}
