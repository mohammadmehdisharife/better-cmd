import os

let args = commandLineParams()

if args.len == 0:
  echo "Usage: program <file-name>"
  quit(1)

if not fileExists(args[0]):
  writeFile(args[0], "")

echo "Created file: " & args[0]
