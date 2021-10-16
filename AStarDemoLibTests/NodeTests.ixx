module;

#include <memory>
#include <gtest/gtest.h>

export module NodeTests;

import AStarLib;

using namespace testing;

TEST(NodeTests, TestConstructor)
{
    Node node(5, 6);

    ASSERT_EQ(5, node.row());
    ASSERT_EQ(6, node.col());
}

TEST(NodeTests, TestCost)
{
    Node node(5, 6);

    ASSERT_EQ(0.0, node.cost());

    const auto cost = 5.3;
    node.set_cost(cost);
    ASSERT_EQ(cost, node.cost());
}

TEST(NodeTests, TestEstimation)
{
    Node node(5, 6);

    ASSERT_EQ(0.0, node.estimation());

    const auto estimation = 5.3;
    node.set_estimation(estimation);
    ASSERT_EQ(estimation, node.estimation());
}

TEST(NodeTests, TestEquality)
{
    Node node1(20, 20);
    Node node2(20, 20);

    ASSERT_EQ(node1, node2);
}

TEST(NodeTests, TestInequality)
{
    Node node1(20, 20);
    Node node2(23, 25);

    ASSERT_NE(node1, node2);
}

TEST(NodeTests, TestTotalCost)
{
    Node node(15, 20);

    node.set_cost(34);
    node.set_estimation(10);

    ASSERT_EQ(node.cost() + node.estimation(), node.total_cost());
}

TEST(NodeTests, TestParent)
{
    Node node(15, 20);

    ASSERT_EQ(node.get_parent().get(), nullptr);

    auto parentNode = std::make_shared<Node>(45, 56);
    node.set_parent(parentNode);

    ASSERT_NE(node.get_parent().get(), nullptr);
    ASSERT_EQ(parentNode, node.get_parent());

}

export class NodeTests;
