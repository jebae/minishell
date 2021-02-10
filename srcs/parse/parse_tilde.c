#include "minishell.h"

static int	concat_home(char **envs, char **token)
{
	char	*home;

	home = get_env((char *)"HOME", envs);
	if (home && concat(token, home, ft_strlen(home)) == -1)
		return (-1);
	return (1);
}

static int	parse_tilde_phrase(char *input, t_context *ctx, char **after)
{
	int		i;
	int		j;

	i = 1;
	j = 1;
	while (input[j] != '\0' && !ft_iswhitespace(input[j]))
	{
		if (input[j] == '\'' || input[j] == '"')
		{
			if (concat(after, input + i, j - i) == -1)
				return (-1);
			i = (input[j] == '\'')
				? parse_quote(input + j, after)
				: parse_dblquote(input + j, ctx->envs, after);
			if (i == -1)
				return (-1);
			j += i;
			i = j;
		}
		else
			j++;
	}
	if (i != j && concat(after, input + i, j - i) == -1)
		return (-1);
	return (j);
}

static int	is_valid(char *p)
{
	int		i;

	i = 0;
	if (p[0] == '+' || p[0] == '-')
		i++;
	while (p[i] != '\0' && ft_isdigit(p[i]))
		i++;
	if (p[i] == '/' || p[i] == '\0')
		return (1);
	return (0);
}

static int	concat_tilde_phrase(char *p, t_context *ctx, char **token)
{
	int		i;
	char	*path;

	i = 0;
	if (ft_isdigit(p[0]) || ft_isdigit(p[1]))
	{
		if (get_dirstack_history(p, &ctx->dir_stack, &path) == -1)
			return (-1);
		i = 1;
		while (p[i] != '\0' && ft_isdigit(p[i]))
			i++;
	}
	else if (p[0] == '+' && ++i)
		path = ctx->pwd;
	else if (p[0] == '-' && ++i)
		path = get_env((char *)"OLDPWD", ctx->envs);
	else
		path = get_env((char *)"HOME", ctx->envs);
	if (path && concat(token, path, ft_strlen(path)) == -1)
		return (-1);
	if (p[i] == '/')
		return (concat(token, p + i, ft_strlen(p + i)));
	return (0);
}

int			parse_tilde(char *input, t_context *ctx, char **token)
{
	int		i;
	int		r;
	char	*p;

	if (input[1] == '\0' || ft_iswhitespace(input[1]))
		return (concat_home(ctx->envs, token));
	if ((p = ft_strnew(0)) == NULL)
		return (-1);
	if ((i = parse_tilde_phrase(input, ctx, &p)) == -1)
	{
		ft_memdel((void **)&p);
		return (-1);
	}
	if (is_valid(p))
		r = concat_tilde_phrase(p, ctx, token);
	else
	{
		i = 1;
		while (input[i] == '~')
			i++;
		r = concat(token, input, i);
	}
	ft_memdel((void **)&p);
	return ((r == -1) ? -1 : i);
}
