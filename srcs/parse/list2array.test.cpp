#include <gtest/gtest.h>
#include "minishell.h"

class List2ArrayTest: public ::testing::Test {
protected:
	t_list	list;
	char	**arr;

	virtual void	SetUp() {
		init_list(&list);
		arr = NULL;
	}

	virtual void	TearDown() {
		clear_list(&list);

		if (arr) {
			for (int i=0; arr[i]; i++) {
				free(arr[i]);
			}
			free(arr);
		}
	}
};

TEST_F(List2ArrayTest, empty)
{
	arr = list2array(&list);

	ASSERT_NE((long)arr, NULL);
	ASSERT_EQ((long)arr[0], NULL);
}

TEST_F(List2ArrayTest, one_exist)
{
	char	*data = strdup("hello");

	push_list_node(data, &list);

	arr = list2array(&list);

	ASSERT_NE((long)arr, NULL);
	ASSERT_STREQ(arr[0], data);
	ASSERT_NE((long)arr[0], NULL);
}

TEST_F(List2ArrayTest, multi_exist)
{
	const char	*data[] = {
		"aa","bb","ccc","adddd","ee",
	};
	int			len;

	for (int i=0; i < 5; i++) {
		push_list_node(strdup(data[i]), &list);
	}

	arr = list2array(&list);

	ASSERT_NE((long)arr, NULL);

	len = 0;
	while (arr[len])
		len++;
	ASSERT_EQ(len, 5);

	for (int i=0; i < 5; i++) {
		ASSERT_STREQ(arr[i], data[i]);
	}
}
