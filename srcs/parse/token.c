#include "minishell.h"

char		**list2array(t_list *list)
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

int			concat(char **token, char *input, size_t len)
{
	char	*s;

	s = strnewncat(*token, input, len);
	if (s == NULL)
		return (-1);
	ft_memdel((void **)token);
	*token = s;
	return (0);
}
