from building import *
import rtconfig

# get current directory
cwd     = GetCurrentDir()
# The set of source files associated with this SConscript file.
src     = Glob('csrc/*.c')
src    += Glob('sys/rt-thread/port/*.c')
src    += Glob('sys/rt-thread/examples/*.c')

path    = [cwd + '/']
path   += [cwd + '/csrc']
path   += [cwd + '/sys/rt-thread/port']

LOCAL_CCFLAGS = ''

group = DefineGroup('u8g2s', src, depend = [''], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')