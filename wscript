import Options
from os import unlink, symlink, popen
from os.path import exists 

srcdir = "src"
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "libswipl"
  obj.find_sources_in_dirs("src")
  obj.lib = [ "swipl" ]
  obj.includes = ["./src"]
  obj.cxxflags = ["-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]

def shutdown():
  if Options.commands['clean']:
    if exists('libswipl.node'): unlink('libswipl.node')
  else:
    if exists('build/default/libswipl.node') and not exists('libswipl.node'):
      symlink('build/default/libswipl.node', 'libswipl.node')
    else:
	  if exists('build/Release/libswipl.node') and not exists('libswipl.node'):
	    symlink('build/Release/libswipl.node', 'libswipl.node')

