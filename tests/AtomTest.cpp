#include "../Atom.hpp"
#include <gtest/gtest.h>

using namespace Jah;

class AtomTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code if needed
  }

  void TearDown() override {
    // Cleanup code if needed
  }
};

TEST_F(AtomTest, CreateAtom) {
  Atom atom("type", "summary", "body", "footer");
  EXPECT_EQ(atom.getType(), "type");
  EXPECT_EQ(atom.getSummary(), "summary");
  EXPECT_EQ(atom.getBody(), "body");
  EXPECT_EQ(atom.getFooter(), "footer");
}

TEST_F(AtomTest, ContainsIssue) {
  Atom atom("type", "summary", "body", "Related to issue 123");
  EXPECT_TRUE(atom.containsIssue(123));
  EXPECT_FALSE(atom.containsIssue(456));
}

TEST_F(AtomTest, EmptyFooterIssueCheck) {
  Atom atom("type", "summary", "body", "");
  EXPECT_FALSE(atom.containsIssue(123));
}