#include "minishell.h"

void	run_prompt(t_context *ctx)
{
	char	*line;
	char	**args;
	int		r;

	ft_printf("$> ");
	while ((r = get_next_line(0, &line)) == 1)
	{
		if ((args = parse(line, ctx)) != NULL && args[0] != NULL)
			exec(args, ctx);
		ft_memdel((void **)&line);
		if (args)
			clear_arr(args);
		ft_printf("$> ");
	}
}
