import os, strformat
include "../../libs/ansi_colors.nim"

for kind, path in walkDir("."):
  let name = extractFilename(path)
  if kind == pcDir:
    echo fmt"{BoldYellow}[DIR ]  {name}{Reset}"
  else:
    echo fmt"{BoldWhite}[FILE]  {name}{Reset}"
