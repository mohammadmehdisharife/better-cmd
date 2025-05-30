import os, strformat
include "../libs/ansi_colors.nim"

proc printFile(filename: string) =
  if not fileExists(filename):
    echo fmt"{BoldRed}cat: file '{filename}' does not exist.{Reset}"
    return

  try:
    for line in lines(filename):
      echo line
  except OSError as e:
    echo fmt"{BoldRed}cat: cannot open '{filename}': {e.msg}{Reset}"

when isMainModule:
  let args = commandLineParams()
  if args.len == 0:
    echo fmt"{BoldRed}cat: no files provided.{Reset}"
    quit(1)
  for file in args:
    printFile(file)
