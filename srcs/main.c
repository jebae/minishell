#include "minishell.h"

static int	err_with_free(t_context *ctx)
{
	clear_context(ctx);
	return (-1);
}

static int	set_context(char **arge, t_context *ctx)
{
	int		i;

	ft_memset(ctx, 0, sizeof(t_context));
	if ((ctx->pwd = getcwd(NULL, 0)) == NULL)
		return (-1);
	ctx->envs = (char **)ft_memalloc(
		sizeof(char *) * (get_arr_length(arge) + 1));
	if (ctx->envs == NULL)
		return (err_with_free(ctx));
	i = 0;
	while (arge[i])
	{
		if ((ctx->envs[i] = ft_strdup(arge[i])) == NULL)
			return (err_with_free(ctx));
		i++;
	}
	if (update_dirstack_history(ctx->pwd, &ctx->dir_stack) == -1)
		return (err_with_free(ctx));
	return (0);
}

int			main(int argc, char **args, char **arge)
{
	t_context	ctx;

	(void)argc;
	(void)args;
	set_context(arge, &ctx);
	run_prompt(&ctx);
	clear_context(&ctx);
	return (0);
}
