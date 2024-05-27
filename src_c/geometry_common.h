#ifndef PYGAME_CE_GEOMETRY_COMMON_H
#define PYGAME_CE_GEOMETRY_COMMON_H

#include "geometry.h"

int
_pg_circle_set_radius(PyObject *value, pgCircleBase *circle);

int
pgCircle_FromObject(PyObject *obj, pgCircleBase *out);

int
pgCircle_FromObjectFastcall(PyObject *const *args, Py_ssize_t nargs,
                            pgCircleBase *out);

static int
pgLine_FromObject(PyObject *obj, pgLineBase *out);

static int
pgLine_FromObjectFastcall(PyObject *const *args, Py_ssize_t nargs,
                          pgLineBase *out);

/* === Collision Functions === */

inline int
pgCollision_CirclePoint(pgCircleBase *circle, double Cx, double Cy);

inline int
pgCollision_CircleCircle(pgCircleBase *A, pgCircleBase *B);

inline int
pgCollision_RectCircle(double rx, double ry, double rw, double rh,
                       pgCircleBase *circle);

static int
pgCollision_LineLine(pgLineBase *, pgLineBase *);

static int
pgCollision_LinePoint(pgLineBase *, double, double);

static int
pgCollision_LineCircle(pgLineBase *, pgCircleBase *);

static int
pgCollision_RectLine(SDL_Rect *, pgLineBase *);

#endif  // PYGAME_CE_GEOMETRY_COMMON_H
