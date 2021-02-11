#include "minishell.h"

static int	get_args_len(char **args)
{
	int		i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

static char	*get_path(char *arg, t_context *ctx)
{
	char	*path;
	char	*temp;

	if (arg == NULL)
	{
		if ((temp = get_env((char *)"HOME", ctx->envs)) == NULL)
			path = ft_strdup(ctx->pwd);
		else
			path = ft_strdup(temp);
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		if ((temp = get_env((char *)"OLDPWD", ctx->envs)) == NULL)
			path = NULL;
		else
			path = ft_strdup(temp);
	}
	else
		path = resolve_path(ctx->pwd, arg);
	return (path);
}

static int	file_err(const char *msg, char *arg)
{
	ft_fprintf(2, "cd: %s: %s\n", msg, arg);
	return (0);
}

static int	is_valid(char *path, char *arg)
{
	struct stat	st;

	if (path == NULL)
	{
		if (ft_strcmp(arg, "-") == 0)
			ft_fprintf(2, "cd: OLDPWD not set\n");
		else
			ft_fprintf(2, "cd: unexpected error\n");
		return (0);
	}
	if (access(path, F_OK) == -1)
		return (file_err("no such file or directory", arg));
	if (lstat(path, &st) == -1)
		return (file_err("no such file or directory", arg));
	if ((st.st_mode & S_IFMT) == S_IFLNK)
		stat(path, &st);
	if ((st.st_mode & S_IFMT) != S_IFDIR)
		return (file_err("not a directory", arg));
	if (access(path, X_OK) == -1)
		return (file_err("permission denied", arg));
	return (1);
}

int			cmd_cd(char **args, t_context *ctx)
{
	char	*path;

	if (get_args_len(args) > 1)
	{
		ft_fprintf(2, "cd: too many arguments\n");
		return (-1);
	}
	path = get_path(args[0], ctx);
	if (!is_valid(path, args[0]))
	{
		ft_memdel((void **)&path);
		return (-1);
	}
	if (set_env((char *)"OLDPWD", ctx->pwd, &ctx->envs) == -1
		|| set_env((char *)"PWD", path, &ctx->envs) == -1)
	{
		ft_memdel((void **)&path);
		return (-1);
	}
	ft_memdel((void **)&ctx->pwd);
	ctx->pwd = path;
	chdir(path);
	if (update_dirstack_history(path, &ctx->dir_stack) == -1)
		return (-1);
	return (0);
}
