#include "minishell.h"

static int	is_valid_format(char *arg)
{
	int		i;

	if (arg == NULL)
		return (0);
	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (ft_iswhitespace(arg[i]) ||
			(!ft_isalnum(arg[i]) && arg[i] != '_') ||
			(ft_isdigit(arg[i]) && i == 0))
			return (0);
		i++;
	}
	if (i == 0 || arg[i] == '\0')
		return (0);
	return (1);
}

static int	add_env(char *env, char ***envs_ptr)
{
	char	**envs;
	char	*temp;
	int		i;
	int		len;

	len = get_envs_len(*envs_ptr);
	if ((envs = (char **)ft_memalloc(sizeof(char *) * (len + 2))) == NULL)
		return (-1);
	envs[len] = env;
	i = 0;
	while ((*envs_ptr)[i])
	{
		envs[i] = (*envs_ptr)[i];
		i++;
	}
	if (i > 0 && ft_strncmp(envs[i - 1], "_=", 2) == 0)
	{
		temp = envs[i - 1];
		envs[i - 1] = envs[i];
		envs[i] = temp;
	}
	ft_memdel((void **)envs_ptr);
	*envs_ptr = envs;
	return (0);
}

/*
** return values
**  1: one env added
**  0: one env updated
** -1: error
*/

int			set_env(char *key, char *value, char ***envs_ptr)
{
	char	*env;
	int		idx;

	if (!is_valid_env_key(key) || value == NULL)
		return (-1);
	env = strcat_all(3, key, "=", value);
	if (env == NULL)
		return (-1);
	idx = get_env_idx(key, *envs_ptr);
	if (idx == -1)
	{
		if (add_env(env, envs_ptr) == -1)
		{
			ft_memdel((void **)&env);
			return (-1);
		}
		return (1);
	}
	ft_memdel((void **)&(*envs_ptr)[idx]);
	(*envs_ptr)[idx] = env;
	return (0);
}

static int	split_arg(char *arg, char **key, char **value)
{
	int		i;

	*key = NULL;
	*value = NULL;
	i = 0;
	while (arg[i] != '=')
		i++;
	if ((*key = ft_strsub(arg, 0, i)) == NULL)
		return (-1);
	i++;
	if ((*value = ft_strsub(arg, i, ft_strlen(arg) - i)) == NULL)
	{
		ft_memdel((void **)key);
		return (-1);
	}
	return (0);
}

int			cmd_setenv(char **args, char ***envs_ptr)
{
	char	*key;
	char	*value;

	while (*args)
	{
		if (is_valid_format(*args))
		{
			if (split_arg(*args, &key, &value) == -1)
				return (-1);
			if (set_env(key, value, envs_ptr) == -1)
			{
				ft_memdel((void **)&key);
				ft_memdel((void **)&value);
				return (-1);
			}
			ft_memdel((void **)&key);
			ft_memdel((void **)&value);
		}
		args++;
	}
	return (0);
}
