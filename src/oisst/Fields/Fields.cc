/*
 * (C) Copyright 2020-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "oisst/Fields/Fields.h"
#include "oisst/State/State.h"

#include "oops/util/missingValues.h"

#include "atlas/field.h"
#include "atlas/array.h"
#include "atlas/option.h"

using atlas::array::make_view;

namespace oisst {

// ----------------------------------------------------------------------------

  Fields & Fields::operator+=(const Fields &other) {
    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);

    for (int i = 0; i < vars_.size(); i++) {
      auto fd       = make_view<double, 1>(atlasFieldSet_->field(i));
      auto fd_other = make_view<double, 1>(other.atlasFieldSet_->field(i));
      for (int j = 0; j < size; j++) {
        if (fd(j) == missing || fd_other(j) == missing)
          fd(j) = missing;
        else
          fd(j) += fd_other(j);
      }
    }

    return *this;
  }

// ----------------------------------------------------------------------------

  void Fields::accumul(const double &zz, const State &rhs) {
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));
    auto fd_rhs = make_view<double, 1>(rhs.atlasFieldSet()->field(0));
    double missing = util::missingValue(missing);
    const size_t size = geom_->atlasFunctionSpace()->size();

    for (size_t i = 0; i < size; i++) {
      if (fd(i) == missing || fd_rhs(i) == missing)
        fd(i) = missing;
      else
        fd(i) +=  zz*fd_rhs(i);
    }
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
