#include "headers.h"

void print_relative_path_modified_for_proclore(const char *full_path, const char *home_dir)
{
    // printf("full_path = %s\n", full_path);
    // printf("home_dir = %s\n", home_dir);

    if (strstr(full_path, home_dir) == full_path)
    {
        const char *relative_path = full_path + strlen(home_dir);
        if (relative_path[0] == '/')
        {
            relative_path++;
        }
        printf("~/%s\n", relative_path);
    }
    else
    {
        printf("%s\n", full_path);
    }
}

void get_process_details(char *input_path, pid_t pid, char *home_dir, pastevents *store)
{
    char *path = (char *)calloc(PATH_MAX, sizeof(char));

    if (strlen(input_path) == 0)
    {
        // printf("path is empty\n");
        snprintf(path, PATH_MAX, "/proc/%d/stat", pid);
    }
    else
    {
        // printf("path is not empty\n");
        snprintf(path, PATH_MAX, "/proc/%s/stat", input_path);
    }

    // printf("file path: %s\n", path);

    FILE *file_ptr = fopen(path, "r");
    if (file_ptr == NULL)
    {
        perror("fopen error");
        // remove_last_event(store);
        return;
    }

    char **all_params_stat = (char **)calloc(50, sizeof(char *)), *file = (char *)calloc(4096, sizeof(char *));
    int param_count = 0;

    while (fgets(file, 4096, file_ptr) != NULL)
    {
        char *token = strtok(file, " \t\n\0");
        while (token != NULL)
        {
            all_params_stat[param_count++] = token;
            token = strtok(NULL, " \t\n\0");
        }
    }

    // pid
    printf("pid : %s\n", all_params_stat[0]);
    // Process Status(R / R + / S / S + / Z)
    char *process_status = strdup(all_params_stat[2]);
    // if foreground process, cat '+'
    char *process_status_char = all_params_stat[2];
    char *process_group_id_str = all_params_stat[4], *foreground_group_id_str = all_params_stat[7];
    int process_group_id = atoi(process_group_id_str), foreground_group_id = atoi(foreground_group_id_str);
    // printf("process group id : %d\n", process_group_id);
    // printf("foreground group id : %d\n", foreground_group_id);
    if (process_group_id != foreground_group_id)
    {
        process_status_char = strcat(process_status_char, "+");
    }
    printf("process status : %s\n", all_params_stat[2]);
    // Process group
    printf("process group : %s\n", all_params_stat[4]);
    // Virtual Memory
    printf("virtual memory : %s\n", all_params_stat[22]);
    // Executable path of process
    char exec_path[PATH_MAX];
    sprintf(exec_path, "/proc/%s/exe", all_params_stat[0]);
    readlink(exec_path, exec_path, PATH_MAX);
    // printf("exec path : %s\n", exec_path);
    printf("exec path : ");
    print_relative_path_modified_for_proclore(exec_path, home_dir);

    // for (int i = 0; i < param_count; i++)
    // {
    //     printf("%s ", all_params_stat[i]);
    // }

    char *line = NULL;
}

void proclore(char *path, char *name, pid_t pid, pastevents *store)
{
    // printf("path: %s\n", path);
    get_process_details(path, pid, name, store);
}