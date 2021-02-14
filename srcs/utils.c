#include "minishell.h"

size_t	get_arr_length(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	clear_arr(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		ft_memdel((void **)&arr[i]);
		i++;
	}
	ft_memdel((void **)&arr);
}

void	clear_context(t_context *ctx)
{
	clear_arr(ctx->envs);
	clear_list(&ctx->dir_stack);
	ft_memdel((void **)&ctx->pwd);
}
