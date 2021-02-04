#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

TEST(unset_env, key_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*expected[] = {
		"PAGER=less",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};
	ASSERT_EQ(unset_env((char *)"LESS", &arge), 1);
	ASSERT_EQ(get_envs_len(arge), len - 1);
	for (int i=0; i < len - 1; i++) {
		ASSERT_STREQ(expected[i], arge[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(unset_env, key_not_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	ASSERT_EQ(unset_env((char *)"NOT_EXIST", &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len);
	for (int i=0; i < len; i++) {
		ASSERT_STREQ(envs[i], arge[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(unset_env, empty_envs)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * 1);
	arge[0] = NULL;

	// test
	ASSERT_EQ(unset_env((char *)"LESS", &arge), 0);
	ASSERT_EQ(get_envs_len(arge), 0);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(unset_env, invalid)
{
	char		**arge;

	// prepare
	arge = (char **)malloc(sizeof(char *) * 1);
	arge[0] = NULL;

	// test
	const char	*args[] = {
		"FOO ",
		" FOO",
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
	};
	for (int i=0; i < (int)(sizeof(args) / sizeof(char *)); i++) {
		ASSERT_EQ(unset_env((char *)args[i], &arge), -1);
	}
	ASSERT_EQ(get_envs_len(arge), 0);

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, one_key_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {"LESS", NULL};
	const char	*expected[] = {
		"PAGER=less",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};

	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len - 1);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], expected[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, multi_key_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {"LSCOLORS", "LESS", NULL};
	const char	*expected[] = {
		"PAGER=less",
		"LC_CTYPE=UTF-8",
	};

	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len - 2);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], expected[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, key_not_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {"NOT_EXIST", NULL};

	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], envs[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, multi_key_not_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {"NOT_EXIST", "ALSO_NOT_EXIST", NULL};

	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], envs[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, key_exist_and_not_exist)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {"NOT_EXIST", "LESS", NULL};
	const char	*expected[] = {
		"PAGER=less",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
	};

	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	ASSERT_EQ(get_envs_len(arge), len - 1);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], expected[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, invalid)
{
	char		**arge;
	const char	*envs[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
		"LC_CTYPE=UTF-8",
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
	const char	*args[] = {
		"FOO ",
		" FOO",
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
		NULL,
	};

	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	::testing::internal::GetCapturedStderr();
	ASSERT_EQ(get_envs_len(arge), len);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], envs[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_unsetenv, existing_key_with_invalid)
{
	char		**arge;
	const char	*envs[] = {
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
	const char	*args[] = {" LESS", "LC_CTYPE", NULL};
	const char	*expected[] = {
		"PAGER=less",
		"LESS=-R",
		"LSCOLORS=Gxfxcxdxbxegedabagacad",
	};

	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_unsetenv((char **)args, &arge), 0);
	::testing::internal::GetCapturedStderr();
	ASSERT_EQ(get_envs_len(arge), len - 1);

	for (int i=0 ; arge[i]; i++) {
		ASSERT_STREQ(arge[i], expected[i]);
	}

	// free
	for (int i=0; arge[i] != NULL; i++) {
		free(arge[i]);
	}
	free(arge);
}
