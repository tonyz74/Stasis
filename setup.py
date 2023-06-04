import os
import os.path
from subprocess import run

if not os.path.exists("obj"):
    os.mkdir("obj")

cwd = os.getcwd()
os.chdir("obj")
objdir = os.getcwd()


def cmake(name):
    if not os.path.exists(name):
        os.mkdir(name)
    os.chdir(name)

    run(["cmake", os.path.join(cwd, "deps", name)])
    run(["make", "-j", "4"])

    # go back
    os.chdir(objdir)


cmake("glfw")
