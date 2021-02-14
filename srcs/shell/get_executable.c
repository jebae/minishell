#include "minishell.h"

static int	is_executable(char *path, int log)
{
	struct stat	st;

	if (access(path, F_OK) == -1)
	{
		if (log)
			shell_error("no such file or directory", path);
		return (0);
	}
	stat(path, &st);
	if ((st.st_mode & S_IFMT) == S_IFDIR)
	{
		if (log)
			shell_error("file is a directory", path);
		return (0);
	}
	if (access(path, X_OK) == -1)
	{
		if (log)
			shell_error("permission denied", path);
		return (0);
	}
	return (1);
}

static char	**get_paths(char *arg, char **envs)
{
	char	*path_env;
	char	**paths;
	char	*temp;
	int		i;

	if ((path_env = get_env("PATH", envs)) == NULL)
		return ((char **)ft_memalloc(sizeof(char *)));
	if ((paths = ft_strsplit(path_env, ':')) == NULL)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if ((temp = strcat_all(3, paths[i], "/", arg)) == NULL)
		{
			clear_arr(paths);
			return (NULL);
		}
		ft_memdel((void **)&paths[i]);
		paths[i] = temp;
		i++;
	}
	return (paths);
}

static char	*search_in_path_env(char *path, char **envs)
{
	char	**paths;
	char	*exe;
	int		i;
	int		file_exist;

	if ((paths = get_paths(path, envs)) == NULL)
		return (NULL);
	file_exist = 0;
	exe = NULL;
	i = -1;
	while (paths[++i])
	{
		if (access(paths[i], F_OK) == 0)
			file_exist = 1;
		if (is_executable(paths[i], 0))
			break ;
	}
	if (paths[i])
		exe = ft_strdup(paths[i]);
	else if (file_exist)
		shell_error("permission denied", path);
	else
		shell_error("command not found", path);
	clear_arr(paths);
	return (exe);
}

char		*get_executable(char *arg, char **envs)
{
	if (arg[0] == '/' || ft_strncmp(arg, "./", 2) == 0
		|| ft_strncmp(arg, "../", 3) == 0)
	{
		if (is_executable(arg, 1))
			return (ft_strdup(arg));
	}
	else
		return (search_in_path_env(arg, envs));
	return (NULL);
}
