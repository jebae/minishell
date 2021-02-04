#include "minishell.h"

static int	is_valid_format(char *arg)
{
	while (*arg)
	{
		if (ft_iswhitespace(*arg) ||
			(!ft_isalnum(*arg) && *arg != '_'))
			return (0);
		arg++;
	}
	return (1);
}

static int	unset_by_idx(char ***envs_ptr, int idx)
{
	char	**envs;
	int		i;
	int		len;

	len = get_envs_len(*envs_ptr);
	if ((envs = (char **)ft_memalloc(sizeof(char *) * len)) == NULL)
		return (-1);
	i = 0;
	while (i < idx)
	{
		envs[i] = (*envs_ptr)[i];
		i++;
	}
	while (i < len - 1)
	{
		envs[i] = (*envs_ptr)[i + 1];
		i++;
	}
	ft_memdel((void **)&(*envs_ptr)[idx]);
	ft_memdel((void **)envs_ptr);
	*envs_ptr = envs;
	return (0);
}

/*
** return values
**  1: one env removed
**  0: no env removed
** -1: error
*/

int			unset_env(char *key, char ***envs_ptr)
{
	int		idx;

	if (!is_valid_format(key))
		return (-1);
	idx = get_env_idx(key, *envs_ptr);
	if (idx == -1)
		return (0);
	if (unset_by_idx(envs_ptr, idx) == -1)
		return (-1);
	return (1);
}

int			cmd_unsetenv(char **args, char ***envs_ptr)
{
	while (*args)
	{
		if (is_valid_format(*args))
		{
			if (unset_env(*args, envs_ptr) == -1)
				return (-1);
		}
		args++;
	}
	return (0);
}
