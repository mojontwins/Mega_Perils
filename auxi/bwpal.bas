' 
Dim As Integer i
Dim As String palIn, palEntry, palOut
Dim as uByte r, g, b
Dim as uByte media

Print "paste pal (comma separated, no spaces, 4 hex digits per entry)"
Print "like... $0000,$0822,$0422,..."
line input palIn

palOut = ""
For i = 1 To Len (palIn) Step 6
	palEntry = Mid (palIn, i + 1, 4)
	r = Val ("&H" & mid (palEntry, 4, 1)) \ 2
	g = Val ("&H" & mid (palEntry, 3, 1)) \ 2
	b = Val ("&H" & mid (palEntry, 2, 1)) \ 2
	media = (r + g + g + b) \ 4

	palOut = palOut & "$0" & Hex (media * 2, 1) & Hex (media * 2, 1) & Hex (media * 2, 1) & ","
Next i

Print palOut
