#include <windows.h>
#define BUFSIZE 4096
 
VOID main(VOID)
{
   CHAR chBuf[BUFSIZE];
   DWORD dwRead, dwWritten;
   HANDLE hStdin, hStdout;
   BOOL fSuccess;
 
   hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
   hStdin = GetStdHandle(STD_INPUT_HANDLE);
   if ((hStdout == INVALID_HANDLE_VALUE) || (hStdin == INVALID_HANDLE_VALUE))
      ExitProcess(1);
 
   for (;;)
   {
   // Read from standard input.
      fSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
      if (! fSuccess || dwRead == 0)
         break;
 
   // Write to standard output.
      fSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);
      if (! fSuccess)
         break;
   }
}