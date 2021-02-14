#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

TEST(env, one_env)
{
	const char	*arge[] = {"FOO=bar", NULL};

	::testing::internal::CaptureStdout();
	cmd_env((char **)arge);
	std::string output = ::testing::internal::GetCapturedStdout();

	ASSERT_STREQ(output.c_str(), "FOO=bar\n");
}

TEST(env, multi_env)
{
	const char	*arge[] = {"FOO=bar", "TOYSTORY=1", NULL};
	char		expected[256] = {0,};

	for (int i=0; i < (int)(sizeof(arge) / sizeof(char *)) - 1; i++) {
		strcat(expected, arge[i]);
		strcat(expected, "\n");
	}

	::testing::internal::CaptureStdout();
	cmd_env((char **)arge);
	std::string output = ::testing::internal::GetCapturedStdout();

	ASSERT_STREQ(output.c_str(), expected);
}

TEST(get_env_idx, exist)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ(get_env_idx((char *)"TOYSTORY", (char **)arge), 1);
}

TEST(get_env_idx, env_without_value)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ(get_env_idx((char *)"TOYSTORY", (char **)arge), 1);
}

TEST(get_env_idx, not_exist_case1)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ(get_env_idx((char *)"NOT_EXIST", (char **)arge), -1);
}

TEST(get_env_idx, not_exist_case2)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ(get_env_idx((char *)"TOYSTORY2", (char **)arge), -1);
}

TEST(get_env_idx, not_exist_case3)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ(get_env_idx((char *)"TOYSTOR", (char **)arge), -1);
}

TEST(get_env, exist)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_STREQ(get_env("TOYSTORY", (char **)arge), "awesome");
}

TEST(get_env, env_without_value)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_STREQ(get_env("TOYSTORY", (char **)arge), "");
}

TEST(get_env, not_exist_case1)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ((long)get_env("NOT_EXIST", (char **)arge), NULL);
}

TEST(get_env, not_exist_case2)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ((long)get_env("TOYSTORY2", (char **)arge), NULL);
}

TEST(get_env, not_exist_case3)
{
	const char	*arge[] = {
		"FOO=bar",
		"TOYSTORY=awesome",
		"INSIDE_OUT=1",
		NULL,
	};

	ASSERT_EQ((long)get_env("TOYSTOR", (char **)arge), NULL);
}

TEST(is_valid_env_key, valid)
{
	const char	*keys[] = {
		"FOO",
		"_FOO",
		"_100",
		"_",
		"FOO_BAR",
		"foo",
	};

	for (int i=0; i < (int)(sizeof(keys) / sizeof(char *)); i++) {
		ASSERT_TRUE(is_valid_env_key((char *)keys[i]));
	}
}

TEST(is_valid_env_key, invalid)
{
	const char	*keys[] = {
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
		"100",
		"1FOO",
	};

	for (int i=0; i < (int)(sizeof(keys) / sizeof(char *)); i++) {
		ASSERT_FALSE(is_valid_env_key((char *)keys[i]));
	}
}
