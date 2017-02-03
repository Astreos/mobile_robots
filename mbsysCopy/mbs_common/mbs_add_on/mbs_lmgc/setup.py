import sys
import os
import shutil

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

prjLibName = 'Robotran_Lmgc90' 
robotranCommonDir = '../../../'  
projectDir = '../../../../MBprojects/MultiplePendulum/'

incl_directories= [robotranCommonDir+'mbs_common/mbs_load_xml',
                   robotranCommonDir+'mbs_common/mbs_module',
                   robotranCommonDir+'mbs_common/mbs_struct',
                   robotranCommonDir+'mbs_common/mbs_utilities',
                   projectDir+'userfctR',
                   '/usr/include/libxml2',
                   '/usr/local/include/libxml2']


# build "myext.so" python extension to be added to "PYTHONPATH" afterwards...
setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [
        Extension("py_mbs_lmgc", 
                  sources=["py_mbs_lmgc.pyx" ],
                  libraries=[prjLibName,"xml2", "m","gsl","gslcblas"],     
                  include_dirs = incl_directories,
                  extra_compile_args=["-Wno-strict-prototypes","-Wno-comment", "-Wno-unused-function"],
                  extra_link_args=["-L"+projectDir+"workR/build/"]
             )
        ]
)
