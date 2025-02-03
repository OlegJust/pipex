#ifndef PIPEX_H
#define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include "libft/libft.h"

char    *ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
char	*get_cmd_path(char *cmd, char **env);

#endif
