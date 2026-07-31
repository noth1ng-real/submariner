# Submariner

Windows Malware Development

## Compilation
Compile `exe`:
```
cl -O2 /GS- /Fe:output.exe source.cpp
```
Compile `dll`:
```
cl /LD /O2 /GS- /FE:output.dll source.cpp
```