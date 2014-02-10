/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Southwest Research Institute
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 * 	notice, this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright
 * 	notice, this list of conditions and the following disclaimer in the
 * 	documentation and/or other materials provided with the distribution.
 * 	* Neither the name of the Southwest Research Institute, nor the names
 *	of its contributors may be used to endorse or promote products derived
 *	from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FILTER_BASE_H_
#define FILTER_BASE_H_

#include <typeinfo>
#include "ros/assert.h"
#include "ros/console.h"
#include "ros/ros.h"
// is this the worst dep?
#include <moveit/planning_request_adapter/planning_request_adapter.h>
#include <class_loader/class_loader.h>

namespace industrial_trajectory_filters
{

/**
 * @brief Message Adapter structure serves as a stand-in for the arm navigation message type with
 * which the filter was specialized.
 *
 */
struct MessageAdapter
{

  /**
   * @brief Arm navigation message type for trajectory filter
   *
   */
  struct Request
  {
    trajectory_msgs::JointTrajectory trajectory;
  } request;
};

/**
 * @brief This version of the FilterBase<T> can be used to encapsulate the functionality from an arm navigation
 *   trajectory filter into a PlanningRequestAdapter that is used by MoveIt.
 *
 * 	 Since this version of the FilterBase<T> class only provides some degree of backwards compatibility
 * 	 with the original FilterBase interface, then some parts of the arm navigation filter must be modified in order
 * 	 to accommodate the differences introduced by this new interface class.
 *
 * 	 The purely virtual methods "update" and "configure" have been preserved since it is assumed that any
 * 	 trajectory filter specialization implemented at least these two methods.  The getter methods for the
 * 	 filter's type and name have been kept as well, however the corresponding members aren't populated from within
 * 	 the scope of the FilterBase<T> class. Thus, the users implementation must fill these members with
 * 	 the appropriate string data.
 *
 * 	 All the "getParam" methods for parameter lookup have been removed and so the filter implementation
 * 	 must be modified to read parameters using the regular roscpp parameter API (either through the node
 * 	 handle or the bare version)
 *
 * Steps for conversion to a Moveit PlanningRequestAdapter:
 *
 *   a - Remove all arm_navigation header files since these can not be included in a catkin based package.
 *
 *   b - Remove all header files that belong to a rosbuild only package.
 *
 *   c - Include the #include <industrial_trajectory_filters/filter_base.h>" header file in your filter's
 *   	 implementation header file.
 *
 *   d-  In your filter implementation, replace the base class "filters::FilterBase<T>" with
 *   	 "industrial_trajectory_filters::FilterBase<T>"
 *
 *   e-  Remove all calls to the getParam() methods from the old FilterBase<T> class and use the roscpp
 *   	 parameter interface for any parameter lookup tasks instead.
 *
 *   f-  In your filter's header file, declare a typedef that specializes your filter template implementation
 *   	 to a version that uses the "MessageAdapter" structure. For instance, if we are converting the NPointFilter
 *   	 filter class then we would add the following after the class declaration:
 *
 *   	 typedef NPointFilter<MessageAdapter> NPointFilterAdapter;
 *
 *   	 In case the "MessageAdapter" structure does not fit the expected template class then the filter's implementation
 *   	 must provide its own adapter structure that resembles the necessary parts of expected arm_navigation message type.
 *   	 The specialization of such filter would then pass the custom adapter structure in the template argument as follows:
 *
 *   	 typedef NPointFilter<CustomMessageAdapter> NPointFilterAdapter;
 *
 *   g-  In your filter's source file, remove the plugin declaration "PLUGINLIB_DECLARE_CLASS" and add the
 *   	 class loader macro.  In the case of the "NPointFilter" this would be as follows:
 *
 *   	 CLASS_LOADER_REGISTER_CLASS(industrial_trajectory_filters::NPointFilterAdapter,
 *		 planning_request_adapter::PlanningRequestAdapter);
 *
 *	 h - All parameters used by your filter must me made available as a regular ros parameter.  For instance, in the
 *	     "ompl_planning_pipeline.launch" file in a moveit package for your robot you would add the following element:
 *	     <param name="my_filter_parameter" value="5" />
 *
 *	 i - (Optional)  the "adaptAndPlan" methods default implementation already maps the trajectory data between the
 *	 	 MessageAdapter structure and the planning interface objects in the argument list.  The filter's implementation
 *	 	 should override this method whenever a custom adapter structure is used.
 */
template<typename T>
  class FilterBase
  {
  public:

    /**
     * @brief Default constructor
     */
    FilterBase() : configured_(false)
    {

    };

    /**
     * Default destructor
     */
    virtual ~FilterBase()
    {
    }
    ;

    /**
     * @brief Original FilterBase method, return filter type
     * @return filter type (as string)
     */
    std::string getType()
    {
      return filter_type_;
    }
    ;

    /**
     * @brief Original FitlerBase Method
     * @return filter name (as string).
     */
    inline const std::string& getName()
    {
      return filter_name_;
    }
    ; // original FilterBase method

    /**
     * @brief Update the filter and return the data separately. This function
     * must be implemented in the derived class.
     *
     * @param data_in A reference to the data to be input to the filter
     * @param data_out A reference to the data output location
     * @return true on success, otherwise false.
     */
    virtual bool update(const T& data_in, T& data_out)=0;

    //virtual std::string getFilterType(void)=0;
    
  protected:
    /**
     * @brief  filter name
     */
    std::string filter_name_;

    /**
     * The type of the filter (Used by FilterChain for Factory construction)
     */
    std::string filter_type_;

    /**
     * @brief FilterBase method for the sub class to configure the filter
     * This function must be implemented in the derived class.
     * @return true if successful, otherwise false.
     */
    // KAT KEEP THIS, DO WE PASS NH THRU OR have node ask for what it needs?
    virtual bool configure()=0;

    /**
     * @brief Holds filter configuration state.
     */
    bool configured_;


  protected:

    /**
     * @brief Return description string
     * @return description (as a string)
     */
    virtual std::string getDescription() const
    {
      // non const pointer to this
      FilterBase<MessageAdapter> *p = const_cast<FilterBase<MessageAdapter>*>(this);

      std::stringstream ss;
      ss << "Trajectory filter '" << p->getName() << "' of type '" << p->getType() << "'";
      return ss.str();
    }
  };

}

#endif /* FILTER_BASE_H_ */
