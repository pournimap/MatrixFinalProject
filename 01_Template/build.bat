cls

del Source.exe

cl.exe /c /EHsc Source.cpp

link.exe Source.obj user32.lib gdi32.lib kernel32.lib opengl32.lib /MACHINE:X64

del Source.obj

Source.exe
