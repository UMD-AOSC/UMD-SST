/*
 * (C) Copyright 2020-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <string>

#include "oisst/Fields/Fields.h"
#include "oisst/Geometry/Geometry.h"
#include "oisst/State/State.h"

#include "atlas/field.h"
#include "atlas/array.h"
#include "atlas/option.h"

#include "oops/util/abor1_cpp.h"
#include "oops/util/missingValues.h"

using atlas::array::make_view;
using atlas::option::name;

namespace oisst {

// ----------------------------------------------------------------------------

  Fields::Fields(const Geometry & geom, const oops::Variables & vars,
                 const util::DateTime & vt)
    : geom_(new Geometry(geom)), time_(vt), vars_(vars) {
    // the constructor that gets called by everything (all State
    //  and Increment constructors ultimately end up here)

    if (vars_.size() != 1) {
      util::abor1_cpp("Fields::Fields(), vars_.size() != 1",
                       __FILE__, __LINE__);
    }

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<double>(
                         name(var));
      auto fd = make_view<double, 1>(fld);
      fd.assign(0.0);

      atlasFieldSet_->add(fld);
    }
  }

// ----------------------------------------------------------------------------

  Fields::Fields(const Fields & other)
    : Fields(*other.geom_, other.vars_, other.time_) {
    // copy data from object other
    *this = other;
  }

// ----------------------------------------------------------------------------
  Fields & Fields::operator =(const Fields & other) {
    const int size = geom_->atlasFunctionSpace()->size();
    for (int i = 0; i < vars_.size(); i++) {
      auto fd       = make_view<double, 1>(atlasFieldSet_->field(0));
      auto fd_other = make_view<double, 1>(other.atlasFieldSet_->field(0));
      for (int j = 0; j < size; j++)
        fd(j) = fd_other(j);
    }
    return *this;
  }

// ----------------------------------------------------------------------------

  Fields & Fields::operator+=(const Fields &other) {
    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);
    auto fd       = make_view<double, 1>(atlasFieldSet_->field(0));
    auto fd_other = make_view<double, 1>(other.atlasFieldSet_->field(0));

    for (int j = 0; j < size; j++) {
      if (fd(j) == missing || fd_other(j) == missing)
        fd(j) = missing;
      else
        fd(j) += fd_other(j);
    }

    return *this;
  }

// ----------------------------------------------------------------------------

  void Fields::accumul(const double &zz, const Fields &rhs) {
    const size_t size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));
    auto fd_rhs = make_view<double, 1>(rhs.atlasFieldSet()->field(0));

    for (size_t i = 0; i < size; i++) {
      if (fd(i) == missing || fd_rhs(i) == missing)
        fd(i) = missing;
      else
        fd(i) +=  zz*fd_rhs(i);
    }
  }

// ----------------------------------------------------------------------------

  double Fields::norm() const {
    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));

    int nValid = 0;
    double norm = 0.0, s = 0.0;
    for (int i = 0; i < size; i++) {
      if (fd(i) != missing) {
        nValid += 1;
        s += fd(i)*fd(i);
      }
    }

    if (nValid == 0)
      norm = 0.0;
    else
      norm = sqrt(s/(1.0*nValid));

    return norm;
  }

// ----------------------------------------------------------------------------

  void Fields::zero() {
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));

    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);

    for (int i = 0; i < size; i++)
        fd(i) = 0.0;
  }

// ----------------------------------------------------------------------------

  std::shared_ptr<const Geometry> Fields::geometry() const {
    return geom_;
  }

// ----------------------------------------------------------------------------

  std::shared_ptr<atlas::FieldSet> Fields::atlasFieldSet() const {
    return atlasFieldSet_;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
