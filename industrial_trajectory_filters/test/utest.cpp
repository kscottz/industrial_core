// Bring in my package's API, which is what I'm testing
#include <gtest/gtest.h>
#include <industrial_trajectory_filters/uniform_sample_filter.h>
#include <industrial_trajectory_filters/n_point_filter.h>
#include <moveit/planning_request_adapter.h>
#include <ros/builtin_message_traits.h>

// Declare a test
TEST(TestSuite, testCase1)
{

  // QUESTION. WHAT TYPE SHOULD WE TEMPLATE OFF OF


  industrial_trajectory_filters::UniformSampleFilter<JointTracjetoryPoint> derp();
  trajectory_msgs::JointTrajectoryPoint start();
  trajectory_msgs::JointTrajectoryPoint stop();
  trajectory_msgs::JointTrajectoryPoint interp();
  derp.interpolatePt(start,stop,0.0001,interp);
  derp.update(start,stop);

   
  //<test things here, calling EXPECT_* and/or ASSERT_* macros as needed>
}

// Declare another test
TEST(TestSuite, testCase2)
{
  //<test things here, calling EXPECT_* and/or ASSERT_* macros as needed>
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}
