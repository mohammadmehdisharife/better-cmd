import os

for kind, path in walkDir("."):
  let name = extractFilename(path)
  if kind == pcDir:
    stdout.write("\e[1;33m[DIR]  " & name & "\e[0m\n")
  else:
    stdout.write("\e[37;1m[FILE] " & name & "\e[0m\n")
