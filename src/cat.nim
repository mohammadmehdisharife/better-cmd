import os, strformat

proc printFile(filename: string) =
  if not fileExists(filename):
    stdout.write("\e[1;31m")
    stderr.writeLine &"cat: file '{filename}' does not exist."
    stdout.write("\e[0m")
    return

  try:
    for line in lines(filename):
      echo line
  except OSError as e:
    stdout.write("\e[1;31m")   
    stderr.writeLine &"cat: cannot open '{filename}': {e.msg}"
    stdout.write("\e[0m")

when isMainModule:
  let args = commandLineParams()
  if args.len == 0:
    stdout.write("\e[1;31m")   
    echo "Usage: program <file-name>"
    stdout.write("\e[0m") 
    quit(1)
  for file in args:
    printFile(file)
