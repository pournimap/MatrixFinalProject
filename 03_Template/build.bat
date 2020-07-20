cls

del Source.exe

rc.exe Resources\Fire\Text.rc

cl.exe /c /EHsc Source.cpp  -I "Include\freetype\include"

cl.exe /c /EHsc Scene/Cloth/Cloth.cpp -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\include" -I "C:\ProgramData\NVIDIA Corporation\CUDA Samples\v11.0\common\inc"

nvcc.exe -c -o Scene/Cloth/Clothcu.obj Scene/Cloth/Cloth.cu -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\include" -I "C:\ProgramData\NVIDIA Corporation\CUDA Samples\v11.0\common\inc"

link.exe Source.obj Resources\Fire\Text.res Scene/Cloth/Clothcu.obj Cloth.obj user32.lib gdi32.lib kernel32.lib opengl32.lib "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\lib\x64\cudart.lib" "Include\ExternalLibraries\Assimp\Lib\assimp-vc140-mt.lib" "Include\freetype\win64\freetype.lib" /MACHINE:X64

del Source.obj Cloth.obj

Source.exe
