/* NodeTests.ixx - unit tests for the Node class
 * Copyright (C) 2021 Paulo Pinto
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
module;

#include <memory>
#include <gtest/gtest.h>

export module NodeTests;

import AStarLib;

using namespace AStarLib;

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
