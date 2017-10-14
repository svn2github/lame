#ifndef __PYMPEG_INFO_H__
#define __PYMPEG_INFO_H__

#include <hip.h>

typedef struct {
  PyObject_HEAD
  mpeg_info mi;
} py_mpeg_info;

#define PY_MINFO(x) (&(((py_mpeg_info *) (x))->mi))

extern PyTypeObject py_mpeg_info_type;

PyObject *py_info_new_from_mpeg_info(mpeg_info *mi);
PyObject *py_info_new(PyObject *, PyObject *, PyObject *);

#endif /* __PYMPEG_INFO_H__ */

