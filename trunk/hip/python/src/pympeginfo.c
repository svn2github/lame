#include <assert.h>
#include <hip.h>

#include "hipmodule.h"
#include "pympeginfo.h"

static void py_mpeg_info_dealloc(PyObject *);
static PyObject *py_mpeg_info_getattr(PyObject *, char *name);

char py_mpeg_info_doc[] = 
"A mpeg_info object stores information about a MPEG stream.\n\
Information is stored as attributes.";

PyTypeObject py_mpeg_info_type = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "mpeg_info",
  sizeof(py_mpeg_info),
  0,

  /* Standard Methods */
  /* destructor */ py_mpeg_info_dealloc,
  /* printfunc */ 0,
  /* getattrfunc */ py_mpeg_info_getattr,
  /* setattrfunc */ 0,
  /* cmpfunc */ 0,
  /* reprfunc */ 0,
  
  /* Type Categories */
  0, /* as number */
  0, /* as sequence */
  0, /* as mapping */
  0, /* hash */
  0, /* binary */
  0, /* repr */
  0, /* getattro */
  0, /* setattro */
  0, /* as buffer */
  0, /* tp_flags */
  py_mpeg_info_doc,
};


PyObject*
py_info_new_from_mpeg_info(mpeg_info *mi)
{
  py_mpeg_info *newobj;
  newobj = (py_mpeg_info *) PyObject_NEW(py_mpeg_info, 
				     &py_mpeg_info_type);
  newobj->mi = *mi;
  return (PyObject *) newobj;
}

static char *py_info_new_kw[] = {"channels", "rate", "max_bitrate",
				 "nominal_bitrate", "min_bitrate", NULL};

PyObject *
py_info_new(PyObject *self, PyObject *args, PyObject *kwdict)
{
  long channels, rate, max_bitrate, nominal_bitrate, min_bitrate;
  mpeg_info mi; 

  channels = 2;
  rate = 44100;
  max_bitrate = -1;
  nominal_bitrate = 128000;
  min_bitrate = -1;
  if (!PyArg_ParseTupleAndKeywords(args, kwdict, 
				   "|lllll", py_info_new_kw, 
				   &channels, &rate, &max_bitrate,
				   &nominal_bitrate, &min_bitrate))
    return NULL;

  return py_info_new_from_mpeg_info(&mi);
}

static void
py_mpeg_info_dealloc(PyObject *self)
{
  PyMem_DEL(self);
}

#define CMP_RET(x) \
   if (strcmp(name, #x) == 0) \
     return PyInt_FromLong(mi->x)

static PyObject *
py_mpeg_info_getattr(PyObject *self, char *name)
{
  mpeg_info *mi = PY_MINFO(self);
  char err_msg[MSG_SIZE];

  PyErr_Clear();

  switch(name[0]) {
  case 'b':
    CMP_RET(bitrate_upper);
    CMP_RET(bitrate_nominal);
    CMP_RET(bitrate_lower);
    break;
  case 'c':
    CMP_RET(channels);
    break;
  case 'r':
    CMP_RET(rate);
    break;
  }

  snprintf(err_msg, MSG_SIZE, "No attribute: %s", name);
  PyErr_SetString(PyExc_AttributeError, err_msg);
  return NULL;
}

