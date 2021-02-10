#include "minishell.h"

int			parse_dollar(char *input, char **envs, char **token)
{
	int		i;
	int		r;
	char	*env;
	char	*key;

	i = 1;
	while (input[i] != '\0'
		&& (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (i == 1)
		r = concat(token, (char *)"$", 1);
	else
	{
		key = ft_strsub(input, 1, i - 1);
		if (key == NULL)
			return (-1);
		env = get_env(key, envs);
		ft_memdel((void **)&key);
		r = (env == NULL) ? 0 : concat(token, env, ft_strlen(env));
	}
	if (r == -1)
		return (-1);
	return (i);
}
