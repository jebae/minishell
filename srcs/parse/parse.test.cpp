#include <gtest/gtest.h>
#include "minishell.h"

class ParseTest: public ::testing::Test {
protected:
	t_context	ctx;

	virtual void	SetUp() {
		memset(&ctx, 0, sizeof(t_context));
	}

	virtual void	TearDown() {
		if (ctx.envs) {
			for (int i=0; ctx.envs[i]; i++) {
				free(ctx.envs[i]);
			}
			free(ctx.envs);
		}
		if (ctx.pwd)
			free(ctx.pwd);
		if (ctx.dir_stack)
			free(ctx.dir_stack);
	}

	void			test(const char *input, char **expected) {
		char	**args = parse((char *)input, &ctx);
		int		i, len;

		len = 0;
		while (expected[len])
			len++;

		i = 0;
		for (i=0; args[i]; i++) {
			ASSERT_STREQ(args[i], expected[i]);
		}

		ASSERT_EQ(i, len);

		for (i=0; args[i]; i++) {
			free(args[i]);
		}
		free(args);
	}
};

TEST_F(ParseTest, command_only)
{
	const char	*input = "cd";
	const char	*expected[] = {
		"cd",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, one_argument)
{
	const char	*input = "echo hello";
	const char	*expected[] = {
		"echo",
		"hello",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, multi_argument)
{
	const char	*input = "echo hello _1_ world bro";
	const char	*expected[] = {
		"echo",
		"hello",
		"_1_",
		"world",
		"bro",
		NULL
	};

	test(input, (char **)expected);
}
TEST_F(ParseTest, quote)
{
	const char	*input = "'quote   foo  ba  r  '";
	const char	*expected[] = {
		"quote   foo  ba  r  ",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, quote_with_escape)
{
	const char	*input = "'quote  \\nss foo  ba  r  '";
	const char	*expected[] = {
		"quote  \\nss foo  ba  r  ",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, quote_with_escaped_quote)
{
	const char	*input = "'  qu\\'o  t  e'";
	const char	*expected[] = {
		"  qu'o  t  e",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, double_quote)
{
	const char	*input = "\"   double quote  \"";
	const char	*expected[] = {
		"   double quote  ",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, double_quote_with_escape)
{
	const char	*input = "\"   double  \\sgty quote  \"";
	const char	*expected[] = {
		"   double  \\sgty quote  ",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, double_quote_with_escaped_double_quote)
{
	const char	*input = "\"   double  \\\"sgty quote  \"";
	const char	*expected[] = {
		"   double  \"sgty quote  ",
		NULL
	};

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_exist)
{
	const char	*input = "$HOME";
	const char	*expected[] = {
		"/Users/buzz",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, multi_env_exist)
{
	const char	*input = "$HOME    $FOO";
	const char	*expected[] = {
		"/Users/buzz",
		"bar",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 3);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = strdup("FOO=bar");
	ctx.envs[2] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_exist_not_only_env)
{
	const char	*input = "foo  $HOME    bar";
	const char	*expected[] = {
		"foo",
		"/Users/buzz",
		"bar",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_exist_surrounded)
{
	const char	*input = "foo  go$HOME.io    bar";
	const char	*expected[] = {
		"foo",
		"go/Users/buzz.io",
		"bar",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_not_exist)
{
	const char	*input = "$HOME";
	const char	*expected[] = {
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME_=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, multi_env_not_exist)
{
	const char	*input = "$HOME     $FOO    ";
	const char	*expected[] = {
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME_=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_not_exist_whitespace_surrounded)
{
	const char	*input = "   $HOME    ";
	const char	*expected[] = {
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME_=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_not_exist_not_only_env)
{
	const char	*input = "foo  $HOME    bar";
	const char	*expected[] = {
		"foo",
		"bar",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME_=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_not_exist_surrounded)
{
	const char	*input = "foo  go$HOME.io    bar";
	const char	*expected[] = {
		"foo",
		"go.io",
		"bar",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME_=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_exist_and_not_exist)
{
	const char	*input = " $FOFO $HOME     $FOO    ";
	const char	*expected[] = {
		"/Users/buzz",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_in_quote)
{
	const char	*input = "'$HOME'";
	const char	*expected[] = {
		"$HOME",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, env_in_double_quote)
{
	const char	*input = "\"$HOME\"";
	const char	*expected[] = {
		"/Users/buzz",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, only_$)
{
	const char	*input = "echo $";
	const char	*expected[] = {
		"echo",
		"$",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, wrong_env_key_format_after_$)
{
	const char	*input = "echo $:";
	const char	*expected[] = {
		"echo",
		"$:",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

TEST_F(ParseTest, wrong_env_key_format_after_$_following_str)
{
	const char	*input = "echo fa$:";
	const char	*expected[] = {
		"echo",
		"fa$:",
		NULL
	};

	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("HOME=/Users/buzz");
	ctx.envs[1] = NULL;

	test(input, (char **)expected);
}

// TEST_F(ParseTest, tilde_only)
// {
// 	const char	*input = "cd ~";
// 	const char	*expected[] = {
// 		"cd",
// 		"/Users/buzz",
// 		NULL
// 	};

// 	ctx.envs = (char **)malloc(sizeof(char *) * 2);
// 	ctx.envs[0] = strdup("HOME=/Users/buzz");
// 	ctx.envs[1] = NULL;

// 	test(input, (char **)expected);
// }

// TEST_F(ParseTest, tilde_follwed_by_plus)
// {
// 	const char	*input = "cd ~+";
// 	const char	*expected[] = {
// 		"cd",
// 		"/some/where",
// 		NULL
// 	};

// 	ctx.pwd = strdup("/some/where");

// 	test(input, (char **)expected);
// }

// TEST_F(ParseTest, tilde_follwed_by_plus_number)
// TEST_F(ParseTest, tilde_follwed_by_minus)
// TEST_F(ParseTest, tilde_follwed_by_minus_number)
// TEST_F(ParseTest, tilde_followed_by_char)
// TEST_F(ParseTest, tilde_following_char)
// TEST_F(ParseTest, whitespace_around)
