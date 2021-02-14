#include "minishell.h"

static int	is_builtin(char *cmd)
{
	static const char	*builtins[] = {
		"echo", "setenv", "env", "unsetenv", "cd", "exit", "dirs", NULL};
	int					i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	exec_builtin(char **args, t_context *ctx)
{
	if (ft_strcmp(args[0], (char *)"echo") == 0)
		return (cmd_echo(args + 1));
	else if (ft_strcmp(args[0], (char *)"setenv") == 0)
		return (cmd_setenv(args + 1, &ctx->envs));
	else if (ft_strcmp(args[0], (char *)"env") == 0)
		return (cmd_env(ctx->envs));
	else if (ft_strcmp(args[0], (char *)"unsetenv") == 0)
		return (cmd_unsetenv(args + 1, &ctx->envs));
	else if (ft_strcmp(args[0], (char *)"cd") == 0)
		return (cmd_cd(args + 1, ctx));
	else if (ft_strcmp(args[0], (char *)"dirs") == 0)
		return (cmd_dirs(&ctx->dir_stack));
	else if (ft_strcmp(args[0], (char *)"exit") == 0)
		return (cmd_exit(ctx));
	return (0);
}

int			exec(char **args, t_context *ctx)
{
	int		pid;
	int		status;
	char	*exe;

	if (is_builtin(args[0]))
		return (exec_builtin(args, ctx));
	if ((exe = get_executable(args[0], ctx->envs)) == NULL)
		return (-1);
	if ((pid = fork()) == 0)
		return (execve(exe, args, ctx->envs));
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		ft_memdel((void **)&exe);
		return (0);
	}
	else
	{
		ft_memdel((void **)&exe);
		kill(pid, SIGKILL);
		return (-1);
	}
}
