module;

#include <memory>
#include <gtest/gtest.h>

export module MapTests;

import AStarLib;

using namespace AStarLib;

using namespace testing;

TEST(MapTests, TestConstructor)
{
    Map map;

    for (int row = 0; row < map.rows(); ++row) {
        for (int col = 0; col < map.columns(); ++col) {
            ASSERT_EQ(map.at(row, col), Map::CellType::FREE);
        }
    }
}

TEST(MapTests, TestAddPath)
{
    auto startNode = std::make_shared<Node>(20, 20);
    auto endNode = std::make_shared<Node>(21, 21);

    endNode->set_parent(startNode);

    Map map(30, 30);

    map.add_path(endNode.get());

    ASSERT_EQ(map.at(startNode->row(), startNode->col()), Map::CellType::START);
    ASSERT_EQ(map.at(endNode->row(), endNode->col()), Map::CellType::END);
}

export class MapTests;