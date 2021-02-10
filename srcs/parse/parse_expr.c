#include "minishell.h"

int			is_expr_char(char ch, char *token)
{
	if (ft_strchr("\"'$\\", ch) != NULL)
		return (1);
	if (ch == '~' && ft_strlen(token) == 0)
		return (1);
	return (0);
}

int			parse_expr(char **token, char *input, t_context *ctx)
{
	int		r;

	(void)ctx;
	r = 1;
	if (*input == '$')
		r = parse_dollar(input, ctx->envs, token);
	else if (*input == '"')
		r = parse_dblquote(input, ctx->envs, token);
	else if (*input == '\'')
		r = parse_quote(input, token);
	else if (*input == '\\')
		r = parse_backslash(input, token);
	return (r);
}
