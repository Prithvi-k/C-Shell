#include "headers.h"

void warp(char *input, char *home_dir, char *previous_dir, pastevents *store)
{
    char *tokenised_input = strtok(input, " \t\n");

    while (tokenised_input != NULL)
    {
        // printf("Warping to %s\n", tokenised_input);
        if (strchr(tokenised_input, '-') != NULL)
        {
            char *temp_dir = strdup(previous_dir);
            getcwd(previous_dir, PATH_MAX);
            // printf("prev = %s\n", previous_dir);
            chdir(temp_dir);
            // printf("prev comm %s\n", previous_dir);
        }
        else
        {
            getcwd(previous_dir, PATH_MAX);

            if (strchr(tokenised_input, '~') != NULL)
            {
                chdir(strcat(home_dir, tokenised_input + 1));
                // printf("%s\n", tokenised_input);
            }
            else if (chdir(tokenised_input) != 0)
            {
                printf("Couldn't change directory");
                // remove_last_event(store);
                continue;
            }
            // printf("some else%s\n", previous_dir);
        }

        // printf("prev = %s\n", previous_dir);
        char *current_dir = calloc(PATH_MAX, sizeof(char));
        getcwd(current_dir, PATH_MAX);
        printf("%s\n", current_dir);

        tokenised_input = strtok(NULL, " ");
    }
}