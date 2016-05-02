from distutils.core import setup, Extension
setup(name='SERVER_AND_CLIENT_MODULE', version='1.0',  \
      ext_modules=[Extension('client', ['ClientPython.c'])])