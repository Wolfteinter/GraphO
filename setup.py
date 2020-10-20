from distutils.core import setup, Extension

module = Extension('graphO', sources = ['graphO.cpp'], language='c++')

setup (name = 'graphO',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [module])
