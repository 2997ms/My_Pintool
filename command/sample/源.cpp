#include <windows.h>
#include <tchar.h>
#include <stdio.h>
 
#define BUFSIZE 4096
 
HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr,hInputFile, hStdout;

VOID WriteToPipe(VOID);
VOID ReadFromPipe(VOID);
VOID ErrorExit(LPSTR);
 
BOOL CreateChildProcess()
{
   TCHAR szCmdline[] = TEXT("C:\\Users\\wangchong\\Desktop\\sample\\Debug\\sample.exe");
   PROCESS_INFORMATION piProcInfo;
   STARTUPINFO siStartInfo;
   BOOL bFuncRetn = FALSE;
 
// Set up members of the PROCESS_INFORMATION structure.
 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure.
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO);
   siStartInfo.hStdError = hChildStdoutWr;
   siStartInfo.hStdOutput = hChildStdoutWr;
   siStartInfo.hStdInput = hChildStdinRd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process.
   
   bFuncRetn = CreateProcess(NULL,
      szCmdline,     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // use parent's current directory
      &siStartInfo,  // STARTUPINFO pointer
      &piProcInfo);  // receives PROCESS_INFORMATION
  
   if (bFuncRetn == 0)
      ErrorExit("CreateProcess failed\n");
   else
   {
      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
      return bFuncRetn;
   }
}

int _tmain(int argc, TCHAR *argv[])
{
   SECURITY_ATTRIBUTES saAttr;
   BOOL fSuccess;

// Set the bInheritHandle flag so pipe handles are inherited.
 
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;
// Get the handle to the current STDOUT.
 
   hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
 
// Create a pipe for the child process's STDOUT.
 
   if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
      ErrorExit("Stdout pipe creation failed\n");
// Ensure the read handle to the pipe for STDOUT is not inherited.
   SetHandleInformation( hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);
// Create a pipe for the child process's STDIN.
 
   if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
      ErrorExit("Stdin pipe creation failed\n");
// Ensure the write handle to the pipe for STDIN is not inherited.
 
   SetHandleInformation( hChildStdinWr, HANDLE_FLAG_INHERIT, 0);
 
// Now create the child process.
  
   fSuccess = CreateChildProcess();
   if (! fSuccess)
      ErrorExit("Create process failed with");
// Get a handle to the parent's input file.
 
   if (argc == 1)
      ErrorExit("Please specify an input file");
   printf( "\nContents of %s:\n\n", argv[1]);
   hInputFile = CreateFile(argv[1], GENERIC_READ, 0, NULL,
      OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
   if (hInputFile == INVALID_HANDLE_VALUE)
      ErrorExit("CreateFile failed");
 
// Write to pipe that is the standard input for a child process.
 
   WriteToPipe();
 
// Read from pipe that is the standard output for child process.
 
   ReadFromPipe();
 
   return 0;
}
