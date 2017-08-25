#!/usr/bin/env python
import tempfile, subprocess, os


def gtest_build(target_include, target_lib):
  tmp_dir = tempfile.mkdtemp()

  tar_name = "release-1.8.0.tar.gz"
  print("INFO : copying " + tar_name + " " + tmp_dir)
  os.chdir(tmp_dir)
  subprocess.check_call([ "wget", "https://github.com/google/googletest/archive/" + tar_name ])

  subprocess.check_call([ "tar", "xf", tar_name ])

  dir_name = "googletest-release-1.8.0/googletest"
  build_dir = tmp_dir + "/" + dir_name
  os.chdir(build_dir)
  subprocess.check_call(["cmake", "-DBUILD_SHARED_LIBS=ON", "."])
  subprocess.check_call(["make"])

  print("INFO : build under " + build_dir)

  #subprocess.check_call([ "cp", "-a", "include/gtest", target_include ])
  #subprocess.check_call([ "cp", "-a", "libgtest_main.so", "libgtest.so", target_lib ])

def get_target_dir(create):
  cur_dir = os.getcwd()
  target_dir = cur_dir + "/../third_party"
  target_include = target_dir + "/include"
  target_lib = target_dir + "/lib"

  if create == True:
    subprocess.check_call([ "mkdir", target_dir ])
    subprocess.check_call([ "mkdir", target_include ])
    subprocess.check_call([ "mkdir", target_lib ])

  return (target_include, target_lib)

def main():
  (target_include, target_lib) = get_target_dir(False)
  gtest_build(target_include, target_lib)

main()
