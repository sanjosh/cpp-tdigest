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

    ASSERT_EQ(tree->checkBalance(), true);
    ASSERT_EQ(tree->checkAggregates(), true);
    ASSERT_EQ(tree->checkIntegrity(), true);
}

TEST(AvlTreeTest, LoadSave) {
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

    std::string out_string;
    tree->save(out_string);

    AvlTree* newtree = new AvlTree();
    newtree->load(out_string);

    EXPECT_EQ(tree->root(), newtree->root());
    EXPECT_EQ(tree->size(), newtree->size());

    ASSERT_EQ(newtree->checkBalance(), true);
    ASSERT_EQ(newtree->checkAggregates(), true);
    ASSERT_EQ(newtree->checkIntegrity(), true);

}
