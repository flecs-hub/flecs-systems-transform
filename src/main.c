#include <include/transform.h>

void EcsAddMatTransform2D(ecs_rows_t *rows) {
    ecs_world_t *world = rows->world;
    ecs_type_t TEcsMatTransform2D = ecs_column_type(rows, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_set_ptr(world, rows->entities[i], EcsMatTransform2D, &ECS_MAT3X3_IDENTITY);
    }
}

void EcsApplyTransform2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_column(rows, EcsMatTransform2D, 1);
    EcsPosition2D *p = ecs_column(rows, EcsPosition2D, 2);
    EcsRotation2D *r = ecs_column_test(rows, EcsRotation2D, 3);
    EcsScale2D *s = ecs_column_test(rows, EcsScale2D, 4);
    EcsMatTransform2D *m_parent = ecs_shared_test(rows, EcsMatTransform2D, 5);
    int i;

    if (!m_parent) {
        for (i = 0; i < rows->count; i ++) {
            m[i] = ECS_MAT3X3_IDENTITY;
        }
    } else {
        for (i = 0; i < rows->count; i ++) {
            m[i] = *m_parent;
        }
    }

    for (i = 0; i < rows->count; i ++) {
        ecs_mat3x3_add_translation(&m[i], &p[i]);
    }

    if (r) {
        for (i = 0; i < rows->count; i ++) {
            ecs_mat3x3_add_rotation(&m[i], r[i].angle);
        }
    }

    if (s) {
        for (i = 0; i < rows->count; i ++) {
            ecs_mat3x3_add_scale(&m[i], &s[i]);
        }
    }
}

void FlecsSystemsTransformImport(
    ecs_world_t *world,
    int flags)
{
    ECS_IMPORT(world, FlecsComponentsTransform, flags);
    ECS_MODULE(world, FlecsSystemsTransform);

    /* System that adds transform matrix to every entity with transformations */
    ECS_SYSTEM(world, EcsAddMatTransform2D, EcsOnLoad, 
        EcsPosition2D | EcsRotation2D | EcsScale2D, 
        !EcsMatTransform2D, 
        SYSTEM.EcsHidden);

    ECS_SYSTEM(world, EcsApplyTransform2D, EcsOnValidate, 
        EcsMatTransform2D,
        EcsPosition2D,
        ?EcsRotation2D,
        ?EcsScale2D,
        CASCADE.EcsMatTransform2D,
        SYSTEM.EcsHidden);

    ECS_TYPE(world, EcsTransform2D,  
        EcsAddMatTransform2D, 
        EcsApplyTransform2D);

    ECS_SET_ENTITY(handles, EcsTransform2D);
}
