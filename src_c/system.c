#include "pygame.h"

#include "pgcompat.h"

#include "doc/system_doc.h"

static int file_dialog_uid = 0;

static PyObject *
pg_system_get_cpu_instruction_sets(PyObject *self, PyObject *_null)
{
    PyObject *tmp_bool = NULL;
    PyObject *instruction_sets = NULL;

    instruction_sets = PyDict_New();
    if (!instruction_sets) {
        goto error;
    }

#define INSERT_INSTRUCTIONSET_INFO(name, SDL_fn_name)               \
    tmp_bool = PyBool_FromLong((SDL_fn_name)());                    \
    if (PyDict_SetItemString(instruction_sets, (name), tmp_bool)) { \
        Py_DECREF(tmp_bool);                                        \
        goto error;                                                 \
    }                                                               \
    Py_DECREF(tmp_bool);

    // Don't insert SDL_HasRDTSC because it's been removed in SDL3
    INSERT_INSTRUCTIONSET_INFO("ALTIVEC", SDL_HasAltiVec);
    INSERT_INSTRUCTIONSET_INFO("MMX", SDL_HasMMX);
    INSERT_INSTRUCTIONSET_INFO("SSE", SDL_HasSSE);
    INSERT_INSTRUCTIONSET_INFO("SSE2", SDL_HasSSE2);
    INSERT_INSTRUCTIONSET_INFO("SSE3", SDL_HasSSE3);
    INSERT_INSTRUCTIONSET_INFO("SSE41", SDL_HasSSE41);
    INSERT_INSTRUCTIONSET_INFO("SSE42", SDL_HasSSE42);
    INSERT_INSTRUCTIONSET_INFO("AVX", SDL_HasAVX);
    INSERT_INSTRUCTIONSET_INFO("AVX2", SDL_HasAVX2);
    INSERT_INSTRUCTIONSET_INFO("AVX512F", SDL_HasAVX512F);
    INSERT_INSTRUCTIONSET_INFO("NEON", SDL_HasNEON);
    INSERT_INSTRUCTIONSET_INFO("ARMSIMD", SDL_HasARMSIMD);
#if SDL_VERSION_ATLEAST(2, 24, 0)
    INSERT_INSTRUCTIONSET_INFO("LSX", SDL_HasLSX);
    INSERT_INSTRUCTIONSET_INFO("LASX", SDL_HasLASX);
#else
    if (PyDict_SetItemString(instruction_sets, "LSX", Py_False))
        goto error;
    if (PyDict_SetItemString(instruction_sets, "LASX", Py_False))
        goto error;
#endif

#undef INSERT_INSTRUCTIONSET_INFO

    return instruction_sets;

error:
    Py_XDECREF(instruction_sets);
    return NULL;
}

static PyObject *
pg_system_get_total_ram(PyObject *self, PyObject *_null)
{
    return PyLong_FromLong(SDL_GetSystemRAM());
}

static PyObject *
pg_system_get_pref_path(PyObject *self, PyObject *args, PyObject *kwargs)
{
    char *org, *project;
    static char *kwids[] = {"org", "app", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwids, &org,
                                     &project)) {
        return NULL;
    }

    char *path = SDL_GetPrefPath(org, project);
    if (path == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }

    PyObject *ret = PyUnicode_FromString(path);
    SDL_free(path);

    return ret;
}

