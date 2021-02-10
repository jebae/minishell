#ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/stat.h>
#ifdef __cplusplus
extern "C"
{
#endif
# include "libft.h"
# include "ft_printf.h"
#ifdef __cplusplus
}
#endif
# include <stdio.h>

typedef struct	s_dir_stack
{
	/* data */
}				t_dir_stack;

typedef struct	s_context
{
	char		*pwd;
	t_dir_stack	*dir_stack;
}				t_context;

int				cmd_echo(char **args);

int				env(char **envs);
int				get_env_idx(char *key, char **envs);
char			*get_env(char *key, char **envs);
int				get_envs_len(char **envs);
int				is_valid_env_key(char *key);

int				set_env(char *key, char *value, char ***envs_ptr);
int				cmd_setenv(char **args, char ***envs_ptr);

int				unset_env(char *key, char ***envs_ptr);
int				cmd_unsetenv(char **args, char ***envs_ptr);

int				cmd_cd(char **args, char ***envs_ptr, t_context *ctx);
int				is_valid_cd_arg(char **args);

char			**parse(char *input, char **envs, t_context *ctx);
int				concat(char **token, char *input, size_t len);
int				is_expr_char(char ch, char *token);
int				parse_expr(
	char **token, char *input, char **envs, t_context *ctx);
int				parse_backslash(char *input, char **token);
int				parse_dollar(char *input, char **envs, char **token);
int				parse_quote(char *input, char **token);
int				parse_dblquote(char *input, char **envs, char **token);
int				parse_space(char **token, char *input, t_list *tokens);
char			**list2array(t_list *list);

#endif
