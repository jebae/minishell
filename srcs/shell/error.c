#include "minishell.h"

int		shell_error(const char *msg, char *arg)
{
	if (arg)
		ft_fprintf(2, "minishell: %s: %s\n", msg, arg);
	else
		ft_fprintf(2, "minishell: %s\n", msg);
	return (-1);
}
