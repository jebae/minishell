#include "minishell.h"

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

char	**list2array(t_list *list)
{
	char		**arr;
	t_list_node	*token;
	int			i;
	int			err;

	arr = (char **)ft_memalloc(sizeof(char *) * (list->length + 1));
	if (arr == NULL)
		return (NULL);
	err = 0;
	i = 0;
	token = list->head;
	while (!err && token)
	{
		if ((arr[i++] = ft_strdup((char *)token->data)) == NULL)
			err = 1;
		token = token->next;
	}
	if (err)
	{
		clear_arr(arr);
		return (NULL);
	}
	return (arr);
}
