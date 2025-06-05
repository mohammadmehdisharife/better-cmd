import os, strformat
include "../../libs/ansi_colors.nim"

for kind, path in walkDir("."):
  let name = extractFilename(path)
  if kind == pcDir:
    write(stdout, fmt"{BoldYellow}📁|{name}{Reset}  ")
  else:
    write(stdout, fmt"{BoldWhite}📄|{name}{Reset}  ")

write(stdout, "\n")
