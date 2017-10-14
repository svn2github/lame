#include <hip.h>
#include <stdio.h>

#include "hipmodule.h"

static PyMethodDef hip_methods[] = {
  {"hip", py_hip_new, 
   METH_VARARGS, py_hip_doc},
  {NULL, NULL}
};

static char docstring[] = "";
PyObject *
v_error_from_code(int code, char *msg)
{
  char errmsg[MSG_SIZE];
  char *reason;

  switch (code) {
  case HIP_FALSE: 
    reason = "Function returned FALSE.";
    break;
  case HIP_HOLE: 
    reason = "Interruption in data.";
    break;
  case HIP_EREAD: 
    reason = "Read error.";
    break;
  case HIP_EFAULT: 
    reason = "Internal logic fault. Bug or heap/stack corruption.";
    break;
  case HIP_EIMPL: 
    reason = "Bitstream uses unimplemented feature.";
    break;
  case HIP_EINVAL: 
    reason = "Invalid argument.";
    break;
  case HIP_ENOTMPEG: 
    reason = "Data is not MPEG data.";
      break;
  case HIP_EBADHEADER: 
    reason = "Invalid MPEG bitstream header.";
    break;
  case HIP_EVERSION: 
    reason = "MPEG version mismatch.";
    break;
  case HIP_ENOTAUDIO: 
    reason = "Packet data is not audio.";
    break;
  case HIP_EBADPACKET: 
    reason = "Invalid packet.";
    break;
  case HIP_EBADLINK: 
    reason = "Invalid stream section, or the requested link is corrupt.";
    break;
  case HIP_ENOSEEK: 
    reason = "Bitstream is not seekable.";
    break;
  default:
      reason = "Unknown error.";
  }

  strncpy(errmsg, msg, MSG_SIZE);
  strncat(errmsg, reason, MSG_SIZE - strlen(errmsg));
  PyErr_SetString(Py_HIPError, errmsg);
  return NULL;
}

void 
inithip(void)
{
  PyObject *module, *dict;

  module = Py_InitModule("hip", hip_methods);
  dict = PyModule_GetDict(module);
  
  Py_HIPError = PyErr_NewException("hip.HIPError", 
				      NULL, NULL);
  PyDict_SetItemString(dict, "HIPError", 
		       Py_HIPError);
  
  PyDict_SetItemString(dict, "__doc__", 
		       PyString_FromString(docstring));

  PyDict_SetItemString(dict, "__version__", 
		       PyString_FromString(VERSION));

  if (PyErr_Occurred())
    PyErr_SetString(PyExc_ImportError, 
		    "hip: init failed");
}

