#include "boltor/ecs.h"

void BTR2DPhysicsSystem(BTR2DPositionComponent *p, BTR2DMovementComponent *m)
{
    const int gravity = -1;

    p->x = p->x + m->dx;

    if (p->y + m->dy > 0)
    {
        p->y  = p->y + m->dy;
        m->dy = m->dy + gravity;
    }
    else
    {
        p->y  = 0;
        m->dy = 0;
    }
};
