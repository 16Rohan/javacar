# Java Compile and Run (`javacar`)

## NOTE: <u>WINDOWS ONLY</u>

## INSTRUCTIONS

- Initially, use `javacar --help`
- All filename arguments **MUST** end with the `.java` file extension. The program will otherwise skip this file.
- Read `CHANGELOG.md`.

---

 **For practicality, add the .exe file to your System, PATH.**

## Steps to add `javacar` to PATH

1) Store the `javacar` executable in a permanent folder. For example: Create the directory `C:\Tools\JAVACAR`
2) Move the `javacar.exe` file into this directory.
3) Add the folder to your System PATH

    - Press `win+s` to open Windows Search.
    - In the **System Properties** window, click on **Environment Variables**.
    - Under **System Variables** or **User Variables** (depending upon your preference/usage), find, and select the **PATH** Variable.
    - Click **Edit**, then **New**, and add the address of your `JAVACAR` folder. Example: `C:\Tools\JAVACAR\`
    - Click OK -> OK -> OK.
4) Open a new terminal, navigate to your working directory, and type: `javacar [args]` or type `javacar --help` for help.

## Quirks

- Javacar will always display compilation, running and error messages. Flags to disable this behavior will be added in the future.
- `error.log` will only store compilation/runtime errors.

---

## Finally

Feel Free to Try it out and Edit it!
