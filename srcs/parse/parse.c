#include "minishell.h"

static int	err_with_free(char *buf)
{
	ft_memdel((void **)&buf);
	return (-1);
}

static int	add_rest_token(char **token, char *input, t_list *tokens)
{
	size_t	len;

	len = ft_strlen(input);
	if (len > 0 && concat(token, input, len) == -1)
		return (-1);
	if (ft_strlen(*token) == 0)
	{
		ft_memdel((void **)token);
		return (0);
	}
	if (push_list_node(*token, tokens) == -1)
		return (-1);
	return (0);
}

static int	parse_special(
	char **token, char *input, t_context *ctx, t_list *tokens)
{
	int		i;

	if (is_expr_char(*input, *token))
		return (parse_expr(token, input, ctx));
	if (ft_strlen(*token) > 0)
	{
		if (push_list_node(*token, tokens) == -1)
			return (-1);
		*token = ft_strnew(0);
		if (*token == NULL)
			return (-1);
	}
	i = 1;
	while (input[i] != '\0' && ft_iswhitespace(input[i]))
		i++;
	return (i);
}

static int	get_tokens(char *input, t_context *ctx, t_list *tokens)
{
	char	*token;
	int		i;
	int		j;

	if ((token = ft_strnew(0)) == NULL)
		return (-1);
	i = 0;
	j = 0;
	while (input[j] != '\0')
	{
		if (is_expr_char(input[j], token) || ft_iswhitespace(input[j]))
		{
			if (input[j] != '~' && concat(&token, input + i, j - i) == -1)
				return (err_with_free(token));
			if ((i = parse_special(&token, input + j, ctx, tokens)) == -1)
				return (err_with_free(token));
			j += i;
			i = j;
		}
		else
			j++;
	}
	if (add_rest_token(&token, input + i, tokens) == -1)
		return (err_with_free(token));
	return (0);
}

char		**parse(char *input, t_context *ctx)
{
	t_list		tokens;
	char		**res;

	init_list(&tokens);
	if (get_tokens(input, ctx, &tokens) == -1)
	{
		clear_list(&tokens);
		return (NULL);
	}
	res = list2array(&tokens);
	clear_list(&tokens);
	return (res);
}
