#include "minishell.h"

static int	handle_file_err(char *msg, char *arg)
{
	ft_fprintf(2, "cd: %s: %s\n", msg, arg);
	return (0);
}

int			is_valid_cd_arg(char **args)
{
	int			i;
	struct stat	st;

	i = 0;
	while (args[i])
		i++;
	if (i > 1)
	{
		ft_fprintf(2, (char *)"cd: too many arguments\n");
		return (0);
	}
	if (ft_strcmp(args[0], "-") == 0 || ft_strcmp(args[0], "~") == 0)
		return (1);
	if (access(args[0], F_OK) == -1)
		return (handle_file_err((char *)"no such file or directory", args[0]));
	if (lstat(args[0], &st) != -1 && (st.st_mode & S_IFMT) != S_IFDIR)
		return (handle_file_err((char *)"not a directory", args[0]));
	if (access(args[0], X_OK) == -1)
		return (handle_file_err((char *)"permission denied", args[0]));
	return (1);
}
