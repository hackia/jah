#include "../View.hpp"
#include <gtest/gtest.h>

using namespace Jah;
namespace fs = std::filesystem;
class ViewTest : public ::testing::Test {
protected:
  // Add common test data as member variables
  std::vector<fs::path> commonPaths;

  void SetUp() override {
    commonPaths = {"/manager/path", "/developer/path", "/designer/path",
                   "/qa/path", "/team_lead/path"};
  }
};

TEST_F(ViewTest, ViewConstructorShouldNotThrow) {
  EXPECT_NO_THROW({ View view(Manager); });
}

TEST_F(ViewTest, MultipleViewConstructorsShouldNotThrow) {
  EXPECT_NO_THROW({
    View managerView(Manager);
    View developerView(Developer);
    View designerView(Designer);
  });
}