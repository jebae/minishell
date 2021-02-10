#include <gtest/gtest.h>
#include "minishell.h"

class ParseDblquote: public ::testing::Test {
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

TEST_F(ParseDblquote, no_space)
{
	ASSERT_EQ(parse_dblquote((char *)"\"quote\"", envs, &token), 7);
	ASSERT_STREQ(token, "quote");
}

TEST_F(ParseDblquote, with_space)
{
	const char	*input = "\" quote  ss  i  i i  aaa  \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, " quote  ss  i  i i  aaa  ");
}

TEST_F(ParseDblquote, with_backslash)
{
	ASSERT_EQ(parse_dblquote((char *)"\"\\a\"", envs, &token), 4);
	ASSERT_STREQ(token, "\\a");
}

TEST_F(ParseDblquote, with_backslash_surrounded_by_space)
{
	const char	*input = "\"   \\a    \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "   \\a    ");
}

TEST_F(ParseDblquote, with_backslash_dblquote)
{
	ASSERT_EQ(parse_dblquote((char *)"\"\\\"\"", envs, &token), 4);
	ASSERT_STREQ(token, "\"");
}

TEST_F(ParseDblquote, with_backslash_dblquote_surrounded_by_space)
{
	const char	*input = "\"    \\\"       \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    \"       ");
}

TEST_F(ParseDblquote, with_multi_backslash_dblquote_case1)
{
	const char	*input = "\"    \\\"     \\\"  \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    \"     \"  ");
}

TEST_F(ParseDblquote, with_multi_backslash_dblquote_case2)
{
	const char	*input = "\"\\\"     \\\"  \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "\"     \"  ");
}

TEST_F(ParseDblquote, with_multi_backslash_dblquote_case3)
{
	const char	*input = "\"    \\\"     \\\"\"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    \"     \"");
}

TEST_F(ParseDblquote, env_exist)
{
	ASSERT_EQ(parse_dblquote((char *)"\"$HOME\"", envs, &token), 7);
	ASSERT_STREQ(token, env);
}

TEST_F(ParseDblquote, env_not_exist)
{
	ASSERT_EQ(parse_dblquote((char *)"\"$HO:ME\"", envs, &token), 8);
	ASSERT_STREQ(token, ":ME");
}

TEST_F(ParseDblquote, env_exist_surrounded_by_space)
{
	const char	*input = "\"     $HOME \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "     /Users/me ");
}

TEST_F(ParseDblquote, env_not_exist_surrounded_by_space)
{
	const char	*input = "\"     $HO:ME \"";

	ASSERT_EQ(parse_dblquote((char *)input, envs, &token), (long)strlen(input));
	ASSERT_STREQ(token, "     :ME ");
}
