#include "minishell.h"

int		parse_backslash(char *input, char **token)
{
	int		r;
	int		i;

	i = 1;
	if (input[1] == '\0')
		r = concat(token, (char *)"\\", 1);
	else
	{
		r = concat(token, input + 1, 1);
		i++;
	}
	return ((r == -1) ? -1 : i);
}
