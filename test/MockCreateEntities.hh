/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGNITION_PHYSICS_TEST_MOCKCREATEENTITY_HH_
#define IGNITION_PHYSICS_TEST_MOCKCREATEENTITY_HH_

#include <memory>

#include <ignition/physics/FeatureList.hh>
#include <ignition/physics/FramedQuantity.hh>

namespace mock
{
  /////////////////////////////////////////////////
  /// \brief A feature for creating and retrieving links from an engine. This
  /// is used by the mock Frame Semantics plugin so that we can test the
  /// Frame Semantics feature.
  struct MockCreateEntities : public ignition::physics::Feature
  {
    using Identity = ignition::physics::Identity;

    template <typename PolicyT, typename FeaturesT>
    class Engine : public virtual Feature::Engine<PolicyT, FeaturesT>
    {
      public: using FrameData =
        ignition::physics::FrameData<typename PolicyT::Scalar, PolicyT::Dim>;

      public: using LinkPtr = ignition::physics::EntityPtr<ignition::physics::Link<PolicyT, FeaturesT>>;
      public: using Joint = ignition::physics::Joint<PolicyT, FeaturesT>;

      /// \brief Create a link, giving it a name and data. The data is relative
      /// to the world frame.
      public: LinkPtr CreateLink(
          const std::string &_linkName,
          const FrameData &_frameData);

      /// \brief Create a joint, giving it a name and data. The data is relative
      /// to the world frame.
      public: std::unique_ptr<Joint> CreateJoint(
          const std::string &_jointName,
          const FrameData &_frameData);

      /// \brief Retrieve a link that was created earlier.
      public: LinkPtr GetLink(const std::string &_linkName) const;

      /// \brief Retrieve a joint that was created earlier.
      public: std::unique_ptr<Joint> GetJoint(
          const std::string &_jointName) const;
    };

    template <typename PolicyT>
    class Implementation : public virtual Feature::Implementation<PolicyT>
    {
      public: using FrameData =
        ignition::physics::FrameData<typename PolicyT::Scalar, PolicyT::Dim>;

      public: virtual Identity CreateLink(
          const std::string &_linkName,
          const FrameData &_frameData) = 0;

      public: virtual Identity CreateJoint(
          const std::string &_jointName,
          const FrameData &_frameData) = 0;

      public: virtual Identity GetLink(
          const std::string &_linkName) const = 0;

      public: virtual Identity GetJoint(
          const std::string &_jointName) const = 0;
    };
  };


  // ---------------------- Implementations ----------------------

  /////////////////////////////////////////////////
  template <typename PolicyT, typename FeaturesT>
  auto MockCreateEntities::Engine<PolicyT, FeaturesT>::CreateLink(
      const std::string &_linkName,
      const FrameData &_frameData) -> LinkPtr
  {
    return LinkPtr(
          this->pimpl,
          this->template Interface<MockCreateEntities>()
            ->CreateLink(_linkName, _frameData));
  }

  /////////////////////////////////////////////////
  template <typename PolicyT, typename FeaturesT>
  auto MockCreateEntities::Engine<PolicyT, FeaturesT>::CreateJoint(
      const std::string &_jointName,
      const FrameData &_frameData) -> std::unique_ptr<Joint>
  {
    const Identity jointId =
        this->template Interface<MockCreateEntities>()
          ->CreateJoint(_jointName, _frameData);

    if (!jointId)
      return nullptr;

    return std::make_unique<Joint>(this->pimpl, jointId);
  }

  /////////////////////////////////////////////////
  template <typename PolicyT, typename FeaturesT>
  auto MockCreateEntities::Engine<PolicyT, FeaturesT>::GetLink(
      const std::string &_linkName) const -> LinkPtr
  {
//    const Identity linkId =
//        this->template Interface<MockCreateEntities>()->GetLink(_linkName);

//    if (!linkId)
//      return nullptr;

//    return std::make_unique<LinkPtr>(this->pimpl, linkId);

    return LinkPtr(
          this->pimpl,
          this->template Interface<MockCreateEntities>()->GetLink(_linkName));
  }

  /////////////////////////////////////////////////
  template <typename PolicyT, typename FeaturesT>
  auto MockCreateEntities::Engine<PolicyT, FeaturesT>::GetJoint(
      const std::string &_jointName) const -> std::unique_ptr<Joint>
  {
    const Identity jointId =
        this->template Interface<MockCreateEntities>()->GetJoint(_jointName);

    if (!jointId)
      return nullptr;

    return std::make_unique<Joint>(this->pimpl, jointId);
  }
}

#endif
