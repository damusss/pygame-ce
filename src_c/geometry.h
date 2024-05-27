#ifndef PYGAME_CE_GEOMETRY_H
#define PYGAME_CE_GEOMETRY_H

#include "pygame.h"
#include "pgcompat.h"

typedef struct {
    double x, y, r;
} pgCircleBase;

typedef struct {
    PyObject_HEAD pgCircleBase circle;
    PyObject *weakreflist;
} pgCircleObject;

#define pgCircle_CAST(o) ((pgCircleObject *)(o))
#define pgCircle_AsCircle(o) (pgCircle_CAST(o)->circle)
#define pgCircle_GETX(self) (pgCircle_CAST(self)->circle.x)
#define pgCircle_GETY(self) (pgCircle_CAST(self)->circle.y)
#define pgCircle_GETR(self) (pgCircle_CAST(self)->circle.r)
#define pgCircle_Check(o) ((o)->ob_type == &pgCircle_Type)

static PyTypeObject pgCircle_Type;

typedef struct {
    double xa, ya;
    double xb, yb;
} pgLineBase;

typedef struct {
    PyObject_HEAD pgLineBase line;
    PyObject *weakreflist;
} pgLineObject;

#define pgLine_CAST(o) ((pgLineObject *)(o))
#define pgLine_AsLine(o) (pgCircle_CAST(o)->circle)
#define pgLine_GETX1(self) (pgLine_CAST(self)->line.xa)
#define pgLine_GETY1(self) (pgLine_CAST(self)->line.ya)
#define pgLine_GETX2(self) (pgLine_CAST(self)->line.xb)
#define pgLine_GETY2(self) (pgLine_CAST(self)->line.yb)
#define pgLine_Check(o) ((o)->ob_type == &pgLine_Type)

static PyTypeObject pgLine_Type;

/* Constants */

/* PI */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* 2PI */
#ifndef M_TWOPI
#define M_TWOPI 6.28318530717958647692
#endif

/* PI/180 */
#ifndef M_PI_QUO_180
#define M_PI_QUO_180 0.01745329251994329577
#endif

/* 180/PI */
#ifndef M_180_QUO_PI
#define M_180_QUO_PI 57.29577951308232087680
#endif

/* Converts degrees to radians */
static inline double
DEG_TO_RAD(double deg)
{
    return deg * M_PI_QUO_180;
}

/* Converts radians to degrees */
static inline double
RAD_TO_DEG(double rad)
{
    return rad * M_180_QUO_PI;
}

#endif  // PYGAME_CE_GEOMETRY_H
