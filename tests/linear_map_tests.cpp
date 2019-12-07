#include "gtest-1.8.1/gtest.h"
#include <string>
#include <utility>
#include <list>
#include "../src/modulos_basicos/linear_map.h"

using namespace std;

template<class K, class S>
using my_map = linear_map<K, S>;
using Dato = string;

class MapTest : public testing::Test {
 protected:
  my_map<int, int> empty;
  my_map<int, int> ordered;
  my_map<int, int> inversed;
  my_map<int, int> smaller;
  my_map<string, int> string_map;
  my_map<string, Dato> datos;

  void SetUp() {
    for (uint i = 0; i < 5; i++) {
      ordered.insert(make_pair(i, i));
      inversed.insert(make_pair(i, 5-i));
    }
    for (uint i = 0; i < 3; i++) {
      smaller.insert(make_pair(i, i));
    }

    vector<string> string_keys = {
      "Haciendo", "palmas", "arriba"
    };
    for (uint i = 0; i < string_keys.size(); i++) {
      string_map.insert(make_pair(string_keys[i], i));
      datos.insert(make_pair(string_keys[i], Dato(string_keys[i])));
    }
  }
};

TEST(my_map, crear) {
 my_map<int, int> a;
 my_map<int, string> b;
 my_map<string, my_map<string, int>> aa;
 my_map<my_map<int, int>, string> ab;
}

TEST(my_map, insert) {
  my_map<int, int> a;
  for (int i = 0; i < 5; i++) {
    a.insert(make_pair(i, i));
   }
  
  my_map<int, char> b;
  string s = "abcde";
  for (int i = 0; i < 5; i++) {
    b.insert(make_pair(i, s[i]));
  }
}

TEST_F(MapTest, size) {
  EXPECT_EQ(smaller.size(), 3u);
  EXPECT_EQ(ordered.size(), 5u);
  EXPECT_EQ(inversed.size(), 5u);
}

TEST_F(MapTest, at) {
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(ordered.at(i), i);
    EXPECT_EQ(inversed.at(i), 5-i);
  }
}

TEST_F(MapTest, copy) {
  auto copy_ordered = ordered;
  auto copy_smaller = smaller;
  auto copy_string = string_map;
  auto copy_datos = datos;

  EXPECT_EQ(copy_smaller.size(), 3u);
  EXPECT_EQ(copy_ordered.size(), 5u);
  EXPECT_EQ(copy_string.size(), 3u);
  EXPECT_EQ(copy_datos.size(), 3u);
}

TEST_F(MapTest, count) {
  for (int i = -5; i < 10; i++) {
    EXPECT_EQ(smaller.count(i), not (i < 0 or i > 2));
    EXPECT_EQ(ordered.count(i), not (i < 0 or i > 4));
    EXPECT_EQ(inversed.count(i), not (i < 0 or i > 4));
  }
}

TEST_F(MapTest, erase_by_key) {
  auto copy = ordered;
  EXPECT_TRUE(copy.count(3));
  EXPECT_EQ(copy.erase(3), 1u);
  EXPECT_FALSE(copy.count(3));
  EXPECT_EQ(copy.size(), 4u);
}

TEST_F(MapTest, iterator) {
  linear_map<int, int> copy_ordered;
  for (auto x : ordered) {
    copy_ordered.insert(x);
  }

  linear_map<string, int> copy_string;
  for(auto x : string_map) {
    copy_string.insert(x);
  }

  EXPECT_EQ(copy_ordered, ordered);
  EXPECT_EQ(copy_string, string_map);
}

TEST_F(MapTest, const_iterator) {
  const linear_map<int, int> &const_ordered = ordered;
  linear_map<int, int> copy_ordered;
  for (auto x : const_ordered) {
    copy_ordered.insert(x);
  }

  const linear_map<string, int> &const_string = string_map;
  linear_map<string, int> copy_string;
  for(auto x : const_string) {
    copy_string.insert(x);
  }

  EXPECT_EQ(copy_ordered, ordered);
  EXPECT_EQ(copy_string, string_map);
}

TEST_F(MapTest, mostrar) {
  stringstream ss_ordered;
  ss_ordered << ordered;
  EXPECT_EQ(ss_ordered.str(), string("{ 0: 0, 1: 1, 2: 2, 3: 3, 4: 4 }"));
}

TEST_F(MapTest, iterator_find_test) {
    EXPECT_EQ(ordered.find(-1), ordered.end());
    EXPECT_EQ(ordered.find(5), ordered.end());
    EXPECT_NE(ordered.find(3), ordered.end());
    EXPECT_EQ(ordered.find(3)->second, 3);
}

TEST_F(MapTest, iterator_test) {
    EXPECT_EQ(empty.begin(), empty.end());
    EXPECT_NE(ordered.begin(), ordered.end());

    std::list<int> ks, vs, a = {0, 1 ,2 ,3 ,4}, b = {5, 4, 3, 2 , 1};
    for (auto p : ordered) {
        ks.push_back(p.first);
        vs.push_back(p.second);
    }
    EXPECT_EQ(ks, a);
    EXPECT_EQ(vs, a);

    vs.clear();
    for (auto p : inversed) {
        vs.push_back(p.second);
    }
    EXPECT_EQ(vs, b);
}

TEST_F(MapTest, const_iterator_test) {
    const linear_map<int, int> &c_empty = empty;
    const linear_map<int, int> &c_ordered = ordered;
    const linear_map<int, int> &c_inversed = inversed;
    EXPECT_EQ(c_empty.begin(), c_empty.end());
    EXPECT_NE(c_ordered.begin(), c_ordered.end());

    std::list<int> ks, vs, a = {0, 1 ,2 ,3 ,4}, b = {5, 4, 3, 2 , 1};
    for (auto p : c_ordered) {
        ks.push_back(p.first);
        vs.push_back(p.second);
    }
    EXPECT_EQ(ks, a);
    EXPECT_EQ(vs, a);

    vs.clear();
    for (auto p : c_inversed) {
        vs.push_back(p.second);
    }
    EXPECT_EQ(vs, b);
}

TEST_F(MapTest, insert_by_iterator) {
    auto m1 = ordered;
    EXPECT_EQ(m1.insert(make_pair(6,6)).first->first,6);
    EXPECT_EQ(m1.insert(make_pair(-1,-1)).first->second,-1);
}

TEST_F(MapTest, erase_by_iterator) {
    auto m1 = ordered;
    EXPECT_EQ(m1.erase(1), 1u);
    EXPECT_EQ(m1.find(1), m1.end());
    EXPECT_EQ(m1.erase(4), 1u);
}
