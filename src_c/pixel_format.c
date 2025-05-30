#ifndef PYGAMEAPI_SURFACE_INTERNAL
#include "surface.h"
#include "pgcompat.h"
#include "doc/surface_doc.h"
#endif

#ifdef PG_SDL3
static SDL_PixelFormat
#else
static SDL_PixelFormatEnum
#endif
_name_to_format(const char *name) {
    if (!name) {
        return SDL_PIXELFORMAT_UNKNOWN;
    }
    if (!strcmp(name, "index1lsb")) {
        return SDL_PIXELFORMAT_INDEX1LSB;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
    if (!strcmp(name, "")) {
        return ;
    }
#ifdef PG_SDL3
    if (!strcmp(name, "")) {
        return ;
    }
#else
    if (!strcmp(name, "")) {
        return ;
    }
#endif
    return SDL_PIXELFORMAT_UNKNOWN;
}

typedef struct {
    PyObject_HEAD
#ifdef PG_SDL3
        SDL_PixelFormat fmt_enum;
#else
        struct SDL_PixelFormat *fmt;
#endif
} pgPixelFormatObject;

static void
fmt_dealloc(PyObject *self)
{
    pgPixelFormatObject *fmt = (pgPixelFormatObject *)self;
#ifndef PG_SDL3
    if (fmt->fmt) {
        SDL_FreeFormat(fmt->fmt);
    }
#endif
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
fmt_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pgPixelFormatObject *self;
    self = (pgPixelFormatObject *)type->tp_alloc(type, 0);
    if (self) {
#ifdef PG_SDL3
        self->fmt_eum = SDL_PIXELFORMAT_UNKNOWN;
#else
        self->fmt = NULL;
#endif
    }
    return (PyObject *)self;
}

static int
fmt_init(pgPixelFormatObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *fmt_obj;
#ifdef PG_SDL3
    SDL_PixelFormat fmt_enum;
#else
    SDL_PixelFormatEnum fmt_enum;
#endif
    char *keywords[] = {"format", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", keywords, &fmt_obj)) {
        return -1;
    }
    if (PyLong_Check(fmt_obj)) {
        fmt_enum = PyLong_AsLong(fmt_obj);
        if (PyErr_Occurred()) {
            return NULL;
        }
    } else if (PyUnicode_Check(fmt_obj)) {
        const char *fmt_name = PyUnicode_AsUTF8(fmt_obj);
        fmt_enum = _name_to_format(fmt_name);
    } else {
        return RAISE(PyExc_TypeError, "'fomrat' must be string or an enum integer");
    }
#ifdef PG_SDL3
    self->fmt_enum = fmt_enum;
#else
    SDL_PixelFormat *fmt = SDL_AllocFormat(fmt_enum);
    if (!fmt) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    self->fmt = fmt;
#endif
    return 0;
}

static PyObject *
fmt_get_format(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    return PyLong_FromLong(self->fmt_enum);
#else
    return PyLong_FromUInt32(self->fmt->format);
#endif
}

static PyObject *
fmt_get_bits_per_pixel(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    return PyLong_FromUInt32(SDL_BITSPERPIXEL(details));
#else
    return PyLong_FromUInt32(self->fmt->BitsPerPixel);
#endif
}

static PyObject *
fmt_get_bytes_per_pixel(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    return PyLong_FromUInt32(SDL_BYTESPERPIXEL(details));
#else
    return PyLong_FromUInt32(self->fmt->BytesPerPixel);
#endif
}

static PyObject *
fmt_get_padding(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
#else
    SDL_PixelFormat *details = self->fmt;
#endif
    return Py_BuildValue("(ii)", details->padding[0], details->padding[1]);
}

static PyObject *
fmt_get_masks(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
#else
    SDL_PixelFormat *details = self->fmt;
#endif
    return Py_BuildValue("(iiii)", details->Rmask, details->Gmask, details->Bmask, details->Amask);
}

static PyObject *
fmt_get_shifts(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
#else
    SDL_PixelFormat *details = self->fmt;
#endif
    return Py_BuildValue("(iiii)", details->Rshift, details->Gshift, details->Bshift, details->Ashift);
}

static PyObject *
fmt_get_bits(pgPixelFormatObject *self, PyObject *_null)
{
#ifdef PG_SDL3
    SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(self->fmt_enum);
    if(!details) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    return Py_BuildValue("(iiii)", details->Rbits, details->Gbits, details->Bbits, details->Abits);
#else
    SDL_PixelFormat *details = self->fmt;
    return Py_BuildValue("(iiii)", details->Rloss, details->Gloss, details->Bloss, details->Aloss);
#endif
}

static PyGetSetDef fmt_getsets[] = {
    {"format", (getter)fmt_get_format, NULL, "", NULL},
    {"bits_per_pixel", (getter)fmt_get_bits_per_pixel, NULL, "", NULL},
    {"bytes_per_pixel", (getter)fmt_get_bytes_per_pixel, NULL, "", NULL},
    {"padding", (getter)fmt_get_padding, NULL, "", NULL},
    {"masks", (getter)fmt_get_masks, NULL, "", NULL},
    {"shifts", (getter)fmt_get_shifts, NULL, "", NULL},
    {"bits", (getter)fmt_get_bits, NULL, "", NULL},
    {NULL, NULL, NULL, NULL, NULL}};

static PyTypeObject pgPixelFormat_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.surface.PixelFormat",
    .tp_basicsize = sizeof(pgPixelFormatObject),
    .tp_dealloc = fmt_dealloc,
    //.tp_repr = fmt_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = DOC_SURFACE,  // EDIT
    //.tp_methods = surface_methods,
    .tp_getset = fmt_getsets,
    .tp_init = (initproc)fmt_init,
    .tp_new = fmt_new,
};