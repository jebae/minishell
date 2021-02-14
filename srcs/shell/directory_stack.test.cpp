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

TEST_F(DirectoryStackTest, get_history_empty)
{
	char	*path;

	::testing::internal::CaptureStderr();
	ASSERT_EQ(get_dirstack_history((char *)"0", &stack, &path), -1);
	string output = ::testing::internal::GetCapturedStderr();

	::testing::internal::CaptureStderr();
	ASSERT_EQ(get_dirstack_history((char *)"-1", &stack, &path), -1);
	output = ::testing::internal::GetCapturedStderr();
}

TEST_F(DirectoryStackTest, update_history_already_exist)
{
	char		buf[27] = {0,};
	int			i;
	t_list_node	*node;

	setupList();

	ASSERT_EQ(update_dirstack_history((char *)"d", &stack), 0);

	node = stack.head;
	i = 0;
	while (node)
	{
		buf[i] = ((char *)node->data)[0];
		node = node->next;
		i++;
	}
	ASSERT_STREQ(buf, "dabcefghijklmnopqrstuvwxyz");
	ASSERT_STREQ((char *)stack.head->data, "d");
	ASSERT_STREQ((char *)stack.tail->data, "z");
}

TEST_F(DirectoryStackTest, update_history_already_exist_at_first)
{
	char		buf[27] = {0,};
	int			i;
	t_list_node	*node;

	setupList();

	ASSERT_EQ(update_dirstack_history((char *)"a", &stack), 0);

	node = stack.head;
	i = 0;
	while (node)
	{
		buf[i] = ((char *)node->data)[0];
		node = node->next;
		i++;
	}
	ASSERT_STREQ(buf, "abcdefghijklmnopqrstuvwxyz");
	ASSERT_STREQ((char *)stack.head->data, "a");
	ASSERT_STREQ((char *)stack.tail->data, "z");
}


TEST_F(DirectoryStackTest, update_history_already_exist_at_last)
{
	char		buf[27] = {0,};
	int			i;
	t_list_node	*node;

	setupList();

	ASSERT_EQ(update_dirstack_history((char *)"z", &stack), 0);

	node = stack.head;
	i = 0;
	while (node)
	{
		buf[i] = ((char *)node->data)[0];
		node = node->next;
		i++;
	}
	ASSERT_STREQ(buf, "zabcdefghijklmnopqrstuvwxy");
	ASSERT_STREQ((char *)stack.head->data, "z");
	ASSERT_STREQ((char *)stack.tail->data, "y");
}

TEST_F(DirectoryStackTest, update_history_to_empty)
{
	ASSERT_EQ((int)stack.length, 0);

	ASSERT_EQ(update_dirstack_history((char *)"!", &stack), 1);
	ASSERT_EQ((int)stack.length, 1);
	ASSERT_STREQ((char *)stack.head->data, "!");
	ASSERT_STREQ((char *)stack.tail->data, "!");
}


TEST_F(DirectoryStackTest, update_history_not_exist)
{
	char		buf[28] = {0,};
	int			i;
	int			prev_length;
	t_list_node	*node;

	setupList();

	prev_length = stack.length;

	ASSERT_EQ(update_dirstack_history((char *)"!", &stack), 1);
	ASSERT_EQ((int)stack.length, prev_length + 1);

	node = stack.head;
	i = 0;
	while (node)
	{
		buf[i] = ((char *)node->data)[0];
		node = node->next;
		i++;
	}
	ASSERT_STREQ(buf, "!abcdefghijklmnopqrstuvwxyz");
	ASSERT_STREQ((char *)stack.head->data, "!");
	ASSERT_STREQ((char *)stack.tail->data, "z");
}
