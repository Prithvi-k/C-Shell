#include "headers.h"

char *parent_path(const char *path)
{
    // Find the last occurrence of the path separator '/'
    const char *lastSlash = strrchr(path, '/');

    if (lastSlash == NULL)
    {
        // No slash found, parent path is the same as the input path
        return strdup(path);
    }
    else if (lastSlash == path)
    {
        // Path starts with '/', so parent path is '/'
        return strdup("/");
    }
    else
    {
        // Calculate the length of the parent path
        size_t parentPathLength = lastSlash - path;

        // Allocate memory for the parent path and copy the characters
        char *parentPath = (char *)malloc(parentPathLength + 1);
        if (parentPath == NULL)
        {
            // Memory allocation failed
            return NULL;
        }
        strncpy(parentPath, path, parentPathLength);
        parentPath[parentPathLength] = '\0'; // Null-terminate the parentPath

        return parentPath;
    }
}

int custom_strlen(char *string)
{
    int count = 0;
    char *temp = strdup(string);

    while (temp != NULL && *temp != '\0')
    {
        if (*temp != ' ' && *temp != '\t' && *temp != '\n')
        {
            count++;
        }
        temp++;
    }

    return count;
}

void peek(char *input, char *home_dir, char *prev_dir, pastevents *store)
{
    // printf("i/p = %c\n", input[1]);
    char temp[PATH_MAX];

    int all_entries_flag = 0, detailed_flag = 0;
    char *path = calloc(PATH_MAX, sizeof(char));

    if (custom_strlen(input) == 0 || input == NULL)
    {
        // printf("no path\n");
        path = ".";
    }
    else if (strcmp(input, "..") == 0)
    {
        getcwd(temp, PATH_MAX);
        path = parent_path(temp);
        // printf("%s\n", path);
    }
    else if (*input == '.')
    {
        path = ".";
    }
    else if (*input == '~')
    {
        path = home_dir;
    }
    else if (input[0] == '-' && input[1] == 'a' && input[2] == 'l')
    {
        // printf("-al\n");
        all_entries_flag = 1;
        detailed_flag = 1;
        if (custom_strlen(input) == 3)
        {
            path = ".";
        }
        else
        {
            path = input + 4;
        }
    }
    else if (input[0] == '-' && input[1] == 'l' && input[2] == 'a')
    {
        // printf("-la\n");
        all_entries_flag = 1;
        detailed_flag = 1;
        if (custom_strlen(input) == 3)
        {
            path = ".";
        }
        else
        {
            path = input + 4;
        }
    }
    else if (input[0] == '-' && input[1] == 'l' && input[3] == '-' && input[4] == 'a')
    {
        // printf("-l -a\n");
        all_entries_flag = 1;
        detailed_flag = 1;
        if (custom_strlen(input) == 4)
        {
            path = ".";
        }
        else
        {
            path = input + 6;
        }
    }
    else if (input[0] == '-' && input[1] == 'a' && input[3] == '-' && input[4] == 'l')
    {
        // printf("-a -l\n");
        all_entries_flag = 1;
        detailed_flag = 1;
        if (custom_strlen(input) == 4)
        {
            path = ".";
        }
        else
        {
            path = input + 6;
        }
    }
    else if (input[0] == '-' && input[1] == 'a')
    {
        // printf("-a\n");
        all_entries_flag = 1;
        if (custom_strlen(input) == 2)
        {
            // // printf("-a only\n");
            path = ".";
        }
        else
        {
            path = input + 3;
        }
    }
    else if (input[0] == '-' && input[1] == 'l')
    {
        // printf("-l\n");
        detailed_flag = 1;
        if (custom_strlen(input) < 4)
        {
            // // printf("-l only\n");
            path = ".";
        }
        else
        {
            path = input + 3;
        }
    }
    else if (*input == '-')
    {
        path = prev_dir;
    }
    else
    {
        printf("skipped all\n");
        path = strdup(input);
    }

    char *temp_path = (char *)calloc(PATH_MAX, sizeof(char *));

    if (*path == '.')
    {
        // printf("Current dir\n");
        getcwd(path, PATH_MAX);
    }
    else if (*path == '~' && custom_strlen(path) == 1)
    {
        // printf("Home dir\n");
        path = home_dir;
    }
    else if (*path == '~')
    {
        // printf("Home dir subfloder\n");
        path++;
        temp_path = strcat(home_dir, path);
        path = temp;
    }

    struct dirent **directory_elements;
    int item_count = scandir(path, &directory_elements, NULL, alphasort);

    if (item_count == -1)
    {
        perror("Failed to scan directory");
        // remove_last_event(store);
        exit(EXIT_FAILURE);
    }

    if (all_entries_flag)
    {
        if (detailed_flag)
        {
            for (int i = 0; i < item_count; i++)
            {
                struct stat file_stat;
                if (stat(directory_elements[i]->d_name, &file_stat) == 0)
                {
                    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
                    printf(" ");
                }
                else
                {
                    // remove_last_event(store);
                    perror("stat");
                }

                struct passwd *user_info = getpwuid(file_stat.st_uid);
                struct group *group_info = getgrgid(file_stat.st_gid);

                printf("%10s %10s %8lld ", user_info->pw_name, group_info->gr_name, (long long)file_stat.st_size);

                char time_buffer[50];
                strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stat.st_mtime));
                printf("%s ", time_buffer);

                if (directory_elements[i]->d_type == DT_DIR)
                {
                    printf(COLOR_BLUE "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if ((file_stat.st_mode & S_IXUSR) || (file_stat.st_mode & S_IXGRP) || (file_stat.st_mode & S_IXOTH))
                {
                    printf(COLOR_GREEN "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if (directory_elements[i]->d_type == DT_REG)
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
                else
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
            }
        }
        else
        {
            if (item_count < 0)
            {
                // remove_last_event(store);
                perror("Error scanning directory");
            }

            for (int i = 0; i < item_count; i++)
            {
                struct stat file_stat;
                if (lstat(path, &file_stat) != 0)
                {
                    perror("lstat");
                    continue;
                }
                if (directory_elements[i]->d_type == DT_DIR)
                {
                    printf(COLOR_BLUE "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if (directory_elements[i]->d_type == DT_REG && strstr(directory_elements[i]->d_name, ".exe") == 0 && strstr(directory_elements[i]->d_name, ".out") == 0)
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
                else if ((file_stat.st_mode & S_IXUSR) && (file_stat.st_mode & S_IXGRP) && (file_stat.st_mode & S_IXOTH))
                {
                    printf(COLOR_GREEN "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
            }
        }
    }
    else if (detailed_flag)
    {
        for (int i = 0; i < item_count; i++)
        {
            if (directory_elements[i]->d_name[0] != '.')
            {
                struct stat file_stat;
                if (stat(directory_elements[i]->d_name, &file_stat) == 0)
                {
                    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
                    printf(" ");
                }
                else
                {
                    // remove_last_event(store);
                    perror("stat");
                }

                struct passwd *user_info = getpwuid(file_stat.st_uid);
                struct group *group_info = getgrgid(file_stat.st_gid);

                printf("%10s %10s %8lld ", user_info->pw_name, group_info->gr_name, (long long)file_stat.st_size);

                char time_buffer[50];
                strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stat.st_mtime));
                printf("%s ", time_buffer);

                if (directory_elements[i]->d_type == DT_DIR)
                {
                    printf(COLOR_BLUE "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if ((file_stat.st_mode & S_IXUSR) || (file_stat.st_mode & S_IXGRP) || (file_stat.st_mode & S_IXOTH))
                {
                    printf(COLOR_GREEN "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if (directory_elements[i]->d_type == DT_REG)
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
                else
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
            }
        }
    }
    else
    {
        if (item_count < 0)
        {
            // remove_last_event(store);
            perror("Error scanning directory");
            return;
        }

        for (int i = 0; i < item_count; i++)
        {
            if (directory_elements[i]->d_name[0] != '.')
            {
                struct stat file_stat;
                // printf("%s\n", path);
                if (lstat(path, &file_stat) != 0)
                {
                    perror("lstat");
                    continue;
                }

                if (directory_elements[i]->d_type == DT_DIR)
                {
                    printf(COLOR_BLUE "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else if (directory_elements[i]->d_type == DT_REG && strstr(directory_elements[i]->d_name, ".exe") == 0 && strstr(directory_elements[i]->d_name, ".out") == 0)
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
                else if ((file_stat.st_mode & S_IXUSR) && (file_stat.st_mode & S_IXGRP) && (file_stat.st_mode & S_IXOTH))
                {
                    printf(COLOR_GREEN "%s" COLOR_RESET "\n", directory_elements[i]->d_name);
                }
                else
                {
                    printf("%s\n", directory_elements[i]->d_name);
                }
            }
        }
        // printf("\n");
    }
}