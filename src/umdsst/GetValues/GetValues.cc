/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "umdsst/Geometry/Geometry.h"
#include "umdsst/GetValues/GetValues.h"
#include "umdsst/State/State.h"

#include "eckit/config/Configuration.h"

#include "oops/generic/InterpolatorUnstructured.h"
#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"

namespace umdsst {

// ----------------------------------------------------------------------------

  GetValues::GetValues(const Geometry & geom,
                       const ufo::Locations & locs)
    : geom_(new Geometry(geom)), locs_(locs) {
    oops::InterpolatorUnstructured interpolator(eckit::LocalConfiguration(),
                                                *geom_->atlasFunctionSpace(),
                                                locs_.atlasFunctionSpace());
  }

// -----------------------------------------------------------------------------

  GetValues::~GetValues() { }

// ----------------------------------------------------------------------------

  void GetValues::fillGeoVaLs(const State & state,
                              const util::DateTime & t1,
                              const util::DateTime & t2,
                              ufo::GeoVaLs & geovals) const {
  }

// ----------------------------------------------------------------------------

  void GetValues::print(std::ostream & os) const {
    os << "insert diagnostic information about getvalues"
      << std::endl;
    util::abor1_cpp("GetValues::print() needs to be implemented.",
                     __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

}  // namespace umdsst
