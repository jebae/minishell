#include "minishell.h"

int		cmd_exit(t_context *ctx)
{
	clear_context(ctx);
	exit(0);
	return (0);
}
