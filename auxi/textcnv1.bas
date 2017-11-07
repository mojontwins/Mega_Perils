'
Dim As Integer offset, tsoffset
Dim As Integer i
DiM As String s, m
input "offset = ' '";offset
input "TS_offset"; tsoffset
input "String = ";s

For i = 1 To Len (s)
	m = Mid (s, i, 1)
	if m = " " then print tsoffset; else print asc (m) + tsoffset + offset - 32;
	print ", ";
Next i
