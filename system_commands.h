#ifndef __SYSTEM_COMMANDS_H
#define __SYSTEM_COMMANDS_H

void system_command(char **arguments_to_pass, int number_of_tokens, char *home_dir, pid_t pid, int background_flag, pastevents *store, char *prompt_parameter);

#endif