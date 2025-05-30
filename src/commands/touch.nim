import os, strformat
include "../../libs/ansi_colors.nim"

let args = commandLineParams()

if args.len == 0:
  echo fmt"{BoldRed}Usage: touch <file-name>{Reset}"
  quit(1)

if not fileExists(args[0]):
  writeFile(args[0], "")
  echo fmt"{BoldGreen}Created file: {args[0]}{Reset}"
else:
  echo fmt"{BoldRed}File already exists: {args[0]}{Reset}"
