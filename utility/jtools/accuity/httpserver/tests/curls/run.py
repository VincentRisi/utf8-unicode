import sys, os, os.path
import glob
import argparse
import subprocess, shlex 

parser = argparse.ArgumentParser()
parser.add_argument('-H', '--host', type=str, default='localhost')
parser.add_argument('-P', '--port', type=str, default='32135')
parser.add_argument('rest', nargs='*', type=str)
args = parser.parse_args()
opts = args.rest
host = args.host

def main():
  files = glob.glob('*.*')
  for file in files:
    if file in ['run.py', 'temp.py', 'runall.sh', 'POST.sh','PUT.sh','PATCH.sh','GET.sh', 'DELETE.sh', 'OPTIONS.sh']:
      continue
    count = 0  
    for opt in opts:
      if not opt in file:
        continue
      count += 1
    if count == len(opts):
      name, ext = os.path.splitext(file)
      if ext == '.sh':
        command = '%s %s %s' % (file, args.host, args.port)
      elif ext == '.post':
        command = 'POST.sh %s %s' % (name, args.host, args.port)
      elif ext == '.put':
        command = 'PUT.sh %s %s' % (name, args.host, args.port)
      elif ext == '.patch':
        command = 'PATCH.sh %s %s' % (name, args.host, args.port)
      elif ext == '.options':
        command = 'OPTIONS.sh %s %s' % (name, args.host, args.port)
      parms = shlex.split("bash "+command)
      print (parms)
      subprocess.run(parms)
      

if __name__ == '__main__':
  main()
