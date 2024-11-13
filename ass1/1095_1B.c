/***********************************************************************
 *
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 05.08.2024
 *
 * Assignment: Program to GET and SET the ENVIRONMENT VARIABLES and
 *              to know use of getenv and setenv system calls
 *
 *
 * Input Description: None
 *
 * Output Description: The required ENVIRONMENT VARIABLES along with
 *                      two new ENVIRONMENT VARIABLES are displayed
 *
 *
 * Compilation Command: gcc 1095_1B.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input: None
 * Sample Output:
/--------------------------------------------------
Program to GET and SET Environment Variables

USER: user
HOME: /home/usr/student/ug/yr/user
HOST: localhost.localdomain
ARCH: x86_64
DISPLAY: null
PRINTER: null
PATH: /usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/home/usr/student/ug/yr/user/.local/bin:/home/usr/student/ug/yr/user/bin


Setting new Environment Variables "NEW_USER" & "NEW_HOST"

NEW_USER: Clara
NEW_HOST: Apostos
---------------------------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int i;
    char const* env_var_names[] = { "USER", "HOME", "HOST", "ARCH", "DISPLAY", "PRINTER", "PATH" };

    printf("Program to GET and SET Environment Variables\n\n");

    for (i = 0; i < sizeof(env_var_names) / sizeof(env_var_names[0]); i++) {
        char* env_var = getenv(env_var_names[i]);
        printf("%s: %s\n", env_var_names[i], env_var ? env_var : "null");
    }

    printf("\n\nSetting new Environment Variables \"NEW_USER\" & \"NEW_HOST\"\n\n");

    char const* new_env_var_names[] = { "NEW_USER", "NEW_HOST" };
    if (setenv(new_env_var_names[0], "Clara", 1) < 0) {
        printf("Error setting new Environment Variable 1.");
    }
    if (setenv(new_env_var_names[1], "Apostos", 1) < 0) {
        printf("Error setting new Environment Variable 2.");
    }

    for (i = 0; i < sizeof(new_env_var_names) / sizeof(new_env_var_names[0]); i++) {
        char* new_env_var = getenv(new_env_var_names[i]);
        printf("%s: %s\n", new_env_var_names[i], new_env_var ? new_env_var : "null");
    }

    return 0;
}