static PyObject *
pg_system_get_pref_locales(PyObject *self, PyObject *_null)
{
    PyObject *ret_list = PyList_New(0);
    if (!ret_list) {
        return NULL;
    }

    // Sorry about the SDL3 gnarliness here, this was the best way I could
    // think of to support SDL2/SDL3 at once. The approach is that each
    // version is responsible for coming up with a list and a count,
    // then the iteration over the list is shared (except for the indexing
    // strategy, where SDL2/3 are different)

    PyObject *dict, *val = NULL;
    int num_locales;
    SDL_Locale *current_locale;

#if SDL_VERSION_ATLEAST(3, 0, 0)
    SDL_Locale **locales = SDL_GetPreferredLocales(&num_locales);
    if (!locales) {
        /* Return an empty list if SDL function does not return any useful
         * information */
        return ret_list;
    }
#else
    SDL_Locale *locales = SDL_GetPreferredLocales();
    if (!locales) {
        /* Return an empty list if SDL function does not return any useful
         * information */
        return ret_list;
    }

    num_locales = 0;
    current_locale = locales;
    /* The array is terminated when the language attribute of the last struct
     * in the array is NULL */
    while (current_locale->language) {
        num_locales++;
        current_locale++;
    }
#endif

    for (int i = 0; i < num_locales; i++) {
        dict = PyDict_New();
        if (!dict) {
            goto error;
        }

#if SDL_VERSION_ATLEAST(3, 0, 0)
        current_locale = locales[i];
#else
        current_locale = locales + i;
#endif
        val = PyUnicode_FromString(current_locale->language);
        if (!val) {
            goto error;
        }
        if (PyDict_SetItemString(dict, "language", val)) {
            goto error;
        }
        Py_DECREF(val);

        if (current_locale->country) {
            val = PyUnicode_FromString(current_locale->country);
            if (!val) {
                goto error;
            }
        }
        else {
            Py_INCREF(Py_None);
            val = Py_None;
        }
        if (PyDict_SetItemString(dict, "country", val)) {
            goto error;
        }
        Py_DECREF(val);

        /* reset val to NULL because goto XDECREF's this */
        val = NULL;
        if (PyList_Append(ret_list, dict)) {
            goto error;
        }
        Py_DECREF(dict);
    }

    SDL_free(locales);
    return ret_list;
error:
    Py_XDECREF(val);
    Py_XDECREF(dict);
    SDL_free(locales);
    Py_DECREF(ret_list);
    return NULL;
}

static PyObject *PowerState_class = NULL;

static PyObject *
pg_system_get_power_state(PyObject *self, PyObject *_null)
{
    int sec, pct;
    SDL_PowerState power_state;
    PyObject *return_args;
    PyObject *return_kwargs;
    PyObject *sec_py, *pct_py;

    power_state = SDL_GetPowerInfo(&sec, &pct);

    if (power_state == SDL_POWERSTATE_UNKNOWN) {
        Py_RETURN_NONE;
    }

    if (sec == -1) {
        sec_py = Py_None;
        Py_INCREF(Py_None);
    }
    else {
        sec_py = PyLong_FromLong(sec);
    }

    if (pct == -1) {
        pct_py = Py_None;
        Py_INCREF(Py_None);
    }
    else {
        pct_py = PyLong_FromLong(pct);
    }
    // Error check will be done in Py_BuildValue

    int on_battery = (power_state == SDL_POWERSTATE_ON_BATTERY);
    int no_battery = (power_state == SDL_POWERSTATE_NO_BATTERY);
    int charging = (power_state == SDL_POWERSTATE_CHARGING);
    int charged = (power_state == SDL_POWERSTATE_CHARGED);

    // clang-format off
    return_kwargs = Py_BuildValue(
        "{s:N,s:N,s:N,s:N,s:N,s:N,s:N,s:N}",
        "battery_percent", pct_py,
        "battery_seconds", sec_py,
        "on_battery", PyBool_FromLong(on_battery),
        "no_battery", PyBool_FromLong(no_battery),
        "charging", PyBool_FromLong(charging),
        "charged", PyBool_FromLong(charged),
        "plugged_in", PyBool_FromLong(!on_battery),
        "has_battery", PyBool_FromLong(on_battery || !no_battery)
    );
    // clang-format on

    if (!return_kwargs)
        return NULL;

    return_args = Py_BuildValue("()");

    if (!return_args)
        return NULL;

    if (!PowerState_class) {
        return RAISE(PyExc_SystemError, "PowerState class is not imported.");
    }

    return PyObject_Call(PowerState_class, return_args, return_kwargs);
}

typedef struct {
    int uid;
    SDL_DialogFileFilter *filters;
} pg_FileDialogUserData;

