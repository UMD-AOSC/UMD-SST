/*
 * (C) Copyright 2021-2022 UCAR, University of Maryland
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "umdsst/State/State.h"
#include "umdsst/VariableChange/VariableChange.h"

#include "oops/util/abor1_cpp.h"

namespace umdsst {

// -----------------------------------------------------------------------------

VariableChange::VariableChange(const Parameters_ & params,
                               const Geometry & geom)
    : geom_(geom) {
  ASSERT(1==2);
}

// -----------------------------------------------------------------------------

void VariableChange::changeVar(State &, const oops::Variables &) const {
  ASSERT(1==2);
}

// -----------------------------------------------------------------------------

}  // namespace umdsst
