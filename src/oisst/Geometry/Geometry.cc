/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "oisst/Geometry/Geometry.h"

#include "eckit/config/Configuration.h"

#include "atlas/grid.h"

#include "oops/util/abor1_cpp.h"

namespace oisst {

// ----------------------------------------------------------------------------

  Geometry::Geometry(const eckit::Configuration & conf,
                     const eckit::mpi::Comm & comm) : comm_(comm) {
    atlas::RegularLonLatGrid atlasRllGrid(conf.getString("grid"));
    atlasFunctionSpace_.reset(
      new atlas::functionspace::StructuredColumns(atlasRllGrid,
      atlas::option::halo(0)));

    atlasFieldSet_.reset(new atlas::FieldSet());
  }

// ----------------------------------------------------------------------------

  Geometry::Geometry(const Geometry & other) : comm_(other.comm_) {
    atlasFunctionSpace_.reset(
      new atlas::functionspace::StructuredColumns(other.atlasFunctionSpace_->grid(),
      atlas::option::halo(0)));

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int ifield = 0; ifield < other.atlasFieldSet_->size(); ifield++) {
      atlas::Field atlasField = other.atlasFieldSet_->field(ifield);
      atlasFieldSet_->add(atlasField);
    }
  }

// ----------------------------------------------------------------------------

  Geometry::~Geometry() {}

// ----------------------------------------------------------------------------

  void Geometry::print(std::ostream & os) const {
    int nx, ny;
    ny = static_cast<int>(atlasFunctionSpace()->grid().ny());
    nx = static_cast<int>(((atlas::RegularLonLatGrid&)(atlasFunctionSpace()->grid())).nx());

    os << "Geometry: nx = " << nx << ", ny = " << ny << std::endl;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
