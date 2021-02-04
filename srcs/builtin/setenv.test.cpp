#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

TEST(set_env, add_env_to_empty)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *));
	arge[0] = NULL;

	// test
	ASSERT_EQ(set_env((char *)"FOO", (char *)"bar", &arge), 1);
	ASSERT_EQ(get_envs_len(arge), 1);
	ASSERT_STREQ(arge[0], "FOO=bar");

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, add_env_without__)
{
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	ASSERT_EQ(set_env((char *)"FOO", (char *)"bar", &arge), 1);
	ASSERT_EQ(get_envs_len(arge), len + 1);
	for (int i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	ASSERT_STREQ(arge[len], "FOO=bar");

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, add_env_with__)
{
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=/usr/bin"
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	ASSERT_EQ(set_env((char *)"FOO", (char *)"bar", &arge), 1);
	ASSERT_EQ(get_envs_len(arge), len + 1);
	for (int i=0; i < len - 1; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	ASSERT_STREQ(arge[len - 1], "FOO=bar");
	ASSERT_STREQ(arge[len], "_=/usr/bin");

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, update)
{
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=/usr/bin"
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*expected[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=moa",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=/usr/bin"
	};
	ASSERT_EQ(set_env((char *)"LESS", (char *)"moa", &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len);
	for (int i=0; i < len; i++) {
		ASSERT_STREQ(expected[i], arge[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, add_empty_value)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *));
	arge[0] = NULL;

	// test
	ASSERT_EQ(set_env((char *)"FOO", (char *)"", &arge), 1);
	ASSERT_EQ(get_envs_len(arge), 1);
	ASSERT_STREQ(arge[0], "FOO=");

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, update_empty_value)
{
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=/usr/bin"
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*expected[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=/usr/bin"
	};
	ASSERT_EQ(set_env((char *)"LESS", (char *)"", &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len);
	for (int i=0; i < len; i++) {
		ASSERT_STREQ(expected[i], arge[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(set_env, invalid_key)
{
	char		**arge;
	const char	*keys[] = {
		" FOO",
		"FOO ",
		"=FOO",
		"FOO%",
		"FOO!",
		"FOO#",
		"FOO^",
		"FOO-",
		"FOO+",
		"FOO+",
		"FOO,",
		"FOO.",
		"FOO;",
		"1001",
	};

	// prepare
	arge = (char **)malloc(sizeof(char *));
	arge[0] = NULL;

	// test
	for (int i=0; i < (int)(sizeof(keys) / sizeof(char *)); i++) {
		ASSERT_EQ(set_env((char *)keys[i], (char *)"bar", &arge), -1);
	}
	ASSERT_EQ(get_envs_len(arge), 0);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_one_env_to_empty)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *));
	arge[0] = NULL;

	// test
	const char	*args[] = {"FOO=bar", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	while (arge[i]) i++;
	ASSERT_EQ(i, 1);
	ASSERT_STREQ(arge[0], args[0]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_multi_env_to_empty)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *));
	arge[0] = NULL;

	// test
	const char	*args[] = {"FOO=bar", "TOYSTORY==1 ", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	while (arge[i]) i++;
	ASSERT_EQ(i, 2);
	ASSERT_STREQ(arge[0], args[0]);
	ASSERT_STREQ(arge[1], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_one_env_without__)
{
	char		**arge;
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"FOO=bar", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	for (i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 1);
	ASSERT_STREQ(arge[i - 1], args[0]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_multi_env_without__)
{
	char		**arge;
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"FOO= bar", "TOO=taa ", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	for (i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 2);
	ASSERT_STREQ(arge[i - 2], args[0]);
	ASSERT_STREQ(arge[i - 1], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_one_env_with__)
{
	char		**arge;
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=./a.out",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"FOO=bar", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	for (i=0; i < len - 1; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 1);
	ASSERT_STREQ(arge[i - 2], args[0]);
	ASSERT_STREQ(arge[i - 1], envs[len - 1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_multi_env_with__)
{
	char		**arge;
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=./a.out",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"FOO=bar", "TOO=taa", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	for (i=0; i < len - 1; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 2);
	ASSERT_STREQ(arge[i - 3], args[0]);
	ASSERT_STREQ(arge[i - 2], args[1]);
	ASSERT_STREQ(arge[i - 1], envs[len - 1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, update_one)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"_=./a.out",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"LESS=lesmoa", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	for (i=0; i < len - 1; i++) {
		if (i != 1)
			ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len);
	ASSERT_STREQ(arge[1], args[0]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, update_multiple)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"FOO=bar",
		"_=./a.out",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"LESS=lesmoa", "FOO=bee", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	while (arge[i]) i++;
	ASSERT_EQ(i, len);
	ASSERT_STREQ(arge[1], args[0]);
	ASSERT_STREQ(arge[3], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, update_and_create)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"FOO=bar",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"LESS=lesmoa", "TOO=taa", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 1);
	ASSERT_STREQ(arge[1], args[0]);
	ASSERT_STREQ(arge[i - 1], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, duplicate_env)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"FOO=bar",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"TOO=too", "TOO=taa", NULL};
	int			i = 0;

	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 1);
	ASSERT_STREQ(arge[i - 1], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, invalid_format)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {
		"FOO",
		" FOO=1",
		"FOO =1",
		"=FOO=1",
		"FOO%=1",
		"FOO!=1",
		"FOO#=1",
		"FOO^=1",
		"FOO-=1",
		"FOO+=1",
		"FOO+=1",
		"FOO,=1",
		"FOO.=1",
		"FOO;=1",
		"100=1",
		NULL,
	};
	int			i = 0;

	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	::testing::internal::GetCapturedStderr();

	ASSERT_EQ(get_envs_len(arge), len);
	for (i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, invalid_format_with_valid)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"FOO=bar",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"LESS =lessmoa", "TOO=taa", NULL};
	int			i = 0;

	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	::testing::internal::GetCapturedStderr();

	for (i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 1);
	ASSERT_STREQ(arge[i - 1], args[1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_setenv, add_multi_env_invalid_included_with__)
{
	char		**arge;
	const char	*envs[] = {
		"ZSH_DISABLE_COMPFIX=true",
		"GOPATH=/Users/go",
		"ZSH=/Users/.oh-my-zsh",
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
		"_=./a.out",
	};
	int			len = (int)(sizeof(envs) / sizeof(char *));

	// prepare
	arge = (char **)malloc(sizeof(char *) * (len + 1));
	arge[len] = NULL;

	for (int i=0; i < len; i++) {
		arge[i] = (char *)malloc(strlen(envs[i]) + 1);
		strcpy(arge[i], envs[i]);
	}

	// test
	const char	*args[] = {"FOO=bar", "IM = INVALID", "TOO=taa", NULL};
	int			i = 0;

	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_setenv((char **)args, &arge), 0);
	::testing::internal::GetCapturedStderr();

	for (i=0; i < len - 1; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}
	while (arge[i]) i++;
	ASSERT_EQ(i, len + 2);
	ASSERT_STREQ(arge[i - 3], args[0]);
	ASSERT_STREQ(arge[i - 2], args[2]);
	ASSERT_STREQ(arge[i - 1], envs[len - 1]);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}
