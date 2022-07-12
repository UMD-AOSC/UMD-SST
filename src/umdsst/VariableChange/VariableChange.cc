/*
 * (C) Copyright 2021-2022 UCAR, University of Maryland
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "umdsst/Geometry/Geometry.h"
#include "umdsst/State/State.h"
#include "umdsst/VariableChange/VariableChange.h"

#include "oops/util/abor1_cpp.h"

namespace umdsst {

// -----------------------------------------------------------------------------

VariableChange::VariableChange(const Parameters_ & params,
                               const Geometry & geom)
    : geom_(geom) {
}

// -----------------------------------------------------------------------------

void VariableChange::changeVar(State & xx, const oops::Variables & vars) const {

  for ( int v = 0; v < vars.size(); v++ ) {
    std::string name = vars[v];

    if ( xx.variables().has(name) ) {
      // no variable change required, already exists
      // TODO check if interp_type is already defined?
      xx.fieldSet().field(name).metadata().set("interp_type", "default");
    } else if (name == "sea_area_fraction") {
      // convert integer land mask to a floating point field
      atlas::Field fld = geom_.functionSpace().createField<double>(
        atlas::option::levels(1) |
        atlas::option::name("sea_area_fraction"));
      fld.metadata().set("interp_type", "default");

      auto dst = atlas::array::make_view<double, 2>(fld);
      auto src = atlas::array::make_view<int, 2>(
        geom_.extraFields()->field("gmask"));
      const int size = geom_.functionSpace().size();
      for (int j=0; j < size; j++)
        dst(j, 0) = static_cast<double>(src(j, 0));

      xx.fieldSet().add(fld);
    } else {
      ASSERT(1==2);
    }

  }
}

// -----------------------------------------------------------------------------

}  // namespace umdsst
