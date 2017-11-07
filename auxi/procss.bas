Print "inputs cutstxt.txt and outputs cutstxt.bex"
Print "if you don't like this just change the bas file, dummy."

open "cutstxt.txt" for input as #1
open "cutstxt.bex" for output as #2

Dim As Integer ctscnt, i
Dim As String rdl

ctscnt = 0

While Not Eof (1)
	Line Input #1, rdl
	rdl = Ucase (Trim (rdl))
	If rdl = "" Then
		ctscnt = ctscnt + 1
		Print #2, "    Data 0"
		Print #2, "CutsTxt" & ctscnt & ":"
	Else
		Print #2, "    ' " & rdl
		Print #2, "    Data ";
		For i = 1 To Len (rdl)
			Print #2, "$" & Hex (Asc (Mid (rdl, i, 1)), 2);
			If i < Len (rdl) Then Print #2, ", "; Else Print #2, ", 0"
		Next i
	End If		
Wend

Close
