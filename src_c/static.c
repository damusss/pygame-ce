#define NO_PYGAME_C_API

#define CONTROLLER_NOPYX

#define PYGAMEAPI_RECT_INTERNAL
#define PYGAMEAPI_EVENT_INTERNAL
#define PYGAMEAPI_JOYSTICK_INTERNAL
#define PYGAMEAPI_BASE_INTERNAL
#define PYGAMEAPI_SURFACE_INTERNAL
#define PYGAMEAPI_BUFFERPROXY_INTERNAL
#define PYGAMEAPI_WINDOW_INTERNAL
#define PYGAMEAPI_RENDER_INTERNAL

#define pgSurface_New(surface) (pgSurfaceObject *)pgSurface_New2((surface), 1)
#define pgSurface_NewNoOwn(surface) \
    (pgSurfaceObject *)pgSurface_New2((surface), 0)

#include "pygame.h"
#include "Python.h"

#include <SDL_ttf.h>

#if defined(__EMSCRIPTEN__)
#undef WITH_THREAD
#endif

#if defined(BUILD_STATIC)
#undef import_pygame_base
#undef import_pygame_rect
#undef import_pygame_surface
#undef import_pygame_geometry
#undef import_pygame_color
#undef import_pygame_bufferproxy
#undef import_pygame_rwobject
#undef import_pygame_event

void
import_pygame_base(void)
{
}

void
import_pygame_rect(void)
{
}

void
import_pygame_surface(void)
{
}

void
import_pygame_window(void)
{
}

void
import_pygame_geometry(void)
{
}

void
import_pygame_color(void)
{
}

void
import_pygame_font(void)
{
}

void
import_pygame_freetype(void)
{
}

void
import_pygame_bufferproxy(void)
{
}

void
import_pygame_rwobject(void)
{
}

void
import_pygame_event(void)
{
}

void
import_pygame_joystick(void)
{
}

#undef import_pygame_imageext
#undef import_pygame_render
#undef import_pygame_pixelarray

void
import_pygame_imageext(void)
{
}

void
import_pygame_render(void)
{
}

void
import_pygame_pixelarray(void)
{
}

PyMODINIT_FUNC
PyInit_base(void);
PyMODINIT_FUNC
PyInit_color(void);
PyMODINIT_FUNC
PyInit_constants(void);
PyMODINIT_FUNC
PyInit_version(void);
PyMODINIT_FUNC
PyInit_rect(void);
PyMODINIT_FUNC
PyInit_geometry(void);
PyMODINIT_FUNC
PyInit_surflock(void);
PyMODINIT_FUNC
PyInit_rwobject(void);
PyMODINIT_FUNC
PyInit_bufferproxy(void);

PyMODINIT_FUNC
PyInit_surface(void);
PyMODINIT_FUNC
PyInit_display(void);
PyMODINIT_FUNC
PyInit__freetype(void);
PyMODINIT_FUNC
PyInit_font(void);

PyMODINIT_FUNC
PyInit_draw(void);
PyMODINIT_FUNC
PyInit_mouse(void);
PyMODINIT_FUNC
PyInit_key(void);
PyMODINIT_FUNC
PyInit_event(void);

PyMODINIT_FUNC
PyInit_joystick(void);

PyMODINIT_FUNC
PyInit_imageext(void);

PyMODINIT_FUNC
PyInit_image(void);

PyMODINIT_FUNC
PyInit_mask(void);

PyMODINIT_FUNC
PyInit_mixer_music(void);

PyMODINIT_FUNC
PyInit_pg_mixer(void);

PyMODINIT_FUNC
PyInit_pg_math(void);

PyMODINIT_FUNC
PyInit_pg_time(void);

PyMODINIT_FUNC
PyInit_sdl2(void);

PyMODINIT_FUNC
PyInit_mixer(void);

PyMODINIT_FUNC
PyInit_system(void);

#if defined(CONTROLLER_NOPYX)
PyMODINIT_FUNC
PyInit_controller(void);
#else
PyMODINIT_FUNC
PyInit_controller_old(void);
#endif

PyMODINIT_FUNC
PyInit_transform(void);

PyMODINIT_FUNC
PyInit_video(void);

PyMODINIT_FUNC
PyInit__sprite(void);

PyMODINIT_FUNC
PyInit_pixelcopy(void);

PyMODINIT_FUNC
PyInit_newbuffer(void);

PyMODINIT_FUNC
PyInit_gfxdraw(void);

PyMODINIT_FUNC
PyInit_audio(void);

PyMODINIT_FUNC
PyInit_pixelarray(void);

PyMODINIT_FUNC
PyInit_window(void);

