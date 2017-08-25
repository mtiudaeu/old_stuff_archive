#!/usr/bin/env python

import tempfile
import getopt
import subprocess
import datetime
import sys
import os

#FIXME Could be set and read on install somewhere in the home directory.
install_path = "/opt/"
emsdk_dir_name = "emsdk_portable"
install_dest = install_path + emsdk_dir_name

#---------------------------------------------------------------------
def args_exit_error():
  print("Error : invalid arguments")
  args_exit_usage()

#---------------------------------------------------------------------
def args_exit_usage():
  print("-h : help")
  print("-i : download and build emscripten")
  print("-t : build test")
  print("-e : get environment script")
  print("-s : launch http server")
  sys.exit()

#---------------------------------------------------------------------
def args_parse(argv):
  arg_install = False
  arg_build_test = False
  arg_get_env = False
  arg_http_server = False
  try:
    opts, args = getopt.getopt(argv,"hites",[])
  except getopt.GetoptError:
    args_exit_error()

  for opt, arg in opts:
    if opt == '-h':
      args_exit_usage()
    elif opt in ("-i"):
      arg_install = True
    elif opt in ("-t"):
      arg_build_test = True
    elif opt in ("-e"):
      arg_get_env = True
    elif opt in ("-s"):
      arg_http_server = True

  return (arg_install, arg_build_test, arg_get_env, arg_http_server)

#---------------------------------------------------------------------
def emscripten_install():
  global install_path
  global emsdk_dir_name
  global install_dest
  if os.path.exists( install_dest ):
    raise Exception("emscripten is already installed in " + install_path)

  # Download and untar
  tmp_dir = tempfile.mkdtemp()
  os.chdir( tmp_dir )
  subprocess.check_call( "curl https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz -o emsdk-portable.tar.gz".split())
  subprocess.check_call("gunzip emsdk-portable.tar.gz".split())
  subprocess.check_call("tar -xf emsdk-portable.tar".split())

  # Move to destination path
  subprocess.check_call(("sudo mv " + emsdk_dir_name + " " + install_path).split())
  os.chdir( install_dest )

  # Build it
  subprocess.check_call("./emsdk update".split())
  subprocess.check_call("./emsdk install clang-incoming-64bit emscripten-incoming-64bit sdk-incoming-64bit".split())
  subprocess.check_call("./emsdk activate clang-incoming-64bit emscripten-incoming-64bit sdk-incoming-64bit".split())

  print( "INFO : emscripten was build in " + install_dest )

#---------------------------------------------------------------------
def build_test():
  env_var = os.environ.get("EMSCRIPTEN")
  if env_var is None :
    raise Exception("Missing environment. Need to run \"source <emscripten_root>/emsdk_env.sh\"")

  # Create test program
  subprocess.check_call( "mkdir hello".split() )
  os.chdir( "hello" )
  f = open('hello.c','w')
  f.write("#include <stdio.h>\n" )
  f.write("int main(int argc, char ** argv) {\n" )
  f.write("   printf(\"Hello, world!\\n\");\n" )
  f.write("}\n" )

  # Compile test program (web assembly on/off)
  # Web assembly subprocess.check_call( "emcc hello.c -s WASM=1 -o hello.html".split() )
  # WebGl subprocess.check_call( "emcc hello.c -s FULL_ES2=1 -o hello.html".split() )
  subprocess.check_call( "emcc hello.c -o hello.html".split() )

#---------------------------------------------------------------------
def get_environment_script():
  print("source " + install_dest + "/emsdk_env.sh")

#---------------------------------------------------------------------
def launch_http_server():
  #MDTMP SimpleHttpServer(8080)
  import SimpleHTTPServer
  import SocketServer
   
  PORT = 8080
  Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
  httpd = SocketServer.TCPServer(("", PORT), Handler)
  print "Server running on port ", PORT
  httpd.serve_forever()
  # http://localhost:8080/hello.html

#---------------------------------------------------------------------
def main():
  (arg_install, arg_build_test, arg_get_env, arg_http_server) = args_parse(sys.argv[1:])

  if arg_get_env:
    get_environment_script()
  elif arg_http_server:
    launch_http_server()
  elif arg_build_test:
    build_test()
  elif arg_install:
    emscripten_install()
  else:
    args_exit_error()

#---------------------------------------------------------------------
main()

