' Leemos dos capas y sacamos dos binarios
'

Dim As Integer pf, map_w, map_h, t_cerrojo
Dim Shared As Integer map (2, 1024, 1024)
Dim As Integer x, y, xx, yy, i, total_bolts
Dim As uByte d
Dim As String teh_out
Dim Shared As String bigData

Type COSAS
	t as Byte
	x as Byte
	y as Byte
End Type

Dim cosas (7) As COSAS

Sub ReadLayer (pf As Integer, w As Integer, h As Integer, layer As Integer)

	' Find pilot
	Dim As String linea
	Dim As String word
	Dim As Integer counter
	Dim As Integer valor
	Dim As Integer x, y, i
	Dim As String m
	
	Print "Parsing layer"; layer
	
	Do
		Line Input #pf, linea
		If Instr(linea, "const") Then Exit Do
	Loop
	
	bigData = ""
	Do
		Line Input #pf, linea
		linea = trim (linea)
		If Instr (linea, "};") Then Exit Do
		bigData = bigData & linea
	Loop
	
	counter = 260 * w * h
	x = 0
	y = 0
	
	bigData = bigData + ","
	word = ""
	For i = 1 To len (bigData)
		m = Mid (bigData, i, 1)
		
		If m = "," Then 
			valor = Val (trim (word))
			word = ""
			map (layer, y, x) = valor

		
			x = x + 1
			If x = w * 20 Then
				x = 0
				y = y + 1
			End If
		Else
			word = word + m
		End If
	
	Next i	
End Sub

Print "RehashMap"
Print

Print "Convierte un export de texto de mappy con dos capas"
Print "a dos archivos binarios ordenados por pantallas"
Print "para usar desde un programa BEX."
Print
Print "Pseudochurrera Genesis Copyleft 2013 The Mojon Twins"
Print

Print "Las pantallas son de 20x13 tiles fijos."
Print

If Command (1) = "" Or _
	Val (Command (2)) <= 0 Or _
	Val (Command (3)) <= 0 Or _
	Val (Command (4)) <= 0 Or _
	Command (5) = "" Then
	Print "USO:"
	Print "   rehasmap.exe mapa.txt map_w map_h t_cerrojo output 3rd"
	Print
	Print "output = bin para dos archivos binarios y un bolts.bex."
	Print "output = bex para un archivo bex pegable en fuente."
	Print "Si se especifica 3rd se procesa la tercera capa con objetos"
	Print "47 = bolt; 46 = movable"
	System
End If

Print "Working!"

Print
Print "Step 1: Read map layers from text"

pf = Freefile
Open Command (1) For Input As #pf
map_w = Val (Command (2))
map_h = Val (Command (3))
t_cerrojo = Val (Command (4))

ReadLayer (pf, map_w, map_h, 0)
ReadLayer (pf, map_w, map_h, 1)
If Command (6) = "3rd" Then
	ReadLayer (pf, map_w, map_h, 2)
End If
Close (pf)

Print "Step 2: Rehashing screens"

If Command (5) = "bex" Then
	Print "Writing map.bex"	
	pf = FreeFile
	Open "map.bex" For Output as #pf

End If

For i = 0 To 1
	
	If Command (5) = "bin" Then 
		pf = FreeFile
		Open "map." + Trim (Str (i)) + ".bin" For Binary as #pf
		Print "Writing map." + Trim (Str (i)) + ".bin"
	Else
		Print #pf, "map_layer_" + Trim (Str (i)) + ":"
	End If
	
	For yy = 0 to map_h - 1
		For xx = 0 To map_w - 1
			
			If Command (5) = "bin" Then
				For y = 0 To 12
					For x = 0 To 19
						d = map (i, 13 * yy + y, 20 * xx + x)
						Put #pf, , d
					Next x
				Next y
			Else
				teh_out = "    Data "
				For y = 0 To 12
					For x = 0 To 19
						teh_out = teh_out & str (map (i, 13 * yy + y, 20 * xx + x))
						If y <> 12 Or x <> 19 Then
							teh_out = teh_out + ","
						End If
					Next x
				Next y
				Print #pf, teh_out
			End If
		
		Next xx
	Next yy
	
	If Command (5) = "bin" Then
		Close (pf)
	Else
		Print #pf, " "
	End If
Next i

Print "Step 3: Finding bolts"

If Command (5) = "bin" Then
	Print "Writing bolts.bex"	
	pf = FreeFile
	Open "bolts.bex" For Output as #pf
End If

print #pf, "    ' Data n_pant, x, y"
print #pf, "map_bolts:"

total_bolts = 0
For yy = 0 To map_h - 1
	For xx = 0 To map_w - 1
		For y = 0 To 12
			For x = 0 To 19
				If map (1, 13 * yy + y, 20 * xx + x) = t_cerrojo Then
					total_bolts = total_bolts + 1
					print #pf, "    Data " & Trim (Str (yy * map_w + xx)) & ", " & Trim (Str (x)) & ", " & Trim (Str (y))
				End If
			Next x
		Next y
	Next xx
Next yy

print #pf, "    ' Total: " & Trim (Str (total_bolts))
Print "Found: "; total_bolts

If Command (6) = "3rd" Then
	If Command (5) = "bin" Then
		Close pf
		Print "Writing extra.bex"	
		pf = FreeFile
		Open "extra.bex" For Output as #pf
	End If
	
	print #pf, "    ' Data t, x, y ;; 1->key 2->bolt 3->block 4->life..."
	print #pf, "	' 8*3 = 24 bytes per screen"
	print #pf, "map_extra:"
	For yy = 0 To map_h - 1
		For xx = 0 To map_w - 1
			teh_out = "    Data "
			For i = 0 To 7
				cosas (i).t = 0
				cosas (i).x = 0
				cosas (i).y = 0
			Next i
			i = 0
			For y = 0 To 12
				For x = 0 To 19
					Select Case map (2, 13 * yy + y, 20 * xx + x)
						Case 44:
							cosas (i).t = 4
							cosas (i).x = x
							cosas (i).y = y
							i = i + 1
						Case 45:
							cosas (i).t = 1
							cosas (i).x = x
							cosas (i).y = y
							i = i + 1
						Case 46:
							cosas (i).t = 3
							cosas (i).x = x
							cosas (i).y = y
							i = i + 1
						Case 47:
							cosas (i).t = 2
							cosas (i).x = x
							cosas (i).y = y
							i = i + 1
					End Select
				Next x
			Next y
			For i = 0 To 7
				teh_out = teh_out & cosas(i).t & ", " & cosas (i).x & ", " & cosas (i).y
				If i < 7 then
					teh_out = teh_out & ", "
				End If
			Next i
			print #pf, teh_out
		Next xx
	Next yy	
End If

Close (pf)

Print "Done!"
