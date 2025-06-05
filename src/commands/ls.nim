import os, terminal, sequtils
include "../../libs/ansi_colors.nim"

proc padRight(s: string; width: int): string =
  if s.len >= width:
    return s
  else:
    var res = s
    for _ in 0 ..< (width - s.len):
      res.add(' ')
    return res

type Entry = object
  kind: PathComponent
  name: string

var entries: seq[Entry] = @[]

for kind, path in walkDir("."):
  entries.add Entry(kind: kind, name: extractFilename(path))

let maxNameLength = entries.mapIt(it.name.len).max
let padding = 5
let colWidth = maxNameLength + padding
let termWidth = terminalWidth()
let columns = max(1, termWidth div colWidth)

for i, e in entries:
  let symbol = if e.kind == pcDir: "📁" else: "📄"
  let color = if e.kind == pcDir: BoldYellow else: BoldWhite

  stdout.write color
  stdout.write symbol & "|"
  stdout.write padRight(e.name, colWidth - padding)
  stdout.write Reset

  if (i + 1) mod columns == 0:
    echo ""

if entries.len mod columns != 0:
  echo ""
