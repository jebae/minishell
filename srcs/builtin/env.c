#include "minishell.h"

int		cmd_env(char **envs)
{
	while (*envs)
	{
		ft_printf("%s\n", *envs);
		envs++;
	}
	return (0);
}

int		get_env_idx(const char *key, char **envs)
{
	size_t	len;
	size_t	key_len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (envs[i])
	{
		key_len = ft_strchr(envs[i], '=') - envs[i];
		if (len == key_len && ft_strncmp(key, envs[i], len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*get_env(const char *key, char **envs)
{
	int		idx;

	idx = get_env_idx(key, envs);
	if (idx == -1)
		return (NULL);
	return (envs[idx] + ft_strlen(key) + 1);
}

int		get_envs_len(char **envs)
{
	int	i;

	i = 0;
	while (envs[i])
		i++;
	return (i);
}

int		is_valid_env_key(const char *key)
{
	int		i;

	i = 0;
	while (key[i] != '\0')
	{
		if (ft_iswhitespace(key[i]) ||
			(!ft_isalnum(key[i]) && key[i] != '_') ||
			(ft_isdigit(key[i]) && i == 0))
			return (0);
		i++;
	}
	return (1);
}
