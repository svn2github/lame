#include <stdio.h>
#include <endian.h>
#include <assert.h>

#include "hipmodule.h"
#include "pympeginfo.h"
#include "pyhip.h"

char py_hip_doc[] = "";

static void py_hip_file_dealloc(PyObject *);

static PyObject *py_hip_open(py_hip *, PyObject *); 

static PyObject *py_hip_file_getattr(PyObject *, char *name);

static PyObject *py_hip_read(PyObject *, PyObject *, PyObject *);

static PyObject *py_hip_info(PyObject *self, PyObject *args);

char hip_Doc[] =
"A hip object is used to get information about\n\
and read data from an mpeg audio file.\n\
\n\
hip(f) will create a hip object; f can be\n\
either an open, readable file object or a filename string.\n\
\n\
The most useful method for a hip object is \"read\".";

PyTypeObject py_hip_type = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "hip",
  sizeof(py_hip),
  0,

  /* Standard Methods */
  /* destructor */ py_hip_file_dealloc,
  /* printfunc */ 0,
  /* getattrfunc */ py_hip_file_getattr,
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
  hip_Doc,
};


static PyMethodDef HIP_File_methods[] = {
  {"read", (PyCFunction) py_hip_read, 
   METH_VARARGS | METH_KEYWORDS, NULL},
  {"info", py_hip_info, 
   METH_VARARGS, NULL},
/*
  none of these are implemented yet.

  {"comment", py_hip_comment, 
   METH_VARARGS, py_hip_comment_doc},
  {"streams",  py_hip_streams,  
   METH_VARARGS, py_hip_streams_doc},
  {"seekable",  py_hip_seekable,  
   METH_VARARGS, py_hip_seekable_doc},
  {"bitrate",  py_hip_bitrate,  
   METH_VARARGS, py_hip_bitrate_doc},
  {"serialnumber",  py_hip_serialnumber,  
   METH_VARARGS, py_hip_serialnumber_doc},
  {"bitrate_instant",  py_hip_bitrate_instant,  
   METH_VARARGS, py_hip_bitrate_instant_doc},
  {"raw_total",  py_hip_raw_total,  
   METH_VARARGS, py_hip_raw_total_doc},
  {"pcm_total",  py_hip_pcm_total,  
   METH_VARARGS, py_hip_pcm_total_doc},
  {"time_total",  py_hip_time_total, 
   METH_VARARGS, py_hip_time_total_doc},
  {"raw_seek",  py_hip_raw_seek,  
   METH_VARARGS, py_hip_raw_seek_doc},
  {"pcm_seek_page",  py_hip_pcm_seek_page,  
   METH_VARARGS, py_hip_pcm_seek_page_doc},
  {"pcm_seek",  py_hip_pcm_seek,  
   METH_VARARGS, py_hip_pcm_seek_doc},
  {"time_seek",  py_hip_time_seek,  
   METH_VARARGS, py_hip_time_seek_doc},
  {"time_seek_page",  py_hip_time_seek_page,  
   METH_VARARGS, py_hip_time_seek_page_doc},
  {"raw_tell",  py_hip_raw_tell,  
   METH_VARARGS, py_hip_raw_tell_doc},
  {"pcm_tell",  py_hip_pcm_tell,  
   METH_VARARGS, py_hip_pcm_tell_doc},
  {"time_tell",  py_hip_time_tell,  
   METH_VARARGS, py_hip_time_tell_doc},
*/
  {NULL,NULL}
};

PyObject *
py_hip_new(PyObject *self, PyObject *args) /* change to accept kwarg */
{ 
  PyObject *ret;
  
  py_hip *newobj;

  newobj = PyObject_NEW(py_hip, &py_hip_type);

  ret = py_hip_open(newobj, args);
  if (ret == NULL) {
    PyMem_DEL(newobj);
    return NULL;
  } else
    Py_DECREF(ret);

  return (PyObject *) newobj;
}

static void
py_hip_file_dealloc(PyObject *self)
{
  if (PY_HIPFILE(self))
    hip_clear(PY_HIPFILE(self));

  PyMem_DEL(self);
}


static PyObject*
py_hip_file_getattr(PyObject *self, char *name)
{
  return Py_FindMethod(HIP_File_methods, self, name);
}

