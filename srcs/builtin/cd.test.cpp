#include <gtest/gtest.h>
#include <fcntl.h>
#include "minishell.h"

using namespace std;

extern char **environ;

TEST(cmd_cd, relative_path_dotdot)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"..", NULL};
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	realpath(args[0], pwd);
	arge[0] = NULL;

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, relative_path_dot)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {".", NULL};
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	realpath(args[0], pwd);
	arge[0] = NULL;

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, absolute_path)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"/usr", NULL};
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	realpath(args[0], pwd);
	arge[0] = NULL;

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, home)
{
	char		**arge = (char **)malloc(sizeof(char *) * 2);
	const char	*args[] = {"~", NULL};
	char		*pwd;
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	pwd = get_env((char *)"HOME", environ);
	arge[1] = NULL;
	arge[0] = (char *)malloc(strlen("HOME=") + strlen(pwd) + 1);
	strcpy(arge[0], "HOME=");
	strcat(arge[0], pwd);

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, dash)
{
	char		**arge = (char **)malloc(sizeof(char *) * 2);
	const char	*args[] = {"-", NULL};
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	realpath("../..", pwd);
	arge[1] = NULL;
	arge[0] = (char *)malloc(strlen("OLDPWD=") + strlen(pwd) + 1);
	strcpy(arge[0], "OLDPWD=");
	strcat(arge[0], pwd);

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, PWD_OLDPWD_exist)
{
	char		**arge = (char **)malloc(sizeof(char *) * 3);
	const char	*args[] = {"..", NULL};
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	realpath(args[0], pwd);
	arge[0] = strdup("OLDPWD=/foo");
	arge[1] = strdup("PWD=/bar");
	arge[2] = NULL;

	ASSERT_EQ(cmd_cd((char **)args, &arge), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", arge), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", arge), oldpwd);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, not_exist)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"./not_exist", NULL};
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	arge[0] = NULL;


	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_cd((char **)args, &arge), -1);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(),
		(char *)"no such file or director"), NULL);

	getcwd(buf, 1024);
	ASSERT_STREQ(buf, oldpwd);
	ASSERT_EQ((long)get_env((char *)"PWD", arge), NULL);
	ASSERT_EQ((long)get_env((char *)"OLDPWD", arge), NULL);
	chdir(oldpwd);

	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
}

TEST(cmd_cd, not_directory)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"./not_dir", NULL};
	char		oldpwd[1024];
	char		buf[1024];
	int			fd;

	// prepare
	fd = open(args[0], O_CREAT, 0);
	close(fd);

	chmod(args[0], 0755);

	getcwd(oldpwd, 1024);
	arge[0] = NULL;

	// test
	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_cd((char **)args, &arge), -1);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(),
		(char *)"not a directory"), NULL);

	getcwd(buf, 1024);
	ASSERT_STREQ(buf, oldpwd);
	ASSERT_EQ((long)get_env((char *)"PWD", arge), NULL);
	ASSERT_EQ((long)get_env((char *)"OLDPWD", arge), NULL);
	chdir(oldpwd);

	// free
	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
	remove(args[0]);
}

TEST(cmd_cd, permission_denied)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"./not_permitted", NULL};
	char		oldpwd[1024];
	char		buf[1024];

	// prepare
	ASSERT_NE(mkdir(args[0], 0011), -1);

	getcwd(oldpwd, 1024);
	arge[0] = NULL;

	// test
	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_cd((char **)args, &arge), -1);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(),
		(char *)"permission denied"), NULL);

	getcwd(buf, 1024);
	ASSERT_STREQ(buf, oldpwd);
	ASSERT_EQ((long)get_env((char *)"PWD", arge), NULL);
	ASSERT_EQ((long)get_env((char *)"OLDPWD", arge), NULL);
	chdir(oldpwd);

	// free
	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
	remove(args[0]);
}

TEST(cmd_cd, multiple_argument)
{
	char		**arge = (char **)malloc(sizeof(char *));
	const char	*args[] = {"..", ".", NULL};
	char		oldpwd[1024];
	char		buf[1024];

	getcwd(oldpwd, 1024);
	arge[0] = NULL;

	// test
	::testing::internal::CaptureStderr();
	ASSERT_EQ(cmd_cd((char **)args, &arge), -1);
	string output = ::testing::internal::GetCapturedStderr();

	ASSERT_NE((long)strstr(output.c_str(),
		(char *)"too many arguments"), NULL);

	getcwd(buf, 1024);
	ASSERT_STREQ(buf, oldpwd);
	ASSERT_EQ((long)get_env((char *)"PWD", arge), NULL);
	ASSERT_EQ((long)get_env((char *)"OLDPWD", arge), NULL);
	chdir(oldpwd);

	// free
	for (int i=0; arge[i]; i++) {
		free(arge[i]);
	}
	free(arge);
	remove(args[0]);
}
