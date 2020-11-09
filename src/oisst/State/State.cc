/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <numeric>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>

#include <limits>
#include <iomanip>

// #include <netcdf>
#include "netcdf"

#include "oisst/Geometry/Geometry.h"
#include "oisst/State/State.h"
#include "oisst/Increment/Increment.h"

#include "eckit/config/Configuration.h"

#include "atlas/field.h"
#include "atlas/array.h"
#include "atlas/option.h"

#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"
#include "oops/util/missingValues.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"

// using namespace netCDF;
using atlas::array::make_view;
using atlas::option::name;


namespace oisst {

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const eckit::Configuration & conf)
    : Fields(geom, oops::Variables(conf, "state variables"),
             util::DateTime(conf.getString("date"))) {
    // check and read in data if it has filename
    if (conf.has("filename"))
      read(conf);
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const oops::Variables & vars,
               const util::DateTime & time)
    : Fields(geom, vars, time) {}

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const State & other)
    : State(other) {
    // Change state resolution, normally used for interpolation.
    // For now, just copy without interpolation
  }

// ----------------------------------------------------------------------------

  State::State(const State & other)
    : Fields(other) {}

// ----------------------------------------------------------------------------

  State::~State() {}

// ----------------------------------------------------------------------------

  State & State::operator+=(const Increment & dx) {
    Fields::operator+=(dx);
    return *this;
  }

// ----------------------------------------------------------------------------

  void State::print(std::ostream & os) const {
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));
    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);

    double mean = 0.0, sum = 0.0,
           min = std::numeric_limits<double>::max(),
           max = std::numeric_limits<double>::min();
    int nValid = 0;

    for (int i = 0; i < size; i++)
      if (fd(i) != missing) {
        if (fd(i) < min) min = fd(i);
        if (fd(i) > max) max = fd(i);

        sum += fd(i);
        nValid++;
      }

    if (nValid == 0) {
      mean = 0.0;
      oops::Log::debug() << "State::print(), nValid == 0!" << std::endl;
    } else {
      mean = sum / (1.0*nValid);
    }

    os << "min = " << min << ", max = " << max << ", mean = " << mean
       << std::endl;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