static void SDLCALL
_file_dialog_callback(void *userdata, const char *const *filelist, int filter)
{
    pg_FileDialogUserData *data = (pg_FileDialogUserData *)userdata;

    if (data->filters) {
        SDL_free(data->filters);
        data->filters = NULL;
    }

    PyGILState_STATE gil_state = PyGILState_Ensure();

    PyObject *file_list_obj = PyList_New(0);
    if (!file_list_obj) {
        PyErr_NoMemory();
        SDL_free(data);
        PyGILState_Release(gil_state);
        return;
    }

    for (const char *const *file = filelist; file && *file; file++) {
        PyObject *file_obj = PyUnicode_FromString(*file);
        if (!file_obj) {
            Py_DECREF(file_list_obj);
            PyErr_NoMemory();
            SDL_free(data);
            PyGILState_Release(gil_state);
            return;
        }
        PyList_Append(file_list_obj, file_obj);
        Py_DECREF(file_obj);
    }

    PyObject *py_filter = PyLong_FromLong(filter);
    if (!py_filter) {
        Py_DECREF(file_list_obj);
        PyErr_NoMemory();
        SDL_free(data);
        PyGILState_Release(gil_state);
        return;
    }

    PyObject *event_dict = PyDict_New();
    if (event_dict) {
        PyObject *uid_obj = PyLong_FromLong(data->uid);
        if (!uid_obj) {
            PyGILState_Release(gil_state);
            SDL_free(data);
            Py_DECREF(event_dict);
            Py_DECREF(file_list_obj);
            return;
        }
        PyObject *filter_obj = PyLong_FromLong(filter);
        if (!filter_obj) {
            PyGILState_Release(gil_state);
            SDL_free(data);
            Py_DECREF(event_dict);
            Py_DECREF(file_list_obj);
            return;
        }
        PyDict_SetItemString(event_dict, "uid", uid_obj);
        PyDict_SetItemString(event_dict, "filter", filter_obj);
        PyDict_SetItemString(event_dict, "filepaths", file_list_obj);
        Py_DECREF(uid_obj);
        Py_DECREF(filter_obj);
        Py_DECREF(file_list_obj);
        pg_post_event(PGE_FILEDIALOG, event_dict);
        Py_DECREF(event_dict);
    }
    else {
        Py_DECREF(file_list_obj);
    }

    PyGILState_Release(gil_state);
    SDL_free(data);
}

static int
_validate_and_show_file_dialog(SDL_FileDialogType type, int *uid,
                               int allow_many, PyObject *default_location_obj,
                               PyObject *title_obj,
                               PyObject *parent_window_obj,
                               PyObject *filters_obj)
{
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_DialogFileFilter *filters = NULL;

#pragma region FILTERS

    if (filters_obj != Py_None) {
        if (!PySequence_Check(filters_obj)) {
            PyErr_SetString(PyExc_TypeError, "'filters' must be a sequence");
            return -1;
        }

        Py_ssize_t filters_size = PySequence_Size(filters_obj);
        if (filters_size == -1) {
            return -1;  // error already set
        }

        filters = (SDL_DialogFileFilter *)SDL_malloc(
            filters_size * sizeof(SDL_DialogFileFilter));
        if (!filters) {
            PyErr_NoMemory();
            return -1;
        }

        for (Py_ssize_t i = 0; i < filters_size; i++) {
            PyObject *item = PySequence_GetItem(filters_obj, i);
            if (!item) {
                SDL_free(filters);
                return -1;  // error already set
            }

            if (!PySequence_Check(item) || PySequence_Size(item) != 2) {
                PyErr_SetString(PyExc_TypeError,
                                "Each filter item must be a valid sequence of "
                                "exactly 2 string objects");
                Py_DECREF(item);
                SDL_free(filters);
                return -1;
            }

            PyObject *str1_obj = PySequence_GetItem(item, 0);
            PyObject *str2_obj = PySequence_GetItem(item, 1);
            Py_DECREF(item);

            if (!PyUnicode_Check(str1_obj) || !PyUnicode_Check(str2_obj)) {
                PyErr_SetString(PyExc_TypeError,
                                "Each filter item must be a valid sequence of "
                                "exactly 2 string objects");
                Py_XDECREF(str1_obj);
                Py_XDECREF(str2_obj);
                SDL_free(filters);
                return -1;
            }

            const char *str1 = PyUnicode_AsUTF8(str1_obj);
            const char *str2 = PyUnicode_AsUTF8(str2_obj);

            if (!str1 || !str2) {
                Py_XDECREF(str1_obj);
                Py_XDECREF(str2_obj);
                SDL_free(filters);
                return -1;  // error already set
            }

            filters[i].name = str1;
            filters[i].pattern = str2;

            Py_DECREF(str1_obj);
            Py_DECREF(str2_obj);
        }
        if (SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_FILTERS_POINTER,
                                   filters) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            SDL_free(filters);
            return -1;
        }
        if (SDL_SetNumberProperty(props, SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER,
                                  filters_size) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            SDL_free(filters);
            return -1;
        }
    }

