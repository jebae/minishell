#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/stat.h>
# include <signal.h>
#ifdef __cplusplus
extern "C"
{
#endif
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"
#ifdef __cplusplus
}
#endif
# include <stdio.h>

typedef struct	s_context
{
	char		**envs;
	char		*pwd;
	t_list		dir_stack;
}				t_context;

int				cmd_echo(char **args);

int				cmd_env(char **envs);
int				get_env_idx(const char *key, char **envs);
char			*get_env(const char *key, char **envs);
int				get_envs_len(char **envs);
int				is_valid_env_key(const char *key);

int				set_env(char *key, char *value, char ***envs_ptr);
int				cmd_setenv(char **args, char ***envs_ptr);

int				unset_env(char *key, char ***envs_ptr);
int				cmd_unsetenv(char **args, char ***envs_ptr);

int				cmd_cd(char **args, t_context *ctx);
int				is_valid_cd_arg(char **args);

int				cmd_dirs(t_list *dir_stack);

int				cmd_exit(t_context *ctx);

char			**parse(char *input, t_context *ctx);
int				concat(char **token, char *input, size_t len);
int				is_expr(char ch, char *token, int is_first);
int				parse_expr(char *input, t_context *ctx, char **token);
int				parse_backslash(char *input, char **token);
int				parse_dollar(char *input, char **envs, char **token);
int				parse_quote(char *input, char **token);
int				parse_dblquote(char *input, char **envs, char **token);
int				parse_tilde(char *input, t_context *ctx, char **token);
char			**list2array(t_list *list);

int				get_dirstack_history(char *query, t_list *stack, char **path);
int				update_dirstack_history(char *path, t_list *stack);

int				shell_error(const char *msg, char *arg);
char			*get_executable(char *arg, char **envs);
int				exec(char **args, t_context *ctx);
void			run_prompt(t_context *ctx);

void			clear_arr(char **arr);
void			clear_context(t_context *ctx);
size_t			get_arr_length(char **arr);

#endif
