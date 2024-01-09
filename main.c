#include "headers.h"

int main()
{
    char home_dir[PATH_MAX], previous_dir[PATH_MAX];
    char *prompt_parameter = NULL;
    if (getcwd(home_dir, PATH_MAX) == NULL)
    {
        perror("getcwd error");
    }

    FILE *fp;
    pastevents *store_commands = (pastevents *)malloc(sizeof(pastevents));
    store_commands->count = 0;

    load_past_events(fp, store_commands);
    // printf("loaded. count = %d\n", store_commands->count);

    pid_t pid = getpid();

    while (1)
    {
        wait(NULL);

        prompt(home_dir, prompt_parameter);
        char input[4096];
        fgets(input, 4096, stdin);

        if (strstr(input, "pastevent") == NULL)
        {
            // printf("not found\n");
            add_past_event(input, fp, store_commands);
        }

        // printf("executing\n");
        execute_command(input, home_dir, previous_dir, pid, fp, store_commands, prompt_parameter);
    }
}
