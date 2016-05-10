#include <iostream>
#include <cstdio>
#include <windows.h>
using namespace std;

string flag = "asdfghjk";
char val[15];

//不弹框的system，返回执行结果
BOOL system_hide(string CommandLine, string &exe_result)
{       
    SECURITY_ATTRIBUTES sa;      
    HANDLE hRead,hWrite;     
	
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);      
    sa.lpSecurityDescriptor = NULL;     
    sa.bInheritHandle = TRUE;     
	
    if (!CreatePipe(&hRead,&hWrite,&sa,0))      
    {     
        return FALSE;   
    }       
	
    STARTUPINFO si;       
    PROCESS_INFORMATION pi;        
    si.cb = sizeof(STARTUPINFO);     
    GetStartupInfo(&si);       
    si.hStdError = hWrite;     
    si.hStdOutput = hWrite;     
    si.wShowWindow = SW_HIDE;      
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;     
	   
    if (!CreateProcess(NULL, (char *)CommandLine.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))       
    {     
		return FALSE;     
    }     
	
    CloseHandle(hWrite);     
			
    char buffer[1024] = {0};     	
    DWORD bytesRead;       
	
    while(true)       		
    {     		
        memset(buffer,0,strlen(buffer));  
		
        if(ReadFile(hRead,buffer,1023,&bytesRead,NULL)==NULL)     			
            break;     
		
		exe_result += buffer;     		
    }       
    return TRUE;      
}

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
 
#define BUFSIZE 4096
 
HANDLE hChildStdinRd, hChildStdinWr, 
   hChildStdoutRd, hChildStdoutWr,
   hInputFile, hStdout;
 
BOOL CreateChildProcess(VOID);
VOID WriteToPipe(VOID);
VOID ReadFromPipe(VOID);
VOID ErrorExit(LPSTR);
 
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
 
BOOL CreateChildProcess()
{
   TCHAR szCmdline[]=TEXT("child");
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
 
VOID WriteToPipe(VOID)
{
   DWORD dwRead, dwWritten;
   CHAR chBuf[BUFSIZE];
 
// Read from a file and write its contents to a pipe.
 
   for (;;)
   {
      if (! ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL) ||
         dwRead == 0) break;
      if (! WriteFile(hChildStdinWr, chBuf, dwRead,
         &dwWritten, NULL)) break;
   }
 
// Close the pipe handle so the child process stops reading.
 
   if (! CloseHandle(hChildStdinWr))
      ErrorExit("Close pipe failed\n");
}
 
VOID ReadFromPipe(VOID)
{
   DWORD dwRead, dwWritten;
   CHAR chBuf[BUFSIZE];
// Close the write end of the pipe before reading from the
// read end of the pipe.
 
   if (!CloseHandle(hChildStdoutWr))
      ErrorExit("Closing handle failed");
 
// Read output from the child process, and write to parent's STDOUT.
 
   for (;;)
   {
      if( !ReadFile( hChildStdoutRd, chBuf, BUFSIZE, &dwRead,
         NULL) || dwRead == 0) break;
      if (! WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL))
         break;
   }
}
 
VOID ErrorExit (LPSTR lpszMessage)
{
   fprintf(stderr, "%s\n", lpszMessage);
   ExitProcess(0);
}
The following is the code for the child process. It uses the inherited handles for STDIN and STDOUT to access the pipe created by the parent. The parent process reads from its input file and writes the information to a pipe. The child receives text through the pipe using STDIN and writes to the pipe using STDOUT. The parent reads from the read end of the pipe and displays the information to its STDOUT.

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
   if ((hStdout == INVALID_HANDLE_VALUE) ||
      (hStdin == INVALID_HANDLE_VALUE))
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

/*
int main()
{
	string s="";
	cout << system_hide("C:\\Users\\wangchong\\Desktop\\sample\\Debug\\sample.exe \r\n asdfghjk \r\n",s) << endl;
	//printf("%s\n",s);

	system("pause");
	return 0;
}*/