#include <gtest/gtest.h>
#include <fcntl.h>
#include "minishell.h"

using namespace std;

extern char **environ;

class CDTest: public ::testing::Test {
protected:
	t_context	ctx;
	char		pwd[1024];
	char		oldpwd[1024];
	char		buf[1024];

	virtual void	SetUp() {
		ctx.envs = (char **)malloc(sizeof(char *));
		ctx.envs[0] = NULL;

		ctx.pwd = getcwd(NULL, 0);
		strcpy(oldpwd, ctx.pwd);
	}

	virtual void	TearDown() {
		chdir(oldpwd);
		for (int i=0; ctx.envs[i]; i++) {
			free(ctx.envs[i]);
		}
		free(ctx.envs);
		free(ctx.pwd);
	}

	void			test_with_realpath(char *args[]) {
		realpath(args[0], pwd);

		ASSERT_EQ(cmd_cd((char **)args, &ctx), 0);
		getcwd(buf, 1024);
		ASSERT_STREQ(buf, pwd);
		ASSERT_STREQ(ctx.pwd, pwd);
		ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), pwd);
		ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), oldpwd);
	}

	void			test_error(char *args[], char *msg)
	{
		char	*temp_oldpwd = get_env((char *)"OLDPWD", ctx.envs);
		char	*temp_pwd = get_env((char *)"PWD", ctx.envs);
		char	*temp_ctxpwd = strdup(ctx.pwd);

		::testing::internal::CaptureStderr();
		ASSERT_EQ(cmd_cd((char **)args, &ctx), -1);
		string output = ::testing::internal::GetCapturedStderr();

		ASSERT_NE((long)strstr(output.c_str(), msg), NULL);
		ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), temp_pwd);
		ASSERT_STREQ(ctx.pwd, temp_ctxpwd);
		ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), temp_oldpwd);
		free(temp_ctxpwd);
	}
};

TEST_F(CDTest, relative_path_dotdot)
{
	const char	*args[] = {"..", NULL};

	test_with_realpath((char **)args);
}

TEST_F(CDTest, relative_path_dot)
{
	const char	*args[] = {".", NULL};

	test_with_realpath((char **)args);
}

TEST_F(CDTest, absolute_path)
{
	const char	*args[] = {"/usr", NULL};

	test_with_realpath((char **)args);
}

TEST_F(CDTest, no_argument_with_HOME)
{
	const char	*args[] = {NULL};
	char		*home;

	free(ctx.envs);
	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = ft_strjoin("HOME=", get_env((char *)"HOME", environ));
	ctx.envs[1] = NULL;
	home = get_env((char *)"HOME", ctx.envs);

	ASSERT_EQ(cmd_cd((char **)args, &ctx), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, home);
	ASSERT_STREQ(ctx.pwd, home);
	ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), home);
	ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), oldpwd);
}

TEST_F(CDTest, no_argument_without_HOME)
{
	const char	*args[] = {NULL};

	strcpy(pwd, ctx.pwd);

	ASSERT_EQ(cmd_cd((char **)args, &ctx), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(ctx.pwd, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), pwd);
}

TEST_F(CDTest, dash_with_OLDPWD)
{
	const char	*args[] = {"-", NULL};

	free(ctx.envs);
	ctx.envs = (char **)malloc(sizeof(char *) * 2);
	ctx.envs[0] = strdup("OLDPWD=/usr/bin");
	ctx.envs[1] = NULL;
	strcpy(pwd, get_env((char *)"OLDPWD", ctx.envs));

	ASSERT_EQ(cmd_cd((char **)args, &ctx), 0);
	getcwd(buf, 1024);
	ASSERT_STREQ(buf, pwd);
	ASSERT_STREQ(ctx.pwd, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), oldpwd);
}

TEST_F(CDTest, dash_without_OLDPWD)
{
	const char	*args[] = {"-", NULL};

	test_error((char **)args, (char *)"OLDPWD not set");
}

TEST_F(CDTest, not_exist)
{
	const char	*args[] = {"./not_exist", NULL};

	test_error((char **)args, (char *)"no such file or directory");
}

TEST_F(CDTest, not_directory)
{
	const char	*args[] = {"./not_dir", NULL};
	int			fd;

	fd = open(args[0], O_CREAT, 0);
	close(fd);
	chmod(args[0], 0755);

	test_error((char **)args, (char *)"not a directory");
	remove(args[0]);
}

TEST_F(CDTest, permission_denied)
{
	const char	*args[] = {"./not_permitted", NULL};

	ASSERT_NE(mkdir(args[0], 0011), -1);

	test_error((char **)args, (char *)"permission denied");
	remove(args[0]);
}

TEST_F(CDTest, multiple_argument)
{
	const char	*args[] = {"..", "..", NULL};

	test_error((char **)args, (char *)"too many arguments");
}

/**
 * /a
 * /b/c -> a (symlink)
 */
TEST_F(CDTest, symlink_dir)
{
	const char	*args[] = {"./b/c", NULL};

	ASSERT_NE(mkdir("./a", 0755), -1);
	ASSERT_NE(mkdir("./b", 0755), -1);
	symlink("../a", "./b/c");
	strcpy(pwd, oldpwd);
	strcat(pwd, "/b/c");

	ASSERT_EQ(cmd_cd((char **)args, &ctx), 0);
	chdir(oldpwd);

	ASSERT_STREQ(ctx.pwd, pwd);
	ASSERT_STREQ(get_env((char *)"PWD", ctx.envs), pwd);
	ASSERT_STREQ(get_env((char *)"OLDPWD", ctx.envs), oldpwd);
	ASSERT_NE(rmdir("./a"), -1);
	ASSERT_NE(unlink("./b/c"), -1);
	ASSERT_NE(rmdir("./b"), -1);
}

TEST_F(CDTest, symlink_file)
{
	const char	*args[] = {"./b/c", NULL};
	int			fd;

	fd = open("./a", O_CREAT, 0);
	close(fd);
	ASSERT_NE(mkdir("./b", 0755), -1);
	symlink("../a", "./b/c");

	test_error((char **)args, (char *)"not a directory");
	chdir(oldpwd);
	ASSERT_NE(remove("./a"), -1);
	ASSERT_NE(unlink("./b/c"), -1);
	ASSERT_NE(rmdir("./b"), -1);
}
