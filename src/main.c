#include <include/transform.h>

void EcsAddMatTransform2D(ecs_rows_t *rows) {
    ecs_world_t *world = rows->world;
    EcsType TEcsMatTransform2D = ecs_column_type(rows, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_set_ptr(world, rows->entities[i], EcsMatTransform2D, &ECS_MAT3X3_IDENTITY);
    }
}

void EcsInitTransformChildren2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_shared(rows, EcsMatTransform2D, 1);
    EcsMatTransform2D *m_container = ecs_column(rows, EcsMatTransform2D, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        m[i] = *m_container;
    }
}

void EcsApplyTranslation2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_column(rows, EcsMatTransform2D, 1);
    EcsPosition2D *p = ecs_column(rows, EcsPosition2D, 2);
    
    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_mat3x3_add_translation(&m[i], &p[i]);
    }
}

void EcsApplyRotation2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_column(rows, EcsMatTransform2D, 1);
    EcsRotation2D *r = ecs_column(rows, EcsRotation2D, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_mat3x3_add_rotation(&m[i], r[i].angle);
    }
}

void EcsApplyScaling2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_column(rows, EcsMatTransform2D, 1);
    EcsScale2D *s = ecs_column(rows, EcsScale2D, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_mat3x3_add_scale(&m[i], &s[i]);
    }
}

void EcsResetTransform2D(ecs_rows_t *rows) {
    EcsMatTransform2D *m = ecs_column(rows, EcsMatTransform2D, 1);

    int i;
    for (i = 0; i < rows->count; i ++) {
        m[i] = ECS_MAT3X3_IDENTITY;
    }
}

void EcsSystemsTransform(
    ecs_world_t *world,
    int flags,
    void *handles_out)
{
    EcsSystemsTransformHandles *handles = handles_out;

    ECS_IMPORT(world, EcsComponentsTransform, flags);

    /* System that adds transform matrix to every entity with transformations */
    ECS_SYSTEM(world, EcsAddMatTransform2D, EcsOnLoad, EcsPosition2D | EcsRotation2D | EcsScale2D, !EcsMatTransform2D, SYSTEM.EcsHidden);

    /* Systems that add transformations to transform matrix */
    ECS_SYSTEM(world, EcsApplyTranslation2D, EcsPostFrame, EcsMatTransform2D, EcsPosition2D, SYSTEM.EcsHidden);
    ECS_SYSTEM(world, EcsApplyRotation2D, EcsPostFrame, EcsMatTransform2D, EcsRotation2D, SYSTEM.EcsHidden);
    ECS_SYSTEM(world, EcsApplyScaling2D, EcsPostFrame, EcsMatTransform2D, EcsScale2D, SYSTEM.EcsHidden);

    /* Copy transformation from parent to child entities */
    ECS_SYSTEM(world, EcsInitTransformChildren2D, EcsPreFrame, EcsMatTransform2D, CONTAINER.EcsMatTransform2D, SYSTEM.EcsHidden);

    /* Reset transformation */
    ECS_SYSTEM(world, EcsResetTransform2D, EcsPreFrame, EcsMatTransform2D, SYSTEM.EcsHidden);

    ECS_TYPE(world, EcsTransform2D, 
        EcsApplyTranslation2D, 
        EcsApplyRotation2D, 
        EcsApplyScaling2D, 
        EcsInitTransformChildren2D, 
        EcsResetTransform2D);

    ECS_SET_COMPONENT(handles, EcsTransform2D);
}
