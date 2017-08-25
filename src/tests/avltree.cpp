#include "../tdigest/avltree.hpp"

#include <gtest/gtest.h>

TEST(AvlTreeTest, GenericTest) {
    AvlTree* tree = new AvlTree();

    ASSERT_EQ(tree->root(), 0);
    ASSERT_EQ(tree->size(), 0);

    tree->add(5., 3);
    tree->add(8., 1);
    tree->add(9., 2);
    tree->add(4., 2);
    tree->add(7., 2);
    tree->add(6., 2);
    tree->add(2., 2);
    tree->add(1., 6);
    tree->add(3., 6);

    tree->add(5.1, 3);
    tree->add(8.5555, 1);
    tree->add(9.023, 2);
    tree->add(4.232, 2);
    tree->add(7.112, 2);
    tree->add(66., 2);
    tree->add(223.0, 2);
    tree->add(1.01, 6);
    tree->add(23.11, 6);

    ASSERT_EQ(tree->checkBalance(), true);
    ASSERT_EQ(tree->checkAggregates(), true);
    ASSERT_EQ(tree->checkIntegrity(), true);
}