static PyObject *
py_hip_open(py_hip *self, PyObject *args)
{
  char *fname;
  char *initial = NULL;
  long ibytes = 0;
  FILE *file;

  PyObject *fobject;
  int retval;
  char errmsg[MSG_SIZE];
  
  if (PyArg_ParseTuple(args, "s|sl", &fname, &initial, &ibytes)) {

    file = fopen(fname, "r");

    if (file == NULL) {
      snprintf(errmsg, MSG_SIZE, "Could not open file: %s", fname);
      PyErr_SetString(PyExc_IOError, errmsg);
      return NULL;
    }

  } else if (PyArg_ParseTuple(args, "O!|sl", &PyFile_Type, &fobject,
			      &initial, &ibytes)) {
    PyErr_Clear(); /* clear first failure */

    file = PyFile_AsFile(fobject);

    if (file == NULL) 
      return NULL;

  } else {
    PyErr_SetString(PyExc_TypeError, 
		    "Argument 1 is not a filename or file object");
    return NULL;
  }

  self->hipf = (HIP_File*) malloc(sizeof(HIP_File));
  
  retval = hip_open(file, self->hipf, initial, ibytes);

  if (retval < 0)
    return v_error_from_code(retval, "Error opening file: ");

  Py_INCREF(Py_None);
  return Py_None;

}

static char *read_kwlist[] = {"length", "bigendian", "word", "signed", NULL};

static PyObject *
py_hip_read(PyObject *self, PyObject *args, PyObject *kwdict)
{
  py_hip * hip_self = (py_hip *) self;
  PyObject *retobj;
  int retval;

  PyObject *buffobj;
  PyObject *tuple;
  char *buff;
  
  int length, word, sgned, bitstream;
  int bigendianp;

  bigendianp = (__BYTE_ORDER == __BIG_ENDIAN);
  length = 4608;
  word = 2;
  sgned = 1;

  if (!PyArg_ParseTupleAndKeywords(args, kwdict, "|llll", read_kwlist,
				   &length, &bigendianp, &word, &sgned))
    return NULL;

  buffobj = PyBuffer_New(length);

  tuple = PyTuple_New(1);
  Py_INCREF(buffobj);
  PyTuple_SET_ITEM(tuple, 0, buffobj);

  if (!(PyArg_ParseTuple(tuple, "t#", &buff, &length))) {
    return NULL;
  }
  Py_DECREF(tuple);

  Py_BEGIN_ALLOW_THREADS
  retval = hip_read(hip_self->hipf, buff, length, 
		   bigendianp, word, sgned, &bitstream);
  Py_END_ALLOW_THREADS

  if (retval < 0) {
    Py_DECREF(buffobj);
    return v_error_from_code(retval, "Error reading file: ");
  }

  retobj = Py_BuildValue("Oii", buffobj, retval, bitstream);
  Py_DECREF(buffobj);
  return retobj;
}

static PyObject*
py_hip_info(PyObject *self, PyObject *args)
{
  py_hip *hip_self = (py_hip *) self;
  int link = -1;
  mpeg_info *mi;

  if (!PyArg_ParseTuple(args, "|i", &link))
    return NULL;

  mi = hip_info(hip_self->hipf, link);
  if (!mi) {
    PyErr_SetString(PyExc_RuntimeError, "Couldn't get info for hip.");
    return NULL;
  }
  
  return py_info_new_from_mpeg_info(mi);
}

