/* MapTests.ixx - unit tests for the Map class
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