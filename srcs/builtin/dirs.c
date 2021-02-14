#include "minishell.h"

int				cmd_dirs(t_list *dir_stack)
{
	t_list_node		*dir;
	int				idx;

	dir = dir_stack->head;
	idx = 0;
	while (dir)
	{
		ft_printf("%d %s\n", idx, dir->data);
		dir = dir->next;
		idx++;
	}
	return (0);
}
