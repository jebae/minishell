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

static void	move_node_to_top(
	t_list_node *node, t_list_node *prev, t_list *stack)
{
	if (node == stack->head)
		return ;
	prev->next = node->next;
	node->next = stack->head;
	stack->head = node;
	if (node == stack->tail)
		stack->tail = prev;
}

int			update_dirstack_history(char *path, t_list *stack)
{
	t_list_node	*node;
	t_list_node	*prev;
	char		*data;

	prev = NULL;
	node = stack->head;
	while (node && ft_strcmp((char *)node->data, path) != 0)
	{
		prev = node;
		node = node->next;
	}
	if (node)
	{
		move_node_to_top(node, prev, stack);
		return (0);
	}
	if ((data = ft_strdup(path)) == NULL)
		return (-1);
	if (pushleft_list_node(data, stack) == -1)
	{
		ft_memdel((void **)&data);
		return (-1);
	}
	return (1);
}
