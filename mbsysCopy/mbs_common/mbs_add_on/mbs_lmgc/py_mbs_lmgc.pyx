from cpython.version cimport PY_MAJOR_VERSION

cimport numpy as np
import numpy as np

cdef extern from "mbs_lmgc.h":
  ctypedef struct MbsLmgcSystem:
    int nb_mbs
  
  void mbs_lmgc_set_nb_mbs(int)
  void mbs_lmgc_set_mbs_file(int, char*)
  void mbs_lmgc_set_res_file(int, char*)
  void mbs_lmgc_set_anim_file(int, char*)
  void mbs_lmgc_set_node_sensor_ids(int, int, int*)
  int mbs_lmgc_get_node_nb(int)
  void mbs_lmgc_initialize()  
  void mbs_lmgc_finalize()  
  
  MbsLmgcSystem* globalSystem  


def setNbMbs(nbMbs):
  mbs_lmgc_set_nb_mbs(nbMbs)

def getNbMbs():
  return globalSystem.nb_mbs

  
def setMbsFile(iMbs, filename):
#  py_byte_string = mbsFile.encode('UTF-8')
#  cdef char* c_string = py_byte_string

  mbs_lmgc_set_mbs_file(iMbs, filename)

def setResFile(iMbs, filename):

  mbs_lmgc_set_res_file(iMbs, filename)

def setAnimFile(iMbs, filename):

  mbs_lmgc_set_anim_file(iMbs, filename)
  
  
def setNodeSensorIds(iMbs, nodeSensorIds):

  cdef np.ndarray[int, ndim=1, mode="c"] ids
  ids = np.array(nodeSensorIds, dtype=np.int32)

  mbs_lmgc_set_node_sensor_ids(iMbs, len(ids), &ids[0])
  
def getNbNodes(iMbs):
  return mbs_lmgc_get_node_nb(iMbs)
  
def initialize():
    mbs_lmgc_initialize()

def finalize():
    mbs_lmgc_finalize()
