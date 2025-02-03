/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opidhorn <opidhorn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 11:37:41 by opidhorn          #+#    #+#             */
/*   Updated: 2025/02/03 12:31:48 by opidhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void execute_cmd(char *cmd, char **env)
{
    char **args = ft_split(cmd, ' '); // Разбиваем команду на массив строк
    if (!args || !args[0])
    {
        perror("Split failed");
        exit(EXIT_FAILURE);
    }

    char *path = get_cmd_path(args[0], env);
    if (!path)
    {
        perror("Command not found");
        exit(127);
    }

    execve(path, args, env);
    perror("Exec failed");
    exit(EXIT_FAILURE);
}

void child_process(char *cmd, int input_fd, int output_fd, char **env)
{
    if (dup2(input_fd, STDIN_FILENO) < 0 || dup2(output_fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }
    close(input_fd);
    close(output_fd);
    execute_cmd(cmd, env);
}

int main(int argc, char **argv, char **env)
{
    if (argc != 5)
    {
        ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
        return (1);
    }

    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd1 < 0 || fd2 < 0)
    {
        perror("Open failed");
        return (1);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) < 0)
    {
        perror("Pipe failed");
        return (1);
    }

    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        perror("Fork failed");
        return (1);
    }
    if (pid1 == 0)
    {
        close(pipe_fd[0]);
        child_process(argv[2], fd1, pipe_fd[1], env);
    }

    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("Fork failed");
        return (1);
    }
    if (pid2 == 0)
    {
        close(pipe_fd[1]);
        child_process(argv[3], pipe_fd[0], fd2, env);
    }

    close(fd1);
    close(fd2);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return (0);
}
