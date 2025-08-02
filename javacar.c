#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define HELP "--help"
#define VERSION "--version"
#define SILENT "--silent"
#define SHOW_ERROR_MSGS "--errors"
#define CLEARLOGS "--clearlogs"
#define JAVACAR_VERSION "javacar 1.1.0" // Version Updates!!
#define TEMPFILE "tempfile.txt"
#define LOGFILE "error.log"
#define COMPILE_ERROR 11
#define RUNTIME_ERROR 12

void noargs(void);
void help(void);
void helpmessage(void);
void clearLogFile(void);
void deleteTempFile(void);
int logErrorMessage(int ERRORFLAG, const char *javaFileName);
int handleFlags(const int argArrayLength, char *FileNames[]);
int checkFileExtension(const char javaFileName[]);
int checkIfFileExists(const char javaFileName[]);
int compile(const char javaFileName[]);
int run(const char javaFileName[]);
int processFile(const char javaFileName[]);
char *getCurrentDateTimeString(void);
long getFileLength(char *fileName);

int main(int argArrayLength, char *FileNames[])
{
    // Purpose : For all .java files present in the FileNames array,
    // compile and run them in sequence.

    handleFlags(argArrayLength, FileNames);

    for (int i = 1; i < argArrayLength; i++)
    {
        printf("Program %d:\n", i);
        processFile(FileNames[i]);
    }
    puts("\nAll files processed.");
    deleteTempFile();
    return 0;
}

void noargs(void)
{
    printf("Usage: javacar [args]\n");
    printf("Use javacar --help for help.\n");
    printf("Note: All filenames must end in .java extension.\n");
    printf("javacar %s\n", JAVACAR_VERSION);
    return;
}

void help(void)
{
    const char *helptext =
        "Compiles your java program and then runs it\n"
        "Usage: 1. javacar [args]\n"
        "2. javacar --[flag]\n"
        "File names must always end with the .java extension\n"
        "Supported system flags: --help, --version, --clearlogs\n"
        "System flags discard following args if any.";

    puts(helptext);
    return;
}

void helpmessage(void)
{
    printf("Type \"javacar --help\" for help.\n");
    return;
}

void clearLogFile(void)
{
    FILE *fp = fopen(LOGFILE, "w");
    if (!(fp == NULL))
    {
        fclose(fp);
    }
    return;
}

void deleteTempFile(void)
{
    // remove() is a portable function from <stdio.h> to delete files.
    // It returns 0 on success. We check if the file exists first to avoid an
    // error message for cases where no temp file was created.
    if (checkIfFileExists(TEMPFILE) && remove(TEMPFILE) != 0)
    {
        printf("ERROR: Failed to Delete Temporary File.\n");
    }
}

int logErrorMessage(int ERRORFLAG, const char *javaFileName)
{
    FILE *logFile = fopen(LOGFILE, "a");
    char buffer[1024];
    if (logFile == NULL)
    {
        printf("ERROR: Failed to Open Log File. Check Directory Permissions.\n");
        return 1;
    }

    char *currentDateTime = getCurrentDateTimeString();
    const char *errorTypeStr = "ERROR"; // Default

    if (ERRORFLAG == COMPILE_ERROR)
    {
        errorTypeStr = "COMPILE ERROR";
    }
    else if (ERRORFLAG == RUNTIME_ERROR)
    {
        errorTypeStr = "RUNTIME ERROR";
    }
    // Correctly format the log entry with date, time, error type, and filename.
    snprintf(buffer, sizeof(buffer), "[%s] %s in \"%s\":\n", currentDateTime, errorTypeStr, javaFileName);
    fprintf(logFile, buffer);

    long fileSize = getFileLength(TEMPFILE);
    if (fileSize < 0)
    {
        perror("Failed to read file size");
        fclose(logFile);
        free(currentDateTime);
        return 1;
    }

    char *errorMsg = malloc(fileSize + 1); // +1 for null-terminator
    FILE *tempFile = fopen(TEMPFILE, "rb");

    if (errorMsg)
    {
        if (tempFile)
        {
            fread(errorMsg, 1, fileSize, tempFile);
            errorMsg[fileSize] = '\0'; // null terminator
            fputs(errorMsg, logFile);  // Print string from tempfile to error.log
            fclose(tempFile);
        }
        free(errorMsg);
    }

    fclose(logFile);
    free(currentDateTime);
    return 0;
}

