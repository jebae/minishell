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

typedef struct	s_context
{
	char	*pwd;
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

#endif
