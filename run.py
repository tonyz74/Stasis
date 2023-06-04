#!/usr/bin/python3

import os
import subprocess

os.chdir("build")

meson = subprocess.run(["meson", "compile"])
if meson.returncode != 0:
    print("[ERROR]: Failed to compile")
    exit(1)

subprocess.run(["./stasis"])
