#include <gtest/gtest.h>
#include "minishell.h"

class ParseQuote: public ::testing::Test {
protected:
	char		*token;

	virtual void	SetUp() {
		token = strdup("");
	}

	virtual void	TearDown() {
		free(token);
	}
};

TEST_F(ParseQuote, no_space)
{
	ASSERT_EQ(parse_quote((char *)"'quote'", &token), 7);
	ASSERT_STREQ(token, "quote");
}

TEST_F(ParseQuote, with_space)
{
	const char	*input = "' quote  ss  i  i i  aaa  '";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, " quote  ss  i  i i  aaa  ");
}

TEST_F(ParseQuote, with_backslash)
{
	ASSERT_EQ(parse_quote((char *)"'\\a'", &token), 4);
	ASSERT_STREQ(token, "\\a");
}

TEST_F(ParseQuote, with_backslash_surrounded_by_space)
{
	const char	*input = "'   \\a    '";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, "   \\a    ");
}

TEST_F(ParseQuote, with_backslash_quote)
{
	ASSERT_EQ(parse_quote((char *)"'\\''", &token), 4);
	ASSERT_STREQ(token, "'");
}

TEST_F(ParseQuote, with_backslash_quote_surrounded_by_space)
{
	const char	*input = "'    \\'       '";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    '       ");
}

TEST_F(ParseQuote, with_multi_backslash_quote_case1)
{
	const char	*input = "'    \\'     \\'  '";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    '     '  ");
}

TEST_F(ParseQuote, with_multi_backslash_quote_case2)
{
	const char	*input = "'\\'     \\'  '";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, "'     '  ");
}

TEST_F(ParseQuote, with_multi_backslash_quote_case3)
{
	const char	*input = "'    \\'     \\''";

	ASSERT_EQ(parse_quote((char *)input, &token), (long)strlen(input));
	ASSERT_STREQ(token, "    '     '");
}
