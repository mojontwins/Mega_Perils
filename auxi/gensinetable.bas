' modify and run

Dim As Integer i, maximum, amplitude, adjust
Dim As Double Pi, angle

Pi = 3.14159265359

Print "Sine table"

Input "Steps";maximum
Input "Amplitude";amplitude
Input "Adjust";adjust

For i = 0 To maximum - 1
	angle = 2 * Pi * i / maximum 
	print Trim (Str (Int (32768 + amplitude * sin (angle) + adjust))); 
	if i < maximum - 1 then print ", ";
Next i