/*
static PyObject *
py_hip_streams(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  long val;
  
  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_streams(hip_self->hipf);
  return PyInt_FromLong(val);
}

static PyObject *
py_hip_seekable(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  long val;
  
  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_seekable(hip_self->hipf);
  return PyInt_FromLong(val);
}

static PyObject *
py_hip_bitrate(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  long val;
  int i;
  
  if (!PyArg_ParseTuple(args, "i", &i)) 
    return NULL;

  val = hip_bitrate(hip_self->hipf, i);
  if (val < 0)
    return v_error_from_code(val, "Error getting bitrate: ");
  return PyInt_FromLong(val);
}

static PyObject *
py_hip_serialnumber(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  long val;
  int i;
  
  if (!PyArg_ParseTuple(args, "i", &i)) 
    return NULL;

  val = hip_serialnumber(hip_self->hipf, i);
  return PyInt_FromLong(val);
}

static PyObject *
py_hip_bitrate_instant(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  long val;

  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_bitrate_instant(hip_self->hipf);
  if (val < 0)
    return v_error_from_code(val, "Error getting bitrate_instant: ");
  return PyInt_FromLong(val);
}

static PyObject *
py_hip_raw_total(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  ogg_int64_t val;
  int i;

  if(!PyArg_ParseTuple(args, "i", &i)) 
    return NULL;

  val = hip_raw_total(hip_self->hipf, i);
  if (val < 0)
    return v_error_from_code(val, "Error in hip_raw_total: ");
  return PyLong_FromLongLong(val);
}

static PyObject *
py_hip_pcm_total(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  ogg_int64_t val;
  int i;

  if (!PyArg_ParseTuple(args, "i", &i)) 
    return NULL;

  val = hip_pcm_total(hip_self->hipf, i);
  if (val < 0)
    return v_error_from_code(val, "Error in hip_pcm_total: ");
  return PyLong_FromLongLong(val);
}

static PyObject *
py_hip_time_total(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  double val;
  int i;

  if (!PyArg_ParseTuple(args, "i", &i)) 
    return NULL;

  val = hip_time_total(hip_self->hipf, i);
  if (val < 0)
    return v_error_from_code(val, "Error in hip_time_total: ");
  return PyFloat_FromDouble(val);
}

static PyObject *
py_hip_raw_seek(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  int val;
  long pos;

  if(!PyArg_ParseTuple(args, "l", &pos)) 
    return NULL;

  val = hip_raw_seek(hip_self->hipf, pos);
  RETURN_IF_VAL(val, "Error in hip_raw_seek");
}

static PyObject *
py_hip_pcm_seek(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  PyObject *longobj;
  int val;
  ogg_int64_t pos;

  if(!PyArg_ParseTuple(args, "O", &longobj))
    return NULL;
 
  if (!modinfo->arg_to_int64(longobj, &pos))
    return NULL;

  val = hip_pcm_seek(hip_self->hipf, pos);
  RETURN_IF_VAL(val, "Error is hip_pcm_seek");
}

static PyObject *
py_hip_pcm_seek_page(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  int val;
  PyObject *longobj;
  ogg_int64_t pos;

  if(!PyArg_ParseTuple(args, "O", &longobj)) 
    return NULL;

  if (!modinfo->arg_to_int64(longobj, &pos))
    return NULL;

  val = hip_pcm_seek_page(hip_self->hipf, pos);
  RETURN_IF_VAL(val, "Error is hip_pcm_seek_page");
}

static PyObject *
py_hip_time_seek(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  int val;
  double pos;

  if(!PyArg_ParseTuple(args, "d", &pos)) 
    return NULL;

  val = hip_time_seek(hip_self->hipf, pos);
  RETURN_IF_VAL(val, "Error is hip_pcm_time_seek");
}

static PyObject *
py_hip_time_seek_page(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  int val;
  double pos;

  if(!PyArg_ParseTuple(args, "d", &pos)) 
    return NULL;

  val = hip_time_seek_page(hip_self->hipf, pos);
  RETURN_IF_VAL(val, "Error is hip_pcm_time_seek_page");
}

static PyObject *
py_hip_raw_tell(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  ogg_int64_t val;

  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_raw_tell(hip_self->hipf);
  return PyLong_FromLongLong(val);
}

static PyObject *
py_hip_pcm_tell(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  ogg_int64_t val;
  
  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_pcm_tell(hip_self->hipf);
  return PyLong_FromLongLong(val);
}

static PyObject *
py_hip_time_tell(PyObject *self, PyObject *args)
{
  py_hip * hip_self = (py_hip *) self;
  double val;
  
  if (!PyArg_ParseTuple(args, ""))
    return NULL;

  val = hip_time_tell(hip_self->hipf);
  return PyFloat_FromDouble(val);
}



static PyObject *
py_hip_comment(PyObject *self, PyObject *args)
{
  py_hip *hip_self = (py_hip *) self;
  vorbis_comment *comments;

  int link = -1;
  
  if (!PyArg_ParseTuple(args, "|i", &link))
    return NULL;

  comments = hip_comment(hip_self->hipf, link);
  if (!comments) {
    PyErr_SetString(PyExc_RuntimeError, "Couldn't get comments");
    return NULL;
  }

  return py_comment_new_from_vc(comments, self);
}

*/