int checkFileExtension(const char javaFileName[])
{
    const char *ext = strrchr(javaFileName, '.');
    // Instead of checking the full string, we check IF&Where the '.' exists
    // and then we check the presence of .java
    if (ext != NULL)
    {
        return strcmp(ext, ".java") == 0;
    }
    else
    {
        return 0; // No extension found
    }
}

int checkIfFileExists(const char javaFileName[])
{
    FILE *fp = fopen(javaFileName, "r");
    if (fp != NULL)
    {
        fclose(fp);
        return 1; // File exists
    }
    else
    {
        return 0; // File does not exist.
    }
}

int compile(const char javaFileName[])
{
    char command[1024];
    snprintf(command, sizeof(command), "javac %s 2>%s", javaFileName, TEMPFILE);
    // Disables Java inpterpreter error output console
    int compilationStatus = system(command);
    return (compilationStatus == 0);
}

int run(const char javaFileName[])
{
    char command[1024];
    size_t newLength = strlen(javaFileName) - 5; // Length without ".java"

    // Dynamically allocate memory to avoid buffer overflow
    char *newJavaFileName = malloc(newLength + 1);
    if (!newJavaFileName)
    {
        perror("Failed to allocate memory for filename");
        return 0; // Indicate failure
    }
    strncpy(newJavaFileName, javaFileName, newLength); // Copy the base name
    newJavaFileName[newLength] = '\0';                 // Null-terminate it

    snprintf(command, sizeof(command), "java %s 2>%s", newJavaFileName, TEMPFILE);
    int runStatus = system(command); // Execute the command
    free(newJavaFileName);           // Free the allocated memory
    return (runStatus == 0);
}

int processFile(const char javaFileName[])
{
    // Check File Extension, If Not return
    int compiled = 0, ran = 0;

    if (!checkFileExtension(javaFileName))
    {
        printf("ERROR: Unsupported File Extension.\n");
        helpmessage();
        return 1;
    }

    if (!checkIfFileExists(javaFileName))
    {
        printf("ERROR: File Does Not Exist.\n");
        helpmessage();
        return 1;
    }

    compiled = compile(javaFileName);
    if (!compiled)
    {
        printf("ERROR: File Could Not Be Compiled. Check Source Code.\n");
        helpmessage();
        logErrorMessage(COMPILE_ERROR, javaFileName);

        return 1;
    }

    printf("Output:\n");
    ran = run(javaFileName);
    if (!ran)
    {
        printf("ERROR: File execution halted. Check Source Code for Unhandled Exceptions.\n");
        helpmessage();
        logErrorMessage(RUNTIME_ERROR, javaFileName);
        return 1;
    }

    printf("File Successfully Compiled and Run.\n");
    return 0;
}

long getFileLength(char *fileName)
{
    FILE *fp = fopen(fileName, "rb"); // Prevents null char termination stuff
    if (!fp)
    {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

char *getCurrentDateTimeString(void)
{

    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (!local)
        return NULL;

    // Allocate buffer for the formatted string.
    char *buffer = malloc(256); // This memory is freed by the caller (logErrorMessage).
    if (!buffer)
        return NULL;

    strftime(buffer, 256, "%d-%m-%Y %H:%M:%S", local);
    return buffer;
}

int handleFlags(const int argArrayLength, char *FileNames[])
{
    if (argArrayLength == 1)
    {
        noargs();
        exit(1);
    }
    if (strncmp(FileNames[1], "--", 2) == 0)
    {
        if (strcmp(FileNames[1], HELP) == 0)
        {
            help();
            exit(0);
        }
        else if (strcmp(FileNames[1], VERSION) == 0)
        {
            puts(JAVACAR_VERSION);
            exit(0);
        }
        else if (strcmp(FileNames[1], CLEARLOGS) == 0)
        {
            clearLogFile();
            exit(0);
        }
        else
        {
            printf("ERROR: Unrecognised Flags.\n");
            helpmessage();
            exit(1);
        }
    }

    return 0;
}
