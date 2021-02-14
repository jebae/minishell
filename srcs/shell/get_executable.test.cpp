#include <gtest/gtest.h>
#include <fcntl.h>
#include "minishell.h"

using namespace std;

class GetExecutableTest: public ::testing::Test {
protected:
	char	**envs;

	virtual void	SetUp() {
		envs = (char **)malloc(sizeof(char *) * 2);
		envs[0] = (char *)"PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
		envs[1] = NULL;
	}

	virtual void	TearDown() {
		free(envs);
	}
};

TEST_F(GetExecutableTest, dot_executable)
{
	int			fd;
	const char	*arg = "./executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0755);

	char		*exe = get_executable((char *)arg, envs);

	ASSERT_STREQ(exe, arg);
	free(exe);
	remove(arg);
}

TEST_F(GetExecutableTest, dot_file_not_exist)
{
	const char	*arg = "./not_exist";

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "no such file or directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
}

TEST_F(GetExecutableTest, dot_directory)
{
	const char	*arg = "./directory";

	mkdir(arg, 0755);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "is a directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
	rmdir(arg);
}

TEST_F(GetExecutableTest, dot_not_executable)
{
	int			fd;
	const char	*arg = "./not_executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0666);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "permission denied"), NULL);
	ASSERT_EQ((long)exe, NULL);
	remove(arg);
}

TEST_F(GetExecutableTest, dotdot_executable)
{
	int			fd;
	const char	*arg = "../executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0755);

	char		*exe = get_executable((char *)arg, envs);

	ASSERT_STREQ(exe, arg);
	free(exe);
	remove(arg);
}

TEST_F(GetExecutableTest, dotdot_file_not_exist)
{
	const char	*arg = "../not_exist";

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "no such file or directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
}

TEST_F(GetExecutableTest, dotdot_directory)
{
	const char	*arg = "../directory";

	mkdir(arg, 0755);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "is a directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
	rmdir(arg);
}

TEST_F(GetExecutableTest, dotdot_not_executable)
{
	int			fd;
	const char	*arg = "../not_executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0666);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "permission denied"), NULL);
	ASSERT_EQ((long)exe, NULL);
	remove(arg);
}

TEST_F(GetExecutableTest, absolute_path_executable)
{
	int			fd;
	const char	*arg = "/tmp/executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0755);

	char		*exe = get_executable((char *)arg, envs);

	ASSERT_STREQ(exe, arg);
	free(exe);
	remove(arg);
}

TEST_F(GetExecutableTest, absolute_path_file_not_exist)
{
	const char	*arg = "/tmp/not_exist";

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "no such file or directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
}

TEST_F(GetExecutableTest, absolute_path_directory)
{
	const char	*arg = "/tmp/directory";

	mkdir(arg, 0755);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "is a directory"), NULL);
	ASSERT_EQ((long)exe, NULL);
	rmdir(arg);
}

TEST_F(GetExecutableTest, absolute_path_not_executable)
{
	int			fd;
	const char	*arg = "/tmp/not_executable";

	fd = open(arg, O_CREAT, 0);
	chmod(arg, 0666);

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "permission denied"), NULL);
	ASSERT_EQ((long)exe, NULL);
	remove(arg);
}

TEST_F(GetExecutableTest, exist_in_PATH)
{
	const char	*arg = "ls";
	char		*exe = get_executable((char *)arg, envs);

	ASSERT_NE((long)exe, NULL);
	free(exe);
}

TEST_F(GetExecutableTest, not_exist_in_PATH)
{
	const char	*arg = "asdfasdfasdf";

	::testing::internal::CaptureStderr();
	char		*exe = get_executable((char *)arg, envs);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(), "command not found"), NULL);
	ASSERT_EQ((long)exe, NULL);
}
