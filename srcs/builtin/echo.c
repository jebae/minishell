#include "minishell.h"

int			cmd_echo(char **args)
{
	while (*args)
	{
		ft_printf("%s", *args);
		args++;
		if (*args)
			ft_printf(" ");
	}
	ft_printf("\n");
	return (0);
}
