// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief A header file with declaration for FaceDetection Class
 * @file head_pose_detection.hpp
 */
#ifndef DYNAMIC_VINO_LIB__INFERENCES__HEAD_POSE_DETECTION_HPP_
#define DYNAMIC_VINO_LIB__INFERENCES__HEAD_POSE_DETECTION_HPP_

#include <memory>
#include <vector>
#include <string>

#include "dynamic_vino_lib/engines/engine.hpp"
#include "dynamic_vino_lib/inferences/base_inference.hpp"
#include "dynamic_vino_lib/models/head_pose_detection_model.hpp"
#include "inference_engine.hpp"
#include "opencv2/opencv.hpp"

namespace dynamic_vino_lib
{
/**
 * @class HeadPoseResult
 * @brief Class for storing and processing headpose detection result.
 */
class HeadPoseResult : public Result
{
public:
  friend class HeadPoseDetection;
  explicit HeadPoseResult(const cv::Rect & location);
  /**
   * @brief Get the yawl angle of the headpose.
   * @return The yawl value.
   */
  float getAngleY() const
  {
    return angle_y_;
  }
  /**
   * @brief Get the pitch angle of the headpose.
   * @return The pitch value.
   */
  float getAngleP() const
  {
    return angle_p_;
  }
  /**
   * @brief Get the roll angle of the headpose.
   * @return The roll value.
   */
  float getAngleR() const
  {
    return angle_r_;
  }

private:
  float angle_y_ = -1;
  float angle_p_ = -1;
  float angle_r_ = -1;
};

/**
 * @class HeadPoseDetection
 * @brief Class to load headpose detection model and perform headpose detection.
 */
class HeadPoseDetection : public BaseInference
{
public:
  using Result = dynamic_vino_lib::HeadPoseResult;
  HeadPoseDetection();
  ~HeadPoseDetection() override;
  /**
   * @brief Load the headpose detection model.
   */
  void loadNetwork(std::shared_ptr<Models::HeadPoseDetectionModel>);
  /**
   * @brief Enqueue a frame to this class.
   * The frame will be buffered but not infered yet.
   * @param[in] frame The frame to be enqueued.
   * @param[in] input_frame_loc The location of the enqueued frame with respect
   * to the frame generated by the input device.
   * @return Whether this operation is successful.
   */
  bool enqueue(const cv::Mat & frame, const cv::Rect &) override;
  /**
   * @brief Start inference for all buffered frames.
   * @return Whether this operation is successful.
   */
  bool submitRequest() override;
  /**
   * @brief This function will fetch the results of the previous inference and
   * stores the results in a result buffer array. All buffered frames will be
   * cleared.
   * @return Whether the Inference object fetches a result this time
   */
  bool fetchResults() override;
  /**
   * @brief Get the length of the buffer result array.
   * @return The length of the buffer result array.
   */
  const int getResultsLength() const override;
  /**
   * @brief Get the location of result with respect
   * to the frame generated by the input device.
   * @param[in] idx The index of the result.
   */
  const dynamic_vino_lib::Result * getLocationResult(int idx) const override;
  /**
   * @brief Get the name of the Inference instance.
   * @return The name of the Inference instance.
   */
  const std::string getName() const override;
  /**
   * @brief Show the observed detection result either through image window
     or ROS topic.
   */
  const void observeOutput(const std::shared_ptr<Outputs::BaseOutput> & output) override;

  std::vector<Result> getResults()
  {
    return results_;
  }

private:
  std::shared_ptr<Models::HeadPoseDetectionModel> valid_model_;
  std::vector<Result> results_;
};
}  // namespace dynamic_vino_lib
#endif  // DYNAMIC_VINO_LIB__INFERENCES__HEAD_POSE_DETECTION_HPP_
