#!/usr/bin/python3

import os
import os.path
from subprocess import run
from shutil import move

if not os.path.exists("obj"):
    os.mkdir("obj")

root = os.getcwd()
os.chdir("obj")
objdir = os.getcwd()


def cmake(name):
    if not os.path.exists(name):
        os.mkdir(name)
    os.chdir(name)
    run(["cmake", os.path.join(root, "deps", name)])
    run(["make", "-j", "4"])
    os.chdir(objdir)


# Build GLFW
cmake("glfw")


# Build WGPU Native
if not os.path.exists("wgpu-native"):
    os.mkdir("wgpu-native")
os.chdir(os.path.join(root, "deps", "wgpu-native"))
run(["cargo", "build", "--release"])
wgpu_res = os.path.join(objdir, "wgpu-native")
if not os.path.exists(wgpu_res):
    move("target/release", wgpu_res)
os.chdir(objdir)


