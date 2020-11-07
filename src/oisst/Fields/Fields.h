/*
 * (C) Copyright 2020-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OISST_FIELDS_FIELDS_H_
#define OISST_FIELDS_FIELDS_H_

#include <memory>
#include <vector>

#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/Serializable.h"

// forward declarations
namespace oisst {
  class Geometry;
  class State;
}

namespace atlas {
  class FieldSet;
}

// ----------------------------------------------------------------------------

namespace oisst {

  class Fields : public util::Serializable {
   public:
    Fields(const Geometry & , const oops::Variables & ,
           const util::DateTime &);
    Fields(const Fields &);

    // math operators
    Fields & operator =(const Fields &);
    Fields & operator+=(const Fields &);
    void accumul(const double &, const Fields &);
    void zero();
    double norm() const;

    // time manipulation
    void updateTime(const util::Duration & dt) { time_ += dt; }
    const util::DateTime & validTime() const { return time_; }
    util::DateTime & validTime() { return time_; }

    // Serialization (not needed by our project)
    size_t serialSize() const override { return 0; }
    void serialize(std::vector<double> &) const override {}
    void deserialize(const std::vector<double> &, size_t &) override {}

    // other accessors
    std::shared_ptr<atlas::FieldSet> atlasFieldSet() const;
    std::shared_ptr<const Geometry> geometry() const;
    const oops::Variables & variables() const { return vars_; }

   public:
    std::shared_ptr<atlas::FieldSet> atlasFieldSet_;
    std::shared_ptr<const Geometry> geom_;
    util::DateTime time_;
    oops::Variables vars_;
  };
}  // namespace oisst

#endif  // OISST_FIELDS_FIELDS_H_
