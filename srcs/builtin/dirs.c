#include "minishell.h"

void		cmd_dirs(t_context *ctx)
{
	t_list_node		*dir;
	int				idx;

	dir = ctx->dir_stack.head;
	idx = 0;
	while (dir)
	{
		ft_printf("%d %s\n", idx, dir->data);
		dir = dir->next;
		idx++;
	}
}
