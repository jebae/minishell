#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

class ParseTilde: public ::testing::Test {
protected:
	char		*token;
	t_context	ctx;
	const char	*home = "/Users/me";

	virtual void	SetUp() {

		token = strdup("");

		memset(&ctx, 0, sizeof(t_context));

		ctx.envs = (char **)malloc(sizeof(char *) * 4);
		memset(ctx.envs, 0, sizeof(char *) * 4);

		char	path[2] = {0,};

		for (int i=0; i < 26; i++) {
			path[0] = 'a' + i;
			push_list_node(strdup(path), &ctx.dir_stack);
		}
	}

	virtual void	TearDown() {
		free(token);

		for (int i=0; ctx.envs[i]; i++) {
			free(ctx.envs[i]);
		}
		free(ctx.envs);

		if (ctx.pwd)
			free(ctx.pwd);

		clear_list(&ctx.dir_stack);
	}
};

TEST_F(ParseTilde, tilde_only_HOME_exist)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~", &ctx, &token), 1);
	ASSERT_STREQ(token, home);
}

TEST_F(ParseTilde, tilde_space_HOME_exist)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~ ", &ctx, &token), 1);
	ASSERT_STREQ(token, home);
}

TEST_F(ParseTilde, tilde_only_HOME_not_exist)
{
	ASSERT_EQ(parse_tilde((char *)"~", &ctx, &token), 1);
	ASSERT_STREQ(token, "");
}

TEST_F(ParseTilde, tilde_dummy_char_case1)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~a", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_dummy_char_case2)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~../", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_plus)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~+", &ctx, &token), 2);
	ASSERT_STREQ(token, "/usr/bin");
}

TEST_F(ParseTilde, tilde_plus_pwd_not_exist)
{
	ASSERT_EQ(parse_tilde((char *)"~+", &ctx, &token), 2);
	ASSERT_STREQ(token, "");
}

TEST_F(ParseTilde, tilde_plus_space)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~+ ", &ctx, &token), 2);
	ASSERT_STREQ(token, "/usr/bin");
}

TEST_F(ParseTilde, tilde_plus_dummy_char_case1)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~+a", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_plus_dummy_char_case2)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~+../", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_minus_OLDPWD_exist)
{
	ctx.envs[0] = strdup("OLDPWD=/etc/logs");

	ASSERT_EQ(parse_tilde((char *)"~-", &ctx, &token), 2);
	ASSERT_STREQ(token, "/etc/logs");
}

TEST_F(ParseTilde, tilde_minus_OLDPWD_exist_followed_by_space)
{
	ctx.envs[0] = strdup("OLDPWD=/etc/logs");

	ASSERT_EQ(parse_tilde((char *)"~- ", &ctx, &token), 2);
	ASSERT_STREQ(token, "/etc/logs");
}

TEST_F(ParseTilde, tilde_minus_OLDPWD_not_exist)
{
	ASSERT_EQ(parse_tilde((char *)"~-", &ctx, &token), 2);
	ASSERT_STREQ(token, "");
}

