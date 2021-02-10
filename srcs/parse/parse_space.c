#include "minishell.h"

int		parse_space(char **token, char *input, t_list *tokens)
{
	int		i;

	if (ft_strlen(*token) > 0)
	{
		if (push_list_node(*token, tokens) == -1)
			return (-1);
		*token = ft_strnew(0);
		if (*token == NULL)
			return (-1);
	}
	i = 1;
	while (input[i] != '\0' && ft_iswhitespace(input[i]))
		i++;
	return (i);
}
