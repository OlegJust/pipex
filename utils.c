#include "pipex.h"

char *get_cmd_path(char *cmd, char **env)
{
    char **paths;
    char *full_path;
    int i = 0;

    while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
        i++;
    if (!env[i])
        return (NULL);
    paths = ft_split(env[i] + 5, ':');
    for (i = 0; paths[i]; i++)
    {
        full_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
    }
    return (NULL);
}
