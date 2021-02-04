#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

TEST(cmd_echo, one_argument)
{
	const char	*args[] = {"foo", NULL};
	char		expected[1024] = {0,};

	for (int i=0; i < (int)(sizeof(args) / sizeof(char *)) - 1; i++) {
		strcat(expected, args[i]);
	}
	expected[strlen(expected)] = '\n';

	::testing::internal::CaptureStdout();
	cmd_echo((char **)args);
	std::string output = ::testing::internal::GetCapturedStdout();

	ASSERT_STREQ(output.c_str(), expected);
}

TEST(cmd_echo, multiple_argument)
{
	const char	*args[] = {"long long", "time   ", "   a  ", "go", NULL};
	char		expected[1024] = {0,};

	for (int i=0; i < (int)(sizeof(args) / sizeof(char *)) - 1; i++) {
		strcat(expected, args[i]);
		strcat(expected, " ");
	}
	expected[strlen(expected) - 1] = '\n';

	::testing::internal::CaptureStdout();
	cmd_echo((char **)args);
	std::string output = ::testing::internal::GetCapturedStdout();

	ASSERT_STREQ(output.c_str(), expected);
}

TEST(cmd_echo, no_argument)
{
	const char	*args[] = {NULL};

	::testing::internal::CaptureStdout();
	cmd_echo((char **)args);
	std::string output = ::testing::internal::GetCapturedStdout();

	ASSERT_STREQ(output.c_str(), "\n");
}