#pragma endregion

    if (allow_many == 1) {
        if (SDL_SetBooleanProperty(props, SDL_PROP_FILE_DIALOG_MANY_BOOLEAN,
                                   SDL_TRUE) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            return -1;
        }
    }

    if (PyUnicode_Check(default_location_obj)) {
        const char *default_location = PyUnicode_AsUTF8(default_location_obj);
        if (!default_location) {
            return -1;  // error already set
        }
        if (SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_LOCATION_STRING,
                                  default_location) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            return -1;
        }
    }
    else if (default_location_obj != Py_None) {
        PyErr_Format(PyExc_TypeError,
                     "'default_location' must be a string or None, not '%s'",
                     default_location_obj->ob_type->tp_name);
        return -1;
    }

    if (PyUnicode_Check(title_obj)) {
        const char *title = PyUnicode_AsUTF8(title_obj);
        if (!title) {
            return -1;  // error already set
        }
        if (SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING,
                                  title) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            return -1;
        }
    }
    else if (title_obj != Py_None) {
        PyErr_Format(PyExc_TypeError,
                     "'title' must be a string or None, not '%s'",
                     title_obj->ob_type->tp_name);
        return -1;
    }

    if (pgWindow_Check(parent_window_obj)) {
        SDL_Window *window = ((pgWindowObject *)parent_window_obj)->_win;
        if (SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_WINDOW_POINTER,
                                   window) == SDL_FALSE) {
            PyErr_SetString(pgExc_SDLError, SDL_GetError());
            return -1;
        }
    }
    else if (parent_window_obj != Py_None) {
        PyErr_Format(
            PyExc_TypeError,
            "'parent_window' must be a pygame.Window or None, not '%s'",
            title_obj->ob_type->tp_name);
        return -1;
    }

    *uid = file_dialog_uid;
    file_dialog_uid++;

    pg_FileDialogUserData *userdata =
        (pg_FileDialogUserData *)SDL_malloc(sizeof(pg_FileDialogUserData));
    if (!userdata) {
        PyErr_NoMemory();
        return -1;
    }
    userdata->uid = *uid;
    userdata->filters = filters;

    SDL_ShowFileDialogWithProperties(type, _file_dialog_callback, userdata,
                                     props);

    return 0;
}

static PyObject *
pg_system_show_file_dialog(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int uid;
    PyObject *default_location_obj = Py_None, *title_obj = Py_None;
    PyObject *parent_window_obj = Py_None, *filters_obj = Py_None;
    int allow_many = 0;

    static char *keywords[] = {"allow_many",    "default_location", "title",
                               "parent_window", "filters",          NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|pOOOO", keywords,
                                     &allow_many, &default_location_obj,
                                     &title_obj, &parent_window_obj,
                                     &filters_obj))
        return NULL;

    if (_validate_and_show_file_dialog(
            SDL_FILEDIALOG_OPENFILE, &uid, allow_many, default_location_obj,
            title_obj, parent_window_obj, filters_obj) < 0) {
        return NULL;
    }

    return PyLong_FromLong(uid);
}