PyMODINIT_FUNC
PyInit__render(void);

// pygame_static module

void
load_submodule(const char *parent, PyObject *mod, const char *alias)
{
    char fqn[1024];
    if (!mod) {
        snprintf(fqn, sizeof(fqn), "ERROR: PyInit_%s failed for %s.%s", alias,
                 parent, alias);
        puts(fqn);
        PyErr_Print();
        PyErr_Clear();
    }
    else {
        snprintf(fqn, sizeof(fqn), "%s.%s", parent, alias);
        PyObject *modules = PyImport_GetModuleDict();

        PyObject *pmod = PyDict_GetItemString(modules, parent);
        if (!pmod) {
            snprintf(fqn, sizeof(fqn), "ERROR: Parent %s not found for %s.%s",
                     parent, parent, alias);
            puts(fqn);
        }
        else {
            PyDict_SetItemString(modules, fqn, mod);
            PyDict_SetItemString(PyModule_GetDict(mod), "__name__",
                                 PyUnicode_FromString(fqn));
            PyModule_AddObjectRef(pmod, alias, mod);
            Py_XDECREF(mod);
        }
    }
}

void
load_submodule_mphase(const char *parent, PyObject *mdef, PyObject *spec,
                      const char *alias)
{
    char fqn[1024];
    snprintf(fqn, sizeof(fqn), "%s.%s", parent, alias);

    PyObject *modules = PyImport_GetModuleDict();

    Py_DECREF(PyObject_GetAttrString(spec, "name"));
    PyObject_SetAttrString(spec, "name", PyUnicode_FromString(alias));
    PyObject *pmod = PyDict_GetItemString(modules, parent);
    PyObject *mod = PyModule_FromDefAndSpec((PyModuleDef *)mdef, spec);
    PyDict_SetItemString(PyModule_GetDict(mod), "__package__",
                         PyUnicode_FromString(parent));
    // TODO SET PACKAGE
    PyModule_ExecDef(mod, (PyModuleDef *)mdef);

    if (pmod) {
        PyDict_SetItemString(modules, fqn, mod);
        PyDict_SetItemString(PyModule_GetDict(mod), "__name__",
                             PyUnicode_FromString(fqn));
        PyModule_AddObjectRef(pmod, alias, mod);
        Py_XDECREF(mod);
    }
    if (!pmod || PyErr_Occurred()) {
        snprintf(fqn, sizeof(fqn), "Error after init in : %s.%s\n", parent,
                 alias);
        fputs(fqn, stderr);
        PyErr_Print();
        PyErr_Clear();
    }
}

static PyObject *
mod_pygame_import_cython(PyObject *self, PyObject *spec)
{
    load_submodule_mphase("pygame._sdl2", PyInit_sdl2(), spec, "sdl2");

    load_submodule_mphase("pygame._sdl2", PyInit_mixer(), spec, "mixer");
#if defined(CONTROLLER_NOPYX)
    load_submodule("pygame._sdl2", PyInit_controller(), "controller");
#else
    load_submodule_mphase("pygame._sdl2", PyInit_controller_old(), spec,
                          "controller_old");
#endif
    load_submodule_mphase("pygame._sdl2", PyInit_audio(), spec, "audio");
    load_submodule_mphase("pygame._sdl2", PyInit_video(), spec, "video");

    Py_RETURN_NONE;
}

