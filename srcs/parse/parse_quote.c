#include "minishell.h"

int			parse_quote(char *input, char **token)
{
	int		i;
	int		j;

	i = 1;
	j = 1;
	while (input[j] != '\0' && input[j] != '\'')
	{
		if (input[j] == '\\' && input[j + 1] == '\'')
		{
			if (concat(token, input + i, j - i) == -1)
				return (-1);
			i = parse_backslash(input + j, token);
			if (i == -1)
				return (-1);
			j += i;
			i = j;
		}
		else
			j++;
	}
	if (i != j && concat(token, input + i, j - i) == -1)
		return (-1);
	return ((input[j] == '\'') ? j + 1 : j);
}

int			parse_dblquote(char *input, char **envs, char **token)
{
	int		i;
	int		j;

	i = 1;
	j = 1;
	while (input[j] != '\0' && input[j] != '"')
	{
		if (input[j] == '$' || (input[j] == '\\' && input[j + 1] == '"'))
		{
			if (concat(token, input + i, j - i) == -1)
				return (-1);
			i = (input[j] == '$')
				? parse_dollar(input + j, envs, token)
				: parse_backslash(input + j, token);
			if (i == -1)
				return (-1);
			j += i;
			i = j;
		}
		else
			j++;
	}
	if (i != j && concat(token, input + i, j - i) == -1)
		return (-1);
	return ((input[j] == '"') ? j + 1 : j);
}
