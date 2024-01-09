#include "headers.h"

void prompt(char *home_dir, char *prompt_parameter)
{
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");

    char *user_name = getenv("USER");
    if (user_name == NULL)
    {
        perror("user_name getenv error");
        exit(1);
    }

    char *system_name = malloc(HOST_NAME_LENGTH);

    if (gethostname(system_name, HOST_NAME_LENGTH) != 0)
    {
        perror("gethostname error");
        exit(1);
    }

    // printf("<%s@%s> ", user_name, system_name);

    char *current_dir = calloc(PATH_MAX, sizeof(char));

    if (getcwd(current_dir, PATH_MAX) == NULL)
    {
        perror("current_dir getcwd error");
        exit(1);
    }

    if (prompt_parameter == NULL)
    {
        if (strncmp(current_dir, home_dir, strlen(home_dir)) == 0)
        {
            printf("<%s@%s:~%s> ", user_name, system_name, current_dir + strlen(home_dir));
        }
        else
        {
            printf("<%s@%s:%s> ", user_name, system_name, current_dir);
        }
    }
    else
    {
        if (strncmp(current_dir, home_dir, strlen(home_dir)) == 0)
        {
            printf("<%s@%s:~%s %s> ", user_name, system_name, current_dir + strlen(home_dir), prompt_parameter);
        }
        else
        {
            printf("<%s@%s:%s %s> ", user_name, system_name, current_dir, prompt_parameter);
        }

        prompt_parameter = NULL;
    }
}