static PyMethodDef mod_pygame_static_methods[] = {
    {"import_cython", (PyCFunction)mod_pygame_import_cython, METH_O,
     "pygame._sdl2.sdl2"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef mod_pygame_static = {PyModuleDef_HEAD_INIT,
                                               "pygame_static", NULL, -1,
                                               mod_pygame_static_methods};

PyMODINIT_FUNC
PyInit_pygame_static()
{
    // cannot fail here, and font_initialized is already set to 1 in font.c .
    TTF_Init();

    // for correct input in wasm worker
    SDL_SetHint("SDL_EMSCRIPTEN_KEYBOARD_ELEMENT", "1");

    load_submodule("pygame", PyInit_base(), "base");

    //
    load_submodule("pygame", PyInit_constants(), "constants");
    //
    load_submodule("pygame", PyInit_pg_math(), "math");

    // base, pygame.colordict
    load_submodule("pygame", PyInit_color(), "color");

    // base
    load_submodule("pygame", PyInit_rect(), "rect");

    // base, rect
    load_submodule("pygame", PyInit_geometry(), "geometry");

    load_submodule("pygame", PyInit_bufferproxy(), "bufferproxy");
    load_submodule("pygame", PyInit_surflock(), "surflock");

    // base, color, rect, bufferproxy, surflock
    load_submodule("pygame", PyInit_surface(), "surface");

    load_submodule("pygame", PyInit_rwobject(), "rwobject");

    // base, color, rect, bufferproxy, surflock, surface, rwobject
    load_submodule("pygame", PyInit_imageext(), "imageext");
    // base, color, rect, bufferproxy, surflock, surface, rwobject
    load_submodule("pygame", PyInit_image(), "image");

    load_submodule("pygame", PyInit__freetype(), "_freetype");
    load_submodule("pygame", PyInit_font(), "font");
    load_submodule("pygame", PyInit_pixelcopy(), "pixelcopy");
    load_submodule("pygame", PyInit_newbuffer(), "newbuffer");

    // base
    load_submodule("pygame", PyInit_joystick(), "joystick");
    // base, joystick
    load_submodule("pygame", PyInit_event(), "event");

    // base, rect, event
    load_submodule("pygame", PyInit_key(), "key");
    // base, event
    load_submodule("pygame", PyInit_pg_time(), "time");

    load_submodule("pygame", PyInit_transform(), "transform");
    load_submodule("pygame", PyInit_draw(), "draw");

    load_submodule("pygame", PyInit_mask(), "mask");
    load_submodule("pygame", PyInit_mouse(), "mouse");

    load_submodule("pygame", PyInit_pg_mixer(), "mixer");
    load_submodule("pygame.mixer", PyInit_mixer_music(), "music");

    // base, color, rect, bufferproxy, surflock, surface
    load_submodule("pygame", PyInit_window(), "window");

    // base, color, rect, surflock, surface, window
    load_submodule("pygame", PyInit_display(), "display");
    load_submodule("pygame", PyInit__render(), "_render");

    load_submodule("pygame", PyInit_pixelarray(), "pixelarray");

    // base, color, rect, bufferproxy, surflock, surface
    load_submodule("pygame", PyInit_gfxdraw(), "gfxdraw");

    load_submodule("pygame", PyInit_system(), "system");

    return PyModule_Create(&mod_pygame_static);
}

#endif  // defined(BUILD_STATIC)

#include "base.c"

#include "rect.c"
#include "pgcompat_rect.c"

#undef pgSurface_Lock
#undef pgSurface_Unlock
#undef pgSurface_LockBy
#undef pgSurface_UnlockBy
#undef pgSurface_Prep
#undef pgSurface_Unprep

#include "surflock.c"

#undef pgColor_New
#undef pgColor_NewLength
#undef pg_RGBAFromObjEx
#undef pg_MappedColorFromObj
#undef pgColor_Type

#include "color.c"

#undef pgBufferProxy_New

#include "bufferproxy.c"

#undef pgSurface_Blit
#undef pgSurface_New
#undef pgSurface_Type
#undef pgSurface_SetSurface

#include "surface.c"
#include "simd_blitters_avx2.c"
#include "simd_blitters_sse2.c"

#include "window.c"

#undef pgVidInfo_Type
#undef pgVidInfo_New

#include "display.c"

#include "draw.c"

#undef pg_EncodeString
#undef pg_EncodeFilePath
#undef pgRWops_IsFileObject
#undef pgRWops_GetFileExtension
#undef pgRWops_FromFileObject
#undef pgRWops_FromObject

#include "rwobject.c"

#define pgSurface_New(surface) (pgSurfaceObject *)pgSurface_New2((surface), 1)
#include "render.c"
#include "image.c"

#include "imageext.c"

#include "mask.c"

#undef pg_EnableKeyRepeat
#undef pg_GetKeyRepeat
#undef pgEvent_FillUserEvent
#undef pgEvent_Type
#undef pgEvent_New

#include "joystick.c"

#include "event.c"

#include "mouse.c"

#include "key.c"

#include "time.c"

#include "system.c"
#include "geometry.c"

#include "_freetype.c"
#include "freetype/ft_wrap.c"
#include "freetype/ft_render.c"
#include "freetype/ft_render_cb.c"
#include "freetype/ft_cache.c"
#include "freetype/ft_layout.c"
#include "freetype/ft_unicode.c"

#include "font.c"

#include "mixer.c"

#include "music.c"

#include "gfxdraw.c"

#include "alphablit.c"

#include "surface_fill.c"
#include "pixelarray.c"
#include "pixelcopy.c"
#include "newbuffer.c"

#include "_sdl2/controller.c"
#include "_sdl2/controller_old.c"
#include "_sdl2/touch.c"
#include "transform.c"
// that remove some warnings
#undef MAX
#undef MIN
#include "scale2x.c"
