#include "geometry_common.h"

int
_pg_circle_set_radius(PyObject *value, pgCircleBase *circle)
{
    double radius = 0.0;
    if (!pg_DoubleFromObj(value, &radius) || radius <= 0.0) {
        return 0;
    }
    circle->r = radius;
    return 1;
}

int
pgCircle_FromObject(PyObject *obj, pgCircleBase *out)
{
    Py_ssize_t length;

    if (pgCircle_Check(obj)) {
        *out = pgCircle_AsCircle(obj);
        return 1;
    }

    /* Paths for sequences */
    if (pgSequenceFast_Check(obj)) {
        PyObject **f_arr = PySequence_Fast_ITEMS(obj);
        length = PySequence_Fast_GET_SIZE(obj);

        switch (length) {
            case 1:
                return pgCircle_FromObject(f_arr[0], out);
            case 2:
                return pg_TwoDoublesFromObj(f_arr[0], &out->x, &out->y) &&
                       _pg_circle_set_radius(f_arr[1], out);
            case 3:
                return pg_DoubleFromObj(f_arr[0], &out->x) &&
                       pg_DoubleFromObj(f_arr[1], &out->y) &&
                       _pg_circle_set_radius(f_arr[2], out);
            default:
                return 0;
        }
    }
    else if (PySequence_Check(obj)) {
        PyObject *tmp = NULL;
        length = PySequence_Length(obj);

        if (length == 3) {
            tmp = PySequence_ITEM(obj, 0);
            if (!pg_DoubleFromObj(tmp, &out->x)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);

            tmp = PySequence_ITEM(obj, 1);
            if (!pg_DoubleFromObj(tmp, &out->y)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);

            tmp = PySequence_ITEM(obj, 2);
            if (!_pg_circle_set_radius(tmp, out)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);

            return 1;
        }
        else if (length == 2) {
            tmp = PySequence_ITEM(obj, 0);
            if (!pg_TwoDoublesFromObj(tmp, &out->x, &out->y)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);

            tmp = PySequence_ITEM(obj, 1);
            if (!_pg_circle_set_radius(tmp, out)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);

            return 1;
        }
        else if (length == 1) {
            tmp = PySequence_ITEM(obj, 0);
            if (PyUnicode_Check(obj) || !pgCircle_FromObject(tmp, out)) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            return 1;
        }
        else {
            return 0;
        }
    }

    /* Path for objects that have a circle attribute */
    PyObject *circleattr;
    if (!(circleattr = PyObject_GetAttrString(obj, "circle"))) {
        PyErr_Clear();
        return 0;
    }

    if (PyCallable_Check(circleattr)) /*call if it's a method*/
    {
        PyObject *circleresult = PyObject_CallObject(circleattr, NULL);
        Py_DECREF(circleattr);
        if (!circleresult) {
            PyErr_Clear();
            return 0;
        }
        circleattr = circleresult;
    }

    if (!pgCircle_FromObject(circleattr, out)) {
        PyErr_Clear();
        Py_DECREF(circleattr);
        return 0;
    }

    Py_DECREF(circleattr);

    return 1;
}

int
pgCircle_FromObjectFastcall(PyObject *const *args, Py_ssize_t nargs,
                            pgCircleBase *out)
{
    switch (nargs) {
        case 1:
            return pgCircle_FromObject(args[0], out);
        case 2:
            return pg_TwoDoublesFromObj(args[0], &out->x, &out->y) &&
                   _pg_circle_set_radius(args[1], out);
        case 3:
            return pg_DoubleFromObj(args[0], &out->x) &&
                   pg_DoubleFromObj(args[1], &out->y) &&
                   _pg_circle_set_radius(args[2], out);
        default:
            return 0;
    }
}

static int
pgLine_FromObject(PyObject *obj, pgLineBase *out)
{
    Py_ssize_t length;

    if (pgLine_Check(obj)) {
        *out = ((pgLineObject *)obj)->line;
        return 1;
    }
    if (PyList_Check(obj) || PyTuple_Check(obj)) {
        length = PySequence_Fast_GET_SIZE(obj);
        PyObject **farray = PySequence_Fast_ITEMS(obj);

        if (length == 4) {
            if (!pg_DoubleFromObj(farray[0], &(out->xa)) ||
                !pg_DoubleFromObj(farray[1], &(out->ya)) ||
                !pg_DoubleFromObj(farray[2], &(out->xb)) ||
                !pg_DoubleFromObj(farray[3], &(out->yb))) {
                return 0;
            }
            return IS_LINE_VALID(out);
        }
        else if (length == 2) {
            if (!pg_TwoDoublesFromObj(farray[0], &(out->xa), &(out->ya)) ||
                !pg_TwoDoublesFromObj(farray[1], &(out->xb), &(out->yb))) {
                PyErr_Clear();
                return 0;
            }
            return IS_LINE_VALID(out);
        }
        else if (length == 1) /*looks like an arg?*/ {
            if (PyUnicode_Check(farray[0]) ||
                !pgLine_FromObject(farray[0], out)) {
                return 0;
            }
            return IS_LINE_VALID(out);
        }
    }
    if (PySequence_Check(obj)) {
        length = PySequence_Length(obj);
        if (length == 4) { /*  && !pgPolygon_Check(obj) */
            PyObject *tmp;
            tmp = PySequence_GetItem(obj, 0);
            if (!pg_DoubleFromObj(tmp, &(out->xa))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            tmp = PySequence_GetItem(obj, 1);
            if (!pg_DoubleFromObj(tmp, &(out->ya))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            tmp = PySequence_GetItem(obj, 2);
            if (!pg_DoubleFromObj(tmp, &(out->xb))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            tmp = PySequence_GetItem(obj, 3);
            if (!pg_DoubleFromObj(tmp, &(out->yb))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            return IS_LINE_VALID(out);
        }
        else if (length == 2) {
            PyObject *tmp;
            tmp = PySequence_GetItem(obj, 0);
            if (!pg_TwoDoublesFromObj(tmp, &(out->xa), &(out->ya))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            tmp = PySequence_GetItem(obj, 1);
            if (!pg_TwoDoublesFromObj(tmp, &(out->xb), &(out->yb))) {
                Py_DECREF(tmp);
                return 0;
            }
            Py_DECREF(tmp);
            return IS_LINE_VALID(out);
        }
        else if (PyTuple_Check(obj) && length == 1) /*looks like an arg?*/ {
            PyObject *sub = PySequence_GetItem(obj, 0);
            if (PyUnicode_Check(sub) || !pgLine_FromObject(sub, out)) {
                Py_DECREF(sub);
                return 0;
            }
            Py_DECREF(sub);
            return IS_LINE_VALID(out);
        }
        else {
            return 0;
        }
    }
    if (PyObject_HasAttrString(obj, "line")) {
        PyObject *lineattr;
        lineattr = PyObject_GetAttrString(obj, "line");
        if (!lineattr) {
            PyErr_Clear();
            return 0;
        }
        if (PyCallable_Check(lineattr)) /*call if it's a method*/
        {
            PyObject *lineresult = PyObject_CallObject(lineattr, NULL);
            Py_DECREF(lineattr);
            if (!lineresult) {
                PyErr_Clear();
                return 0;
            }
            lineattr = lineresult;
        }
        Py_DECREF(lineattr);
        return pgLine_FromObject(lineattr, out);
    }
    return 0;
}

static int
pgLine_FromObjectFastcall(PyObject *const *args, Py_ssize_t nargs,
                          pgLineBase *out)
{
    if (nargs == 1) {
        return pgLine_FromObject(args[0], out);
    }
    else if (nargs == 2) {
        if (!pg_TwoDoublesFromObj(args[0], &(out->xa), &(out->ya)) ||
            !pg_TwoDoublesFromObj(args[1], &(out->xb), &(out->yb))) {
            return 0;
        }
        return IS_LINE_VALID(out);
    }
    else if (nargs == 4) {
        if (!pg_DoubleFromObj(args[0], &(out->xa)) ||
            !pg_DoubleFromObj(args[1], &(out->ya)) ||
            !pg_DoubleFromObj(args[2], &(out->xb)) ||
            !pg_DoubleFromObj(args[3], &(out->yb))) {
            return 0;
        }
        return IS_LINE_VALID(out);
    }
    return 0;
}

/* === Collision Functions === */

inline int
pgCollision_CirclePoint(pgCircleBase *circle, double Cx, double Cy)
{
    double dx = circle->x - Cx;
    double dy = circle->y - Cy;
    return dx * dx + dy * dy <= circle->r * circle->r;
}

inline int
pgCollision_CircleCircle(pgCircleBase *A, pgCircleBase *B)
{
    double dx, dy;
    double sum_radii;

    dx = A->x - B->x;
    dy = A->y - B->y;
    sum_radii = A->r + B->r;

    return dx * dx + dy * dy <= sum_radii * sum_radii;
}

inline int
pgCollision_RectCircle(double rx, double ry, double rw, double rh,
                       pgCircleBase *circle)
{
    const double cx = circle->x, cy = circle->y;
    const double r_bottom = ry + rh, r_right = rx + rw;

    const double test_x = (cx < rx) ? rx : ((cx > r_right) ? r_right : cx);
    const double test_y = (cy < ry) ? ry : ((cy > r_bottom) ? r_bottom : cy);

    return pgCollision_CirclePoint(circle, test_x, test_y);
}

static int
pgCollision_LineLine(pgLineBase *A, pgLineBase *B)
{
    double x1_m_x2 = A->xa - A->xb;
    double y3_m_y4 = B->ya - B->yb;
    double y1_m_y2 = A->ya - A->yb;
    double x3_m_x4 = B->xa - B->xb;

    double den = x1_m_x2 * y3_m_y4 - y1_m_y2 * x3_m_x4;

    if (!den)
        return 0;

    double x1_m_x3 = A->xa - B->xa;
    double y1_m_y3 = A->ya - B->ya;

    double t1 = x1_m_x3 * y3_m_y4 - y1_m_y3 * x3_m_x4;
    double t = t1 / den;

    double u1 = x1_m_x2 * y1_m_y3 - y1_m_y2 * x1_m_x3;
    double u = -(u1 / den);

    return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}

static int
pgCollision_LinePoint(pgLineBase *line, double Cx, double Cy)
{
    double dx = line->xa - Cx;
    double dy = line->ya - Cy;
    double dx2 = line->xb - Cx;
    double dy2 = line->yb - Cy;
    double dx3 = line->xa - line->xb;
    double dy3 = line->ya - line->yb;

    double d = sqrt(dx * dx + dy * dy) + sqrt(dx2 * dx2 + dy2 * dy2);
    double d3 = sqrt(dx3 * dx3 + dy3 * dy3);

    double width = 0.000001;
    return d >= d3 - width && d <= d3 + width;
}

static int
pgCollision_LineCircle(pgLineBase *line, pgCircleBase *circle)
{
    double xa = line->xa;
    double ya = line->ya;
    double xb = line->xb;
    double yb = line->yb;
    double cx = circle->x;
    double cy = circle->y;
    double r = circle->r;

    if (pgCollision_CirclePoint(circle, xa, ya) ||
        pgCollision_CirclePoint(circle, xb, yb))
        return 1;

    double dx = xa - xb;
    double dy = ya - yb;
    double len = sqrt((dx * dx) + (dy * dy));

    double dot =
        (((cx - xa) * (xb - xa)) + ((cy - ya) * (yb - ya))) / (len * len);

    double closest_x = xa + (dot * (xb - xa));
    double closest_y = ya + (dot * (yb - ya));

    pgLineBase line2 = {xa, ya, xb, yb};
    if (!pgCollision_LinePoint(&line2, closest_x, closest_y))
        return 0;

    dx = closest_x - cx;
    dy = closest_y - cy;
    double distance = sqrt((dx * dx) + (dy * dy));

    return distance <= r;
}

static int
pgCollision_RectLine(SDL_Rect *rect, pgLineBase *line)
{
#if AVX2_IS_SUPPORTED
    if (pg_HasAVX2())
        return pgCollision_RectLine_avx2(rect, line);
#endif /* ~__AVX2__ */

    double x = (double)rect->x;
    double y = (double)rect->y;
    double w = (double)rect->w;
    double h = (double)rect->h;

    pgLineBase a = {x, y, x + w, y};
    pgLineBase b = {x, y, x, y + h};
    pgLineBase c = {x, y + h, x + w, y + h};
    pgLineBase d = {x + w, y, x + w, y + h};

    return pgCollision_LineLine(line, &a) || pgCollision_LineLine(line, &b) ||
           pgCollision_LineLine(line, &c) || pgCollision_LineLine(line, &d);
}
