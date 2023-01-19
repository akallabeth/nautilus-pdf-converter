#!/usr/bin/env python3
import os
import glob

files = glob.glob('locales/**/*.po', recursive=True)
for f in files:
    name, ext = os.path.splitext(f)
    command = 'msgfmt -o ' + name + '.mo ' + f
    print(command)
    os.system(command)
