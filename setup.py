import sys
import os

from distutils.core import setup, Extension


# It'd be really nice if it was straightforward to add --with-clucene
# and --with-boost options to the standard build Command, but this is
# not clear from the documentation. If it *is* easy, and I'm missing
# something, let me (wes.chow@s7labs.com) know... In the meantime, the
# following are build paths to be customized.


# On some systems, CLucene places CLucene/clucene-config.h in a
# non-standard location like /usr/lib, or /var
clucene_config_path = ''
clucene_prefix = ''

# Some systems name the boost library boost_python or boost_python-mt
boost_lib_name = 'boost_python'
boost_prefix = ''

extra_include_dir = []
extra_lib_dir = []

if boost_prefix:
    extra_include_dir.append(os.path.join(boost_prefix, 'include'))
    extra_lib_dir.append(os.path.join(boost_prefix, 'lib'))

if clucene_config_path:
    extra_include_dir.append(clucene_config_path)

if clucene_prefix:
    extra_include_dir.append(os.path.join(clucene_prefix, 'include'))
    extra_lib_dir.append(os.path.join(clucene_prefix, 'lib'))




cy_module = Extension('_cylucene',
                      include_dirs = extra_include_dir,
                      library_dirs = extra_lib_dir,
                      libraries = ['clucene', boost_lib_name],
                      sources = ['src/analysis.cpp', 'src/cylucene.cpp', 'src/document.cpp',
                                 'src/index.cpp', 'src/queryparser.cpp', 'src/search.cpp',
                                 'src/store.cpp', 'src/util.cpp'])

setup(name='cylucene',
      version='0.1',
      description='Python wrapper for CLucene',
      author='Nicolas Lara',
      author_email='nicolaslara@gmail.com',
      url='http://www.s7labs.com',
      py_modules = ['cylucene'],
      ext_modules = [cy_module]
     )
