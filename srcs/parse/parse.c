#include "minishell.h"

static int	err_with_free(char *buf)
{
	ft_memdel((void **)&buf);
	return (-1);
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

static int	get_tokens(
	char *input, char **envs, t_context *ctx, t_list *tokens)
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
			i = (ft_iswhitespace(input[j]))
				? parse_space(&token, input + j, tokens)
				: parse_expr(&token, input + j, envs, ctx);
			if (i == -1)
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

char		**parse(char *input, char **envs, t_context *ctx)
{
	t_list		tokens;
	char		**res;

	init_list(&tokens);
	if (get_tokens(input, envs, ctx, &tokens) == -1)
	{
		clear_list(&tokens);
		return (NULL);
	}
	res = list2array(&tokens);
	clear_list(&tokens);
	return (res);
}
