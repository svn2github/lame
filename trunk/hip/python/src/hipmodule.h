#ifndef __HIPMODULE_H__
#define __HIPMODULE_H__

#include <Python.h>

#define MSG_SIZE 256

PyObject *Py_HIPError;

/* Object docstrings */

extern char py_hip_doc[];

/* Module-accessible functions */

PyObject *py_hip_new(PyObject *, PyObject *);

/* Utility functions/macros */

PyObject *v_error_from_code(int, char*);

#define RETURN_IF_VAL(val, msg)    if (val < 0) { \
                                       return v_error_from_code(val, msg); \
                                   } \
                                   Py_INCREF(Py_None); \
                                   return Py_None;


#endif /* __HIPMODULE_H__ */

