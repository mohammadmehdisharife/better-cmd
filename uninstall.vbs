Option Explicit

Dim shell, userPath, newPath, fso, scriptFolder, binPath

Set shell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

scriptFolder = fso.GetParentFolderName(WScript.ScriptFullName)

binPath = fso.BuildPath(scriptFolder, "bin")

userPath = shell.Environment("User")("PATH")

If InStr(1, userPath, binPath, vbTextCompare) > 0 Then
  newPath = Replace(userPath, binPath, "", 1, -1, vbTextCompare)
  
  newPath = Replace(newPath, ";;", ";")
  
  If Left(newPath,1) = ";" Then
    newPath = Mid(newPath, 2)
  End If
  
  If Right(newPath,1) = ";" Then
    newPath = Left(newPath, Len(newPath)-1)
  End If
  
  shell.Environment("User")("PATH") = newPath
  MsgBox "Removed " & binPath & " from user PATH.", vbInformation, "Done"
Else
  MsgBox binPath & " was not found in user PATH.", vbInformation, "No change"
End If
