# Dll_inject

**Simple process Dynamic Link Library (DLL) injector** this project is made for winapi (win32) for the x86 architecture **ONLY**

# How to use


Source.c:
- line #12 contains a variable named "path" put your DLL path inside the character constant (inside the quotes)
- line #72 youll see `HWND hWnd = FindWindowA(NULL, "DLL_TEST");` replace `DLL_TEST` with your process window name (if possible)
- the program will check if the DLL is accessible if not the program will halt and it will throw an exception
- **PLEASE BE RESPONSIBLE AND USE A VIRTUAL MACHINE TO PLAY WITH THIS TOOL AS THIS MAY CAUSE DATA CORRUPTION IN MEMORY**

Source_dll.c:
- none (this DLL was used for testing)

## Mentions:

I (the creator) am not responsible for any damages so please use responsibly (please do NOT use my code for malicious purposes)

compile with MSVC **ONLY** (visual studio has MSVC compiler built-in)

**I will not release the raw executable versions of this program for security reasons**
