#ifndef __PYHIP_FILE_H__
#define __PYHIP_FILE_H__

#include <Python.h>
#include <hip.h>

typedef struct {
  PyObject_HEAD
  HIP_File *hipf;
  int fclose_on_del;
} py_hip;

#define PY_HIPFILE(x) (((py_hip *)x)->hipf)
extern PyTypeObject py_hip_type;

#endif /* __PYHIP_FILE_H__ */