TEST_F(ParseTilde, tilde_minus_dummy_char_case1)
{
	ASSERT_EQ(parse_tilde((char *)"~-a", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_minus_dummy_char_case2)
{
	ASSERT_EQ(parse_tilde((char *)"~-../", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_num)
{
	ASSERT_EQ(parse_tilde((char *)"~17", &ctx, &token), 3);
	ASSERT_STREQ(token, "r");
}

TEST_F(ParseTilde, tilde_plus_num)
{
	ASSERT_EQ(parse_tilde((char *)"~+17", &ctx, &token), 4);
	ASSERT_STREQ(token, "r");
}

TEST_F(ParseTilde, tilde_plus_num_dummy_char)
{
	ASSERT_EQ(parse_tilde((char *)"~+17../", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_minus_num)
{
	ASSERT_EQ(parse_tilde((char *)"~-17", &ctx, &token), 4);
	ASSERT_STREQ(token, "j");
}

TEST_F(ParseTilde, tilde_minus_num_dummy_char)
{
	ASSERT_EQ(parse_tilde((char *)"~-17../", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_num_out_of_range)
{
	::testing::internal::CaptureStderr();
	ASSERT_EQ(parse_tilde((char *)"~100", &ctx, &token), -1);
	string output = ::testing::internal::GetCapturedStderr();
}

TEST_F(ParseTilde, tilde_slash)
{
	const char	*input = "~/../sub";

	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/Users/me/../sub");
}

TEST_F(ParseTilde, tilde_slash_HOME_not_exist)
{
	const char	*input = "~/../sub";

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/../sub");
}

TEST_F(ParseTilde, tilde_plus_slash)
{
	const char	*input = "~+/../sub";

	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/usr/bin/../sub");
}

TEST_F(ParseTilde, tilde_plus_slash_pwd_not_exist)
{
	const char	*input = "~+/../sub";

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/../sub");
}

TEST_F(ParseTilde, tilde_minus_slash)
{
	const char	*input = "~-/../sub";

	ctx.envs[0] = strdup("OLDPWD=/before/sunrise");

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/before/sunrise/../sub");
}

TEST_F(ParseTilde, tilde_minus_slash_OLDPWD_not_exist)
{
	const char	*input = "~-/../sub";

	ASSERT_EQ(parse_tilde((char *)input, &ctx, &token), (long)strlen(input));
	ASSERT_STREQ(token, "/../sub");
}

TEST_F(ParseTilde, tilde_plus_num_slash)
{
	ASSERT_EQ(parse_tilde((char *)"~+17/go", &ctx, &token), 7);
	ASSERT_STREQ(token, "r/go");
}

TEST_F(ParseTilde, tilde_minus_num_slash)
{
	ASSERT_EQ(parse_tilde((char *)"~-17/go", &ctx, &token), 7);
	ASSERT_STREQ(token, "j/go");
}

TEST_F(ParseTilde, tilde_empty_quote)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~''", &ctx, &token), 3);
	ASSERT_STREQ(token, home);
}

TEST_F(ParseTilde, tilde_quote_plus)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~'+'", &ctx, &token), 4);
	ASSERT_STREQ(token, "/usr/bin");
}

TEST_F(ParseTilde, tilde_quote_minus)
{
	ctx.envs[0] = strdup("OLDPWD=/before/sunrise");

	ASSERT_EQ(parse_tilde((char *)"~'-'", &ctx, &token), 4);
	ASSERT_STREQ(token, "/before/sunrise");
}

TEST_F(ParseTilde, tilde_quote_dummy_char)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~'..'", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_quote_plus_dummy_char)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~'+..'", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_quote_minus_dummy_char)
{
	ctx.envs[0] = strdup("OLDPWD=/before/sunrise");

	ASSERT_EQ(parse_tilde((char *)"~'-..'", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_empty_dblquote)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~\"\"", &ctx, &token), 3);
	ASSERT_STREQ(token, home);
}

TEST_F(ParseTilde, tilde_dblquote_plus)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~\"+\"", &ctx, &token), 4);
	ASSERT_STREQ(token, "/usr/bin");
}

TEST_F(ParseTilde, tilde_dblquote_minus)
{
	ctx.envs[0] = strdup("OLDPWD=/before/sunrise");

	ASSERT_EQ(parse_tilde((char *)"~\"-\"", &ctx, &token), 4);
	ASSERT_STREQ(token, "/before/sunrise");
}

TEST_F(ParseTilde, tilde_dblquote_dummy_char)
{
	ctx.envs[0] = strdup("HOME=/Users/me");

	ASSERT_EQ(parse_tilde((char *)"~\"..\"", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_dblquote_plus_dummy_char)
{
	ctx.pwd = strdup("/usr/bin");

	ASSERT_EQ(parse_tilde((char *)"~\"+..\"", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, tilde_dblquote_minus_dummy_char)
{
	ctx.envs[0] = strdup("OLDPWD=/before/sunrise");

	ASSERT_EQ(parse_tilde((char *)"~\"-..\"", &ctx, &token), 1);
	ASSERT_STREQ(token, "~");
}

TEST_F(ParseTilde, multi_tilde)
{
	ASSERT_EQ(parse_tilde((char *)"~~~", &ctx, &token), 3);
	ASSERT_STREQ(token, "~~~");
}
