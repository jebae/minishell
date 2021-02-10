#include <gtest/gtest.h>
#include "minishell.h"

class ParseDollar: public ::testing::Test {
protected:
	char		*token;
	char		**envs;
	const char	*env = "/Users/me";

	virtual void	SetUp() {
		token = strdup("");
		envs = (char **)malloc(sizeof(char *) * 2);
		envs[0] = strdup("HOME=/Users/me");
		envs[1] = NULL;
	}

	virtual void	TearDown() {
		free(token);
		for (int i=0; envs[i]; i++) {
			free(envs[i]);
		}
		free(envs);
	}
};

TEST_F(ParseDollar, env_exist)
{
	ASSERT_EQ(parse_dollar((char *)"$HOME", envs, &token), 5);
	ASSERT_STREQ(token, env);
}

TEST_F(ParseDollar, env_exist_followed_by_space)
{
	ASSERT_EQ(parse_dollar((char *)"$HOME ", envs, &token), 5);
	ASSERT_STREQ(token, env);
}

TEST_F(ParseDollar, env_not_exist)
{
	ASSERT_EQ(parse_dollar((char *)"$HO:ME ", envs, &token), 3);
	ASSERT_STREQ(token, "");
}

TEST_F(ParseDollar, only_$)
{
	ASSERT_EQ(parse_dollar((char *)"$", envs, &token), 1);
	ASSERT_STREQ(token, "$");
}

TEST_F(ParseDollar, only_$_followed_by_space)
{
	ASSERT_EQ(parse_dollar((char *)"$ ", envs, &token), 1);
	ASSERT_STREQ(token, "$");
}

TEST_F(ParseDollar, wrong_env_key_format_after_$)
{
	ASSERT_EQ(parse_dollar((char *)"$.", envs, &token), 1);
	ASSERT_STREQ(token, "$");
}
