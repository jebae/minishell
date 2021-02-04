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

static int	get_idx(char *key, char **arge)
{
	size_t	len;
	size_t	key_len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (arge[i])
	{
		key_len = ft_strchr(arge[i], '=') - arge[i];
		if (len == key_len && ft_strncmp(key, arge[i], len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	unset_by_idx(char ***arge_ptr, int idx, int len)
{
	char	**arge;
	int		i;

	if ((arge = (char **)ft_memalloc(sizeof(char *) * len)) == NULL)
		return (-1);
	i = 0;
	while (i < idx)
	{
		arge[i] = (*arge_ptr)[i];
		i++;
	}
	while (i < len - 1)
	{
		arge[i] = (*arge_ptr)[i + 1];
		i++;
	}
	ft_memdel((void **)&(*arge_ptr)[idx]);
	ft_memdel((void **)arge_ptr);
	*arge_ptr = arge;
	return (0);
}

int			unset_env(char *arg, char ***arge_ptr, int len)
{
	int		idx;

	if (!is_valid_format(arg))
		return (0);
	idx = get_idx(arg, *arge_ptr);
	if (idx == -1)
		return (0);
	if (unset_by_idx(arge_ptr, idx, len) == -1)
		return (-1);
	return (1);
}

int			cmd_unsetenv(char **args, char ***arge_ptr)
{
	int	len;
	int	removed;

	len = get_envs_len(*arge_ptr);
	while (*args)
	{
		if ((removed = unset_env(*args, arge_ptr, len)) == -1)
			return (-1);
		len -= removed;
		args++;
	}
	return (0);
}
