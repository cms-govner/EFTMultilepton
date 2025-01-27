import subprocess
import sys

# Python wrapper to the underlying check_anaTrees.C root macro, can be used as the executable
#   command in a lobster workflow

xsec = sys.argv[1]
infiles = sys.argv[2:]

with open('files.txt', 'w') as fd:
    for fn in infiles:
        fd.write('{}\n'.format(fn))

subprocess.check_call(["root", "-b", "-l", "-q", "check_anaTrees.C(\"output.root\", \"files.txt\", -1, {xsec},0)".format(xsec=xsec)])
