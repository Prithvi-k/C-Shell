#include "headers.h"

void execute_command(char *input, char *home_dir, char *previous_dir, pid_t pid, FILE *fp, pastevents *pe, char *prompt_parameter)
{
    // int number_of_commands = 0;
    char *semicolon_tokenised_input[MAX_NUMBER_OF_COMMANDS], *semicolon_save_rest;

    char *semicolon_token = strtok_r(input, ";\t\n\0", &semicolon_save_rest);

    while (semicolon_token != NULL)
    {

        int background_flag = 0;
        if (strchr(semicolon_token, '&') != NULL)
        {
            background_flag = 1;
        }

        char *ampersand_save_rest, *ampersand_token = strtok_r(semicolon_token, "&\t\n\0", &ampersand_save_rest);

        while (ampersand_token != NULL)
        {
            char *spaces_save_rest, *spaces_token = strtok_r(ampersand_token, " \t\n\0", &spaces_save_rest), *final_input = (char *)calloc(PATH_MAX, sizeof(char *));
            int input_flag = 0, is_not_first = 0, number_of_tokens = 0;
            char **tokenised_command = (char **)calloc(MAX_NUMBER_OF_COMMANDS, sizeof(char *));
            // strcat(final_input, spaces_token);

            // printf("Test: %s\n", spaces_token);

            while (spaces_token != NULL)
            {
                if (strcmp(spaces_token, "warp") == 0)
                {
                    input_flag = 1;
                }
                else if (strcmp(spaces_token, "peek") == 0)
                {
                    input_flag = 2;
                }
                else if (strcmp(spaces_token, "seek") == 0)
                {
                    input_flag = 3;
                }
                else if (strcmp(spaces_token, "proclore") == 0)
                {
                    // printf("Test: %s\n", spaces_token);
                    input_flag = 4;
                }
                else if (strcmp(spaces_token, "pastevents") == 0)
                {
                    // printf("test\n");
                    input_flag = 5;
                }

                else if (strcmp(spaces_token, "exit") == 0 || strcmp(spaces_token, "quit") == 0 || strcmp(spaces_token, "q") == 0)
                {
                    input_flag = 13;
                }
                else
                {
                    if (input_flag == 0)
                    {
                        input_flag = 7;
                    }
                    if (is_not_first)
                    {
                        strcat(final_input, " ");
                    }
                    tokenised_command[number_of_tokens++] = spaces_token;
                    strcat(final_input, spaces_token);
                    is_not_first++;
                }

                spaces_token = strtok_r(spaces_save_rest, " \t\n\0", &spaces_save_rest);
            }

            if (input_flag == 1)
            {
                warp(final_input, home_dir, previous_dir, pe);
            }
            else if (input_flag == 2)
            {
                peek(final_input, home_dir, previous_dir, pe);
            }
            else if (input_flag == 3)
            {
                seek(final_input, home_dir, pe);
            }
            else if (input_flag == 4)
            {
                // printf("Test: %s\n", final_input);
                proclore(final_input, home_dir, pid, pe);
            }
            else if (input_flag == 5)
            {
                past_events_manager(final_input, fp, pe, home_dir, previous_dir, pid, prompt_parameter);
            }

            else if (input_flag == 7)
            {
                system_command(tokenised_command, number_of_tokens, home_dir, pid, background_flag, pe, prompt_parameter);
            }
            else if (input_flag == 13)
            {
                // remove_last_event(pe);
                exit(EXIT_SUCCESS);
            }

            ampersand_token = strtok_r(ampersand_save_rest, "&\t\n\0", &ampersand_save_rest);
        }

        semicolon_token = strtok_r(semicolon_save_rest, ";\t\n\0", &semicolon_save_rest);
    }

    return;
}