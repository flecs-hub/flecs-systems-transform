#include <flecs_systems_transform.h>

void EcsAddMatTransform2D(ecs_rows_t *rows) {
    ecs_world_t *world = rows->world;
    ecs_type_t TEcsMatTransform2D = ecs_column_type(rows, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_set_ptr(world, rows->entities[i], EcsMatTransform2D, &ECS_MAT3X3_IDENTITY);
    }
}

void EcsApplyTransform2D(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsMatTransform2D, m, 1);
    ECS_COLUMN(rows, EcsPosition2D, p, 2);
    ECS_COLUMN(rows, EcsRotation2D, r, 3);
    ECS_COLUMN(rows, EcsScale2D, s, 4);
    ECS_COLUMN(rows, EcsMatTransform2D, m_parent, 5);
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

    if (!ecs_is_shared(rows, 2)) {
        ecs_mat3x3_addn_translation(m, p, rows->count);
    } else {
        for (i = 0; i < rows->count; i ++) {
            ecs_mat3x3_add_translation(&m[i], p);
        }        
    }

    if (r) {
        ecs_mat3x3_addn_rotation(m, (float*)r, rows->count);
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
        !OWNED.EcsMatTransform2D, 
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
