# Industrial Core
[![Build Status](http://jenkins.ros.org/job/devel-hydro-industrial_core/badge/icon)](http://jenkins.ros.org/job/devel-hydro-industrial_core/)<BR>
[ROS-Industrial][] core meta-package.  See the [ROS wiki][] page for more information.  

## Contents

This repo holds source code for all versions > groovy. For those versions <= groovy see: [SVN repo][]

[ROS-Industrial]: http://www.ros.org/wiki/Industrial
[ROS wiki]: http://ros.org/wiki/industrial_core
[SVN repo]: https://code.google.com/p/swri-ros-pkg/source/browse

## HOW TO INSTALL

* Install ros-core.
* Create your ros  workspace.
* Install this package to your workspace following the [standard procedure](http://wiki.ros.org/catkin/Tutorials/workspace_overlaying). 
* This package depends on MoveIT. On Ubuntu install it with  sudo apt-get install ros-hydro-moveit-full or your favorite package manager.
* For unit tests you will need [googletest](https://code.google.com/p/googletest/) which installs easily using cmake. 
* For additional assistance with unit tests this is a [helpful resource](http://www.thebigblob.com/getting-started-with-google-test-on-ubuntu/). 
* Run the tests with catkin_make run_tests_industrial_trajectory_filters_gtest_utest_trajectory
