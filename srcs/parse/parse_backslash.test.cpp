#include <gtest/gtest.h>
#include "minishell.h"

class ParseBackslash: public ::testing::Test {
protected:
	char	*token;

	virtual void	SetUp() {
		token = strdup("");
	}

	virtual void	TearDown() {
		free(token);
	}
};

TEST_F(ParseBackslash, backslash_with_alnum)
{
	ASSERT_EQ(parse_backslash((char *)"\\sdd", &token), 2);
	ASSERT_STREQ(token, "s");

	ASSERT_EQ(parse_backslash((char *)"\\3", &token), 2);
	ASSERT_STREQ(token, "s3");
}

TEST_F(ParseBackslash, backslash_with_specialchar)
{
	ASSERT_EQ(parse_backslash((char *)"\\:", &token), 2);
	ASSERT_STREQ(token, ":");
}

TEST_F(ParseBackslash, backslash_with_backslash)
{
	ASSERT_EQ(parse_backslash((char *)"\\\\", &token), 2);
	ASSERT_STREQ(token, "\\");
}

TEST_F(ParseBackslash, backslash_with_space)
{
	ASSERT_EQ(parse_backslash((char *)"\\ ", &token), 2);
	ASSERT_STREQ(token, " ");
}

TEST_F(ParseBackslash, backslash_only)
{
	ASSERT_EQ(parse_backslash((char *)"\\", &token), 1);
	ASSERT_STREQ(token, "\\");
}
