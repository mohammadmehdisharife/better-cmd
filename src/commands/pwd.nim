import os, strutils

proc main() =
  let cwd = getCurrentDir()
  let home = getEnv("HOME", getEnv("USERPROFILE"))

  if cwd.startsWith(home):
    let relativePath = cwd[home.len..^1]
    if relativePath.len == 0:
      echo "~"
    else:
      echo "~" & replace(relativePath, "\\", "/")
  else:
    echo replace(cwd, "\\", "/")  

main()
