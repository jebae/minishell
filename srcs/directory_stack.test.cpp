#include <gtest/gtest.h>
#include "minishell.h"

using namespace std;

class DirectoryStackTest: public ::testing::Test {
protected:
	t_list	stack;

	virtual void	SetUp() {
		init_list(&stack);
	}

	void			setupList() {
		char	path[2] = {0,};

		for (int i=0; i < 26; i++) {
			path[0] = 'a' + i;
			push_list_node(strdup(path), &stack);
		}
	}

	virtual void	TearDown() {
		clear_list(&stack);
	}
};

TEST_F(DirectoryStackTest, get_history_no_symbol)
{
	char	*path;

	setupList();

	ASSERT_EQ(get_dirstack_history((char *)"0", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"a");

	ASSERT_EQ(get_dirstack_history((char *)"1", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"b");

	ASSERT_EQ(get_dirstack_history((char *)"24", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"y");

	ASSERT_EQ(get_dirstack_history((char *)"25", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"z");
}

TEST_F(DirectoryStackTest, get_history_plus)
{
	char	*path;

	setupList();

	ASSERT_EQ(get_dirstack_history((char *)"+0", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"a");

	ASSERT_EQ(get_dirstack_history((char *)"+1", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"b");

	ASSERT_EQ(get_dirstack_history((char *)"+24", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"y");

	ASSERT_EQ(get_dirstack_history((char *)"+25", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"z");
}

TEST_F(DirectoryStackTest, get_history_minus)
{
	char	*path;

	setupList();

	ASSERT_EQ(get_dirstack_history((char *)"-1", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"z");

	ASSERT_EQ(get_dirstack_history((char *)"-2", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"y");

	ASSERT_EQ(get_dirstack_history((char *)"-25", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"b");

	ASSERT_EQ(get_dirstack_history((char *)"-26", &stack, &path), 0);
	ASSERT_STREQ(path, (char *)"a");
}

TEST_F(DirectoryStackTest, get_history_out_of_range)
{
	char	*path;

	setupList();

	::testing::internal::CaptureStderr();
	ASSERT_EQ(get_dirstack_history((char *)"-27", &stack, &path), -1);
	string output = ::testing::internal::GetCapturedStderr();

	::testing::internal::CaptureStderr();
	ASSERT_EQ(get_dirstack_history((char *)"26", &stack, &path), -1);
	output = ::testing::internal::GetCapturedStderr();
}
