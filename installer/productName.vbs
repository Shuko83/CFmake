Dim oWordApp, oWordDoc, oFSO
Set oWordApp = CreateObject("Word.Application")

oWordApp.Visible = False

Path = Left(WScript.ScriptFullName, InStr(WScript.ScriptFullName, WScript.ScriptName)-1)
docfullname = Path & "license.rtf"

Set oFSO = CreateObject("Scripting.FileSystemObject") 

If (oFSO.FileExists(docfullname)) Then
	WScript.Echo("License.rtf exists!")		
Else
	WScript.Echo("License.rtf does not exist!")
	WScript.Quit()
End If

oWordApp.Documents.Open(docfullname) 
set oWordDoc = oWordApp.ActiveDocument
'  change the properties ....
software = WScript.Arguments(0)	
oWordDoc.BuiltInDocumentProperties.Item("Subject") = software
oWordDoc.Fields.Update
WScript.Echo("Add " & software & " in the License.rtf file !")	

'  save       
oWordDoc.Save          
oWordApp.Quit
  
Set oWordDoc = Nothing
Set oWordApp = Nothing