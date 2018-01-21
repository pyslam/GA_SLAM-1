#pragma once

// GA SLAM
#include "ga_slam/TypeDefs.hpp"
#include "ga_slam/mapping/Map.hpp"

// Eigen
#include <Eigen/Geometry>

// PCL
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

// STL
#include <mutex>
#include <atomic>

namespace ga_slam {

class PoseCorrection {
  public:
    PoseCorrection(void)
        : globalMapInitialized_(false),
          globalMap_(),
          globalMapPose_(Pose::Identity()),
          lastCorrectedPose_(Pose::Identity()) {}

    PoseCorrection(const PoseCorrection&) = delete;
    PoseCorrection& operator=(const PoseCorrection&) = delete;
    PoseCorrection(PoseCorrection&&) = delete;
    PoseCorrection& operator=(PoseCorrection&&) = delete;

    const Map& getGlobalMap(void) const { return globalMap_; }

    std::mutex& getGlobalMapMutex(void) { return globalMapMutex_; }

    void configure(
            double traversedDistanceThreshold,
            double minSlopeThreshold,
            double slopeSumThresholdMultiplier,
            double matchAcceptanceThreshold,
            double globalMapLength,
            double globalMapResolution);

    void createGlobalMap(
            const Cloud::ConstPtr& globalCloud,
            const Pose& globalCloudPose);

    bool distanceCriterionFulfilled(const Pose& pose) const;

    bool featureCriterionFulfilled(const Map& localMap) const;

    bool matchMaps(
            const Map& localMap,
            const Pose& currentPose,
            Pose& correctedPose);

  protected:
    std::atomic<bool> globalMapInitialized_;

    Map globalMap_;
    mutable std::mutex globalMapMutex_;

    Pose globalMapPose_;
    Pose lastCorrectedPose_;

    double traversedDistanceThreshold_;
    double minSlopeThreshold_;
    double slopeSumThresholdMultiplier_;
    double matchAcceptanceThreshold_;
};

}  // namespace ga_slam
