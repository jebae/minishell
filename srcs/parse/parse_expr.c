#include "minishell.h"

int			is_expr(char ch, char *token, int is_first)
{
	if (ft_strchr("\"'$\\", ch) != NULL)
		return (1);
	if (ch == '~' && ft_strlen(token) == 0 && is_first)
		return (1);
	return (0);
}

int			parse_expr(char *input, t_context *ctx, char **token)
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
	else if (*input == '~')
		r = parse_tilde(input, ctx, token);
	return (r);
}
