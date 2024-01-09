#include "headers.h"

void system_command(char **arguments_to_pass, int number_of_tokens, char *home_dir, pid_t pid, int background_flag, pastevents *store, char *prompt_parameter)
{
    // printf("%s, %d\n", arguments_to_pass[0], background_flag);
    if (background_flag)
    {
        // Background process
        fflush(stdout);
        pid_t child = fork();

        if (child < 0)
        {
            perror("Error in forking");
            return;
        }
        else if (child == 0)
        {
            if (execvp(arguments_to_pass[0], arguments_to_pass) < 0)
            {
                perror("Error in background executing command");
            }
        }
        else
        {
            // Parent process
            printf("Background process started with PID %d\n", child);
        }
    }
    else
    {
        // Foreground process
        pid_t child = fork();

        if (child < 0)
        {
            fflush(stdout);
            perror("Error in forking");
            return;
        }
        else if (child == 0)
        {
            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);

            if (execvp(arguments_to_pass[0], arguments_to_pass) < 0)
            {
                perror("Error in foreground executing command");
                // remove_last_event(store);
            }

            gettimeofday(&end_time, NULL);

            // Calculate time taken in seconds
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                                  (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

            printf("%f", elapsed_time);
            if (prompt_parameter == NULL)
            {
                prompt_parameter = (char *)calloc(PATH_MAX, sizeof(char));
            }
            if (elapsed_time > 2.0)
            {
                snprintf(prompt_parameter, PATH_MAX, "%s : %f", arguments_to_pass[0], elapsed_time);
            }
            printf("prompt_p %s", prompt_parameter);
            fflush(stdout);

        }
        else
        {
            // Parent process
            waitpid(child, NULL, 0);
        }
    }
    return;
}