static PyObject *
pg_system_show_folder_dialog(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int uid;
    PyObject *default_location_obj = Py_None, *title_obj = Py_None;
    PyObject *parent_window_obj = Py_None;
    int allow_many = 0;

    static char *keywords[] = {"allow_many", "default_location", "title",
                               "parent_window", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|pOOO", keywords,
                                     &allow_many, &default_location_obj,
                                     &title_obj, &parent_window_obj))
        return NULL;

    if (_validate_and_show_file_dialog(
            SDL_FILEDIALOG_OPENFOLDER, &uid, allow_many, default_location_obj,
            title_obj, parent_window_obj, Py_None) < 0) {
        return NULL;
    }

    return PyLong_FromLong(uid);
}

static PyObject *
pg_system_show_save_dialog(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int uid;
    PyObject *default_location_obj = Py_None, *title_obj = Py_None;
    PyObject *parent_window_obj = Py_None, *filters_obj = Py_None;

    static char *keywords[] = {"default_location", "title", "parent_window",
                               "filters", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|OOOO", keywords,
                                     &default_location_obj, &title_obj,
                                     &parent_window_obj, &filters_obj))
        return NULL;

    if (_validate_and_show_file_dialog(SDL_FILEDIALOG_SAVEFILE, &uid, 0,
                                       default_location_obj, title_obj,
                                       parent_window_obj, filters_obj) < 0) {
        return NULL;
    }

    return PyLong_FromLong(uid);
}

static PyMethodDef _system_methods[] = {
    {"get_cpu_instruction_sets", pg_system_get_cpu_instruction_sets,
     METH_NOARGS, DOC_SYSTEM_GETCPUINSTRUCTIONSETS},
    {"get_total_ram", pg_system_get_total_ram, METH_NOARGS,
     DOC_SYSTEM_GETTOTALRAM},
    {"get_pref_path", (PyCFunction)pg_system_get_pref_path,
     METH_VARARGS | METH_KEYWORDS, DOC_SYSTEM_GETPREFPATH},
    {"get_pref_locales", pg_system_get_pref_locales, METH_NOARGS,
     DOC_SYSTEM_GETPREFLOCALES},
    {"get_power_state", pg_system_get_power_state, METH_NOARGS,
     DOC_SYSTEM_GETPOWERSTATE},
    {"show_file_dialog", (PyCFunction)pg_system_show_file_dialog,
     METH_VARARGS | METH_KEYWORDS, "test"},
    {"show_folder_dialog", (PyCFunction)pg_system_show_folder_dialog,
     METH_VARARGS | METH_KEYWORDS, "test"},
    {"show_save_dialog", (PyCFunction)pg_system_show_save_dialog,
     METH_VARARGS | METH_KEYWORDS, "test"},
    {NULL, NULL, 0, NULL}};

MODINIT_DEFINE(system)
{
    PyObject *module;
    static struct PyModuleDef _module = {
        .m_base = PyModuleDef_HEAD_INIT,
        .m_name = "system",
        .m_doc = DOC_SYSTEM,
        .m_size = -1,
        .m_methods = _system_methods,
    };

    /* need to import base module, just so SDL is happy. Do this first so if
       the module is there is an error the module is not loaded.
    */
    import_pygame_base();
    if (PyErr_Occurred()) {
        return NULL;
    }

    import_pygame_window();
    if (PyErr_Occurred()) {
        return NULL;
    }

    import_pygame_event();
    if (PyErr_Occurred()) {
        return NULL;
    }

    PyObject *data_classes_module =
        PyImport_ImportModule("pygame._data_classes");
    if (!data_classes_module) {
        return NULL;
    }

    PowerState_class =
        PyObject_GetAttrString(data_classes_module, "PowerState");
    if (!PowerState_class) {
        return NULL;
    }
    Py_DECREF(data_classes_module);

    /* create the module */
    module = PyModule_Create(&_module);
    if (!module) {
        return NULL;
    }

    if (PyModule_AddObject(module, "PowerState", PowerState_class)) {
        Py_DECREF(PowerState_class);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}
