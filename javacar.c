#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HELP "--help"
#define VERSION "--version"
#define JAVACAR_VERSION "1.0.0" // Version Updates!!

void noargs(void);
void help(void);
int checkIfFileExists(char javaFileName[]);
int compile(char javaFileName[]);
int run(char javaFileName[]);

int main(int argc, char *argv[])
{
    int argArrayLength = argc;
    char **FileNames = argv;
    FILE *checkFilePointer;
    int fileExists = 0, compilationSuccess = 0, runSuccess = 0;
    // Purpose : For all .java files present in the FileNames array,
    // compile and run in sequence of arguments.

    // Handling two flags for fun
    if (argArrayLength == 1)
    {
        noargs();
        exit(1);
    }
    else if (strcmp(FileNames[1], HELP) == 0)
    {
        help();
        exit(0);
    }
    else if (strcmp(FileNames[1], VERSION) == 0)
    {
        printf("javacar %s.\n", JAVACAR_VERSION);
        exit(0);
    }

    // Compilations
    for (int i = 1; i < argArrayLength; i++)
    {
        printf("Program %d:\n\n", i);
        fileExists = checkIfFileExists(FileNames[i]);
        if (fileExists)
        {
            compilationSuccess = compile(FileNames[i]);
            if (compilationSuccess)
            {
                printf("Output: \n");
                runSuccess = run(FileNames[i]);
            }
            else
            {
                printf("ERROR: Compilation Failure. Check Source File: %s\n", FileNames[i]);
                printf("Type \"javacar --help\" for help.\n\n");
            }

            if (compilationSuccess && runSuccess)
            {
                printf("%s successfully compiled and run.\n\n", FileNames[i]);
            }
        }
        else
        {
            printf("ERROR: This file does not exist: %s\n", FileNames[i]);
            printf("Type \"javacar --help\" for help.\n\n");
        }
    }

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
        "Compiles your java program and then runs it.\n"
        "Usage: javacar [args]\n"
        "File names must always end with the .java extension.\n"
        "Currently only supports --version and --help flags.\n"
        "Maybe It'll support --silent in the future\n";

    puts(helptext);
    return;
}

int checkIfFileExists(char javaFileName[])
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

int compile(char javaFileName[])
{
    char command[1024];
    snprintf(command, sizeof(command), "javac %s 2>nul", javaFileName);
    int compilationStatus = system(command);
    return (compilationStatus == 0);
}

int run(char javaFileName[])
{
    char command[1024];
    int newLength = strlen(javaFileName) - 5;
    char newJavaFileName[100];
    strncpy(newJavaFileName, javaFileName, newLength);
    newJavaFileName[newLength] = '\0';
    snprintf(command, sizeof(command), "java %s", newJavaFileName);
    int runStatus = system(command);
    return (runStatus == 0);
}
