import os, times, strutils
include "../../libs/ansi_colors.nim"

const nameWidth = 20

proc formatSize(size: int64): string =
  if size > 1_000_000:
    result = $(size div 1_000_000) & "M"
  elif size > 1_000:
    result = $(size div 1_000) & "K"
  else:
    result = $size

for kind, path in walkDir("."):
  let
    name = extractFilename(path)
    info = getFileInfo(path)
    sizeStr = if kind == pcDir: "-" else: formatSize(info.size)
    dateStr = info.lastWriteTime.format("yyyy-MM-dd HH:mm")
    namePadded = name.alignLeft(nameWidth)

  if kind == pcDir:
    echo BoldYellow & "📁 " & namePadded & " " & sizeStr.align(5) & " " & dateStr & " " & Reset
  else:
    echo BoldWhite & "📄 " & namePadded & " " & sizeStr.align(5) & " " & dateStr & " " & Reset
