import os

let args = commandLineParams()

if args.len == 0:
  stdout.write("\e[1;31m")   
  echo "Usage: program <file-name>"
  stdout.write("\e[0m")      
  quit(1)

if not fileExists(args[0]):
  writeFile(args[0], "")
  stdout.write("\e[1;32m")    
  echo "Created file: " & args[0]
  stdout.write("\e[0m")       
else:
  stdout.write("\e[1;31m")    
  echo "File already exists: " & args[0]
  stdout.write("\e[0m")       
