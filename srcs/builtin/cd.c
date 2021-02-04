#include "minishell.h"

static void	free_memory(char *backup[2], char *buf)
{
	ft_memdel((void **)&backup[0]);
	ft_memdel((void **)&backup[1]);
	ft_memdel((void **)&buf);
}

static int	rollback(char *backup[2], char *oldpwd, char ***envs_ptr)
{
	static const char	*keys[2] = {"OLDPWD", "PWD"};
	int					i;

	i = 0;
	while (i < 2)
	{
		if (backup[i])
		{
			if (set_env((char *)keys[i], backup[i], envs_ptr) == -1)
				break ;
		}
		else
		{
			if (unset_env((char *)keys[i], envs_ptr) == -1)
				break ;
		}
		i++;
	}
	chdir(oldpwd);
	free_memory(backup, oldpwd);
	return (-1);
}

static int	prepare_backup(char *backup[2], char **envs)
{
	static const char	*keys[2] = {"OLDPWD", "PWD"};
	char				*env;
	int					i;

	backup[0] = NULL;
	backup[1] = NULL;
	i = 0;
	while (i < 2)
	{
		env = get_env((char *)keys[i], envs);
		if (env)
		{
			backup[i] = ft_strdup(env);
			if (backup[i] == NULL)
				return (-1);
		}
		i++;
	}
	return (0);
}

static char	*get_path(char *arg, char **envs)
{
	if (ft_strcmp(arg, "~") == 0)
		return (get_env((char *)"HOME", envs));
	else if (ft_strcmp(arg, "-") == 0)
		return (get_env((char *)"OLDPWD", envs));
	return (arg);
}

int			cmd_cd(char **args, char ***envs_ptr)
{
	char	*backup[2];
	char	*buf;
	int		res;

	if (!is_valid_cd_arg(args))
		return (-1);
	buf = NULL;
	if (prepare_backup(backup, *envs_ptr) == -1 ||
		(buf = getcwd(NULL, 0)) == NULL)
	{
		free_memory(backup, buf);
		return (-1);
	}
	if ((res = chdir(get_path(args[0], *envs_ptr))) != -1)
	{
		if (set_env((char *)"OLDPWD", buf, envs_ptr) == -1)
			return (rollback(backup, buf, envs_ptr));
		ft_memdel((void **)&buf);
		if ((buf = getcwd(NULL, 0)) == NULL)
			return (rollback(backup, buf, envs_ptr));
		if (set_env((char *)"PWD", buf, envs_ptr) == -1)
			return (rollback(backup, buf, envs_ptr));
	}
	free_memory(backup, buf);
	return (res);
}
