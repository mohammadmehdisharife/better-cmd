Option Explicit

Dim shell, userPath, newPath, fso, scriptFolder, binPath

Set shell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

scriptFolder = fso.GetParentFolderName(WScript.ScriptFullName)

binPath = fso.BuildPath(scriptFolder, "bin")

userPath = shell.Environment("User")("PATH")

If InStr(1, userPath, binPath, vbTextCompare) = 0 Then
  If Len(userPath) > 0 Then
    newPath = userPath & ";" & binPath
  Else
    newPath = binPath
  End If
  shell.Environment("User")("PATH") = newPath
  MsgBox "Installed " & binPath & " to user PATH.", vbInformation, "Installation Complete"
Else
  MsgBox binPath & " is already in user PATH.", vbInformation, "No change"
End If
