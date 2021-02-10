#include <gtest/gtest.h>
#include "minishell.h"

TEST(concat, token_input_not_empty)
{
	char	*token = strdup("hello");

	concat(&token, (char *)"world", 3);

	ASSERT_STREQ(token, "hellowor");
	free(token);
}

TEST(concat, token_followed_by_space)
{
	char	*token = strdup("hello ");

	concat(&token, (char *)"world", 3);

	ASSERT_STREQ(token, "hello wor");
	free(token);
}

TEST(concat, token_empty)
{
	char	*token = strdup("");

	concat(&token, (char *)"world", 2);

	ASSERT_STREQ(token, "wo");
	free(token);
}

TEST(concat, input_empty)
{
	char	*token = strdup("hello world");

	concat(&token, (char *)"", 0);

	ASSERT_STREQ(token, "hello world");
	free(token);
}

TEST(concat, token_input_empty)
{
	char	*token = strdup("");

	concat(&token, (char *)"", 0);

	ASSERT_STREQ(token, "");
	free(token);
}

TEST(concat, len_equal_input_length)
{
	char	*token = strdup("hello world");

	concat(&token, (char *)"sony", 4);

	ASSERT_STREQ(token, "hello worldsony");
	free(token);
}

TEST(concat, len_0)
{
	char	*token = strdup("hello world");

	concat(&token, (char *)"sony", 0);

	ASSERT_STREQ(token, "hello world");
	free(token);
}
