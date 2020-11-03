/*
 * (C) Copyright 2020-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OISST_FIELDS_FIELDS_H_
#define OISST_FIELDS_FIELDS_H_

#include <memory>

#include "oisst/Geometry/Geometry.h"

#include "atlas/field.h"

#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"

// forward declarations
namespace oisst {
  class State;
}

// ----------------------------------------------------------------------------

namespace oisst {

  class Fields {
   public:
    Fields() {}
    Fields(const Geometry & geom, const oops::Variables & vars,
           const util::DateTime & vt)
           : geom_(new Geometry(geom)), time_(vt), vars_(vars) {}

    Fields & operator+=(const Fields &);
    void accumul(const double &, const State &);

   public:
    std::shared_ptr<atlas::FieldSet> atlasFieldSet_;

    oops::Variables vars_;
    std::shared_ptr<const Geometry> geom_;
    util::DateTime time_;
  };
}  // namespace oisst

#endif  // OISST_FIELDS_FIELDS_H_
