#include "minishell.h"

int		get_dirstack_history(char *query, t_list *stack, char **path)
{
	int			idx;
	t_list_node	*node;

	idx = ft_atoi(query);
	if (idx < 0)
		idx += stack->length;
	if (idx < 0 || idx >= (int)stack->length)
	{
		ft_fprintf(2, "minishell: not enough directory stack entries\n");
		return (-1);
	}
	node = stack->head;
	while (node && idx)
	{
		node = node->next;
		idx--;
	}
	*path = (char *)node->data;
	return (0);
}
