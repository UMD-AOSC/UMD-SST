/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "oisst/Geometry/Geometry.h"

#include "eckit/config/Configuration.h"

#include "oops/util/abor1_cpp.h"

namespace oisst {

// ----------------------------------------------------------------------------

  Geometry::Geometry(const eckit::Configuration & conf,
                     const eckit::mpi::Comm & comm)
    : comm_(comm) {
    util::abor1_cpp("Geometry::Geometry() needs to be implemented.",
                    __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  Geometry::Geometry(const Geometry & other)
    : comm_(other.comm_) {
    util::abor1_cpp("Geometry::Geometry() needs to be implemented.",
                     __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  Geometry::~Geometry() {
    util::abor1_cpp("Geometry::~Geometry() needs to be implemented.",
                     __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  void Geometry::print(std::ostream & os) const {
    util::abor1_cpp("Geometry::print() needs to be implemented.",
                    __FILE__, __LINE__);
    os << "Geometry: "
       << "(TODO, print diagnostic info about the geometry here)"
       << std::endl;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
