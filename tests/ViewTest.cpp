#include "../View.hpp"
#include <gtest/gtest.h>

using namespace Jah;

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

TEST_F(ViewTest, ManagerViewShouldFilterPaths) {
  const View managerView(Manager);
  const std::vector<fs::path> paths = {"/path1", "/path2"};
  const auto filteredPaths = managerView.filter(paths);

  EXPECT_FALSE(filteredPaths.empty());
  EXPECT_EQ(filteredPaths.size(), paths.size()); // Manager should see all paths
}

TEST_F(ViewTest, DeveloperViewDefaultVisibility) {
  const View developerView(Developer);
  const fs::path testPath("/test/path");

  EXPECT_FALSE(developerView.canSee(testPath));
  EXPECT_TRUE(developerView.canSee(
      "/developer/path")); // Developer should see their own path
}

TEST_F(ViewTest, QAViewShouldHaveAppropriateAccess) {
  const View qaView(QA);
  const std::vector<fs::path> testPaths = {"/test1", "/test2", "/test3"};

  const auto filteredPaths = qaView.filter(testPaths);
  EXPECT_FALSE(filteredPaths.empty());

  EXPECT_TRUE(qaView.canSee("/test/qa/path"));
  EXPECT_FALSE(qaView.canSee("/developer/path"));
}

TEST_F(ViewTest, TeamLeadViewShouldHaveExpandedAccess) {
  const View teamLeadView(TeamLead);
  const std::vector<fs::path> paths = {"/proj1", "/proj2", "/proj3"};

  const auto filteredPaths = teamLeadView.filter(paths);
  EXPECT_FALSE(filteredPaths.empty());
  EXPECT_EQ(filteredPaths.size(),
            paths.size()); // Team lead should see all project paths

  EXPECT_TRUE(teamLeadView.canSee("/team/lead/path"));
  EXPECT_TRUE(teamLeadView.canSee(
      "/developer/path")); // Team lead should see developer paths
}

TEST_F(ViewTest, ViewAccessHierarchy) {
  const View managerView(Manager);
  const View qaView(QA);
  const View teamLeadView(TeamLead);
  const fs::path sharedPath("/shared/path");

  // Test access hierarchy
  EXPECT_TRUE(managerView.canSee(sharedPath));  // Manager has highest access
  EXPECT_FALSE(qaView.canSee(sharedPath));      // QA has limited access
  EXPECT_TRUE(teamLeadView.canSee(sharedPath)); // Team lead has elevated access
}

TEST_F(ViewTest, RoleSpecificPathVisibility) {
  for (size_t i = 0; i < commonPaths.size(); ++i) {
    const View roleView(static_cast<Observer>(
        i)); // Assuming Observer enum values match path indices

    // Each role should see their own path
    EXPECT_TRUE(roleView.canSee(commonPaths[i]))
        << "Role " << i << " should see their own path: " << commonPaths[i];

    // Test cross-role visibility based on role hierarchy
    if (static_cast<Observer>(i) == Manager) {
      // Manager should see all paths
      for (const auto &path : commonPaths) {
        EXPECT_TRUE(roleView.canSee(path));
      }
    }
  }
}