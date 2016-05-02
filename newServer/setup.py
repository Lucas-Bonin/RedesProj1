from distutils.core import setup, Extension
setup(name='NewModuleTest', version='1.0',  \
      ext_modules=[Extension('FuncTesteDef', ['ExtPython.c'])])