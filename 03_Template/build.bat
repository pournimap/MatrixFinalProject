cls

del Source.exe

cl.exe /c /EHsc Source.cpp

cl.exe /c /EHsc Scene/Cloth/Cloth.cpp -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2\include" -I "C:\ProgramData\NVIDIA Corporation\CUDA Samples\v10.2\common\inc"

::nvcc.exe -c -o Clothcu.obj Cloth.cu -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2\include" -I "C:\ProgramData\NVIDIA Corporation\CUDA Samples\v10.2\common\inc"

link.exe Source.obj Scene/Cloth/Clothcu.obj Cloth.obj user32.lib gdi32.lib kernel32.lib opengl32.lib "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2\lib\x64\cudart.lib" /MACHINE:X64

del Source.obj Cloth.obj

Source.exe
