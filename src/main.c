#include <include/transform.h>

void EcsAddMatTransform2D(EcsRows *rows) {
    EcsWorld *world = rows->world;
    EcsEntity EcsMatTransform2D_h = ecs_component(rows, 1);

    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsEntity entity = ecs_entity(rows, row, ECS_ROW_ENTITY);
        ecs_set_ptr(world, entity, EcsMatTransform2D_h, &ECS_MAT3X3_IDENTITY);
    }
}

void EcsInitTransformChildren2D(EcsRows *rows) {
    EcsMatTransform2D *m_container = ecs_data(rows, NULL, 1);

    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsMatTransform2D *m = ecs_data(rows, row, 0);
        *m = *m_container;
    }
}

void EcsApplyTranslation2D(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsMatTransform2D *m = ecs_data(rows, row, 0);
        EcsPosition2D *t = ecs_data(rows, row, 1);
        ecs_mat3x3_add_translation(m, t);
    }
}

void EcsApplyRotation2D(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsMatTransform2D *m = ecs_data(rows, row, 0);
        EcsRotation2D *t = ecs_data(rows, row, 1);
        ecs_mat3x3_add_rotation(m, t->angle);
    }
}

void EcsApplyScaling2D(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsMatTransform2D *m = ecs_data(rows, row, 0);
        EcsScale2D *t = ecs_data(rows, row, 1);
        ecs_mat3x3_add_scale(m, t);
    }
}

void EcsTransformChildren2D(EcsRows *rows) {
    EcsWorld *world = rows->world;
    float dt = rows->delta_time;
    EcsEntity EcsInitTransformChildren2D_h = ecs_component(rows, 1);
    EcsEntity EcsApplyTranslation2D_h = ecs_component(rows, 2);
    EcsEntity EcsApplyRotation2D_h = ecs_component(rows, 3);
    EcsEntity EcsApplyScaling2D_h = ecs_component(rows, 4);
    EcsEntity EcsTransformChildren2D_h = rows->system;

    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsEntity entity = ecs_entity(rows, row, ECS_ROW_ENTITY);
        ecs_run_w_filter(world, EcsInitTransformChildren2D_h, dt, 0, 0, entity, NULL);
        ecs_run_w_filter(world, EcsApplyTranslation2D_h, dt, 0, 0, entity, NULL);
        ecs_run_w_filter(world, EcsApplyRotation2D_h, dt, 0, 0, entity, NULL);
        ecs_run_w_filter(world, EcsApplyScaling2D_h, dt, 0, 0, entity, NULL);
        ecs_run_w_filter(world, EcsTransformChildren2D_h, dt, 0, 0, entity, NULL);
    }
}

void EcsTransform2D(EcsRows *rows) {
    EcsWorld *world = rows->world;
    float dt = rows->delta_time;
    EcsEntity root = ecs_entity(rows, rows->first, 0);
    EcsEntity EcsInitTransformChildren2D_h = ecs_component(rows, 1);
    EcsEntity EcsApplyTranslation2D_h = ecs_component(rows, 2);
    EcsEntity EcsApplyRotation2D_h = ecs_component(rows, 3);
    EcsEntity EcsApplyScaling2D_h = ecs_component(rows, 4);
    EcsEntity EcsTransformChildren2D_h = ecs_component(rows, 5);

    ecs_run_w_filter(world, EcsInitTransformChildren2D_h, dt, 0, 0, root, NULL);
    ecs_run_w_filter(world, EcsApplyTranslation2D_h, dt, 0, 0, root, NULL);
    ecs_run_w_filter(world, EcsApplyRotation2D_h, dt, 0, 0, root, NULL);
    ecs_run_w_filter(world, EcsApplyScaling2D_h, dt, 0, 0, root, NULL);
    ecs_run_w_filter(world, EcsTransformChildren2D_h, dt, 0, 0, root, NULL);
}

void EcsSystemsTransform(
    EcsWorld *world,
    int flags,
    void *handles_out)
{
    EcsSystemsTransformHandles *handles = handles_out;

    ECS_IMPORT(world, EcsComponentsTransform, flags);

    /* System that adds transform matrix to every entity with transformations */
    ECS_SYSTEM(world, EcsAddMatTransform2D, EcsOnLoad, EcsRoot | EcsPosition2D | EcsRotation2D | EcsScale2D, !EcsMatTransform2D);
    ecs_add(world, EcsAddMatTransform2D_h, EcsHidden_h);

    /* Systems that add transformations to transform matrix */
    ECS_SYSTEM(world, EcsApplyTranslation2D, EcsOnDemand, EcsMatTransform2D, EcsPosition2D);
    ECS_SYSTEM(world, EcsApplyRotation2D, EcsOnDemand, EcsMatTransform2D, EcsRotation2D);
    ECS_SYSTEM(world, EcsApplyScaling2D, EcsOnDemand, EcsMatTransform2D, EcsScale2D);
    ecs_add(world, EcsApplyTranslation2D_h, EcsHidden_h);
    ecs_add(world, EcsApplyRotation2D_h, EcsHidden_h);
    ecs_add(world, EcsApplyScaling2D_h, EcsHidden_h);

    /* Copy transformation from parent to child entities */
    ECS_SYSTEM(world, EcsInitTransformChildren2D, EcsOnDemand, EcsMatTransform2D, CONTAINER.EcsMatTransform2D);
    ecs_add(world, EcsInitTransformChildren2D_h, EcsHidden_h);

    /* System that applies transforms on child entities (invoked recursively) */
    ECS_SYSTEM(world, EcsTransformChildren2D, EcsOnDemand,
        EcsContainer,
        ID.EcsInitTransformChildren2D,
        ID.EcsApplyTranslation2D,
        ID.EcsApplyRotation2D,
        ID.EcsApplyScaling2D,
        !EcsRoot
    );

    /* System that applies transforms on top-level entities */
    ECS_SYSTEM(world, EcsTransform2D, EcsPostFrame,
        EcsRoot,
        ID.EcsInitTransformChildren2D,
        ID.EcsApplyTranslation2D,
        ID.EcsApplyRotation2D,
        ID.EcsApplyScaling2D,
        ID.EcsTransformChildren2D
    );

    ecs_add(world, EcsTransformChildren2D_h, EcsHidden_h);
    ecs_add(world, EcsTransform2D_h, EcsHidden_h);

    handles->Transform2D = EcsTransform2D_h;
}
