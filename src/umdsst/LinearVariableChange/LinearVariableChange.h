/*
 * (C) Copyright 2022-2022 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include "oops/base/LinearVariableChangeParametersBase.h"
#include "oops/util/Printable.h"

// forward declartions
namespace oops {
  class Variables;
}
namespace umdsst {
  // forward declarations
  class State;
  class Geometry;
  class Increment;
}

// -----------------------------------------------------------------------------

namespace umdsst {

// -----------------------------------------------------------------------------

class LinearVariableChangeParameters : public oops::LinearVariableChangeParametersBase {
  OOPS_CONCRETE_PARAMETERS(LinearVariableChangeParameters,
                           oops::LinearVariableChangeParametersBase)
};

// -----------------------------------------------------------------------------

class LinearVariableChange : public util::Printable {
 public:
  static const std::string classname() {return "umdsst::LinearVariableChange";}
  typedef LinearVariableChangeParameters Parameters_;

  explicit LinearVariableChange(const Geometry &, const Parameters_ &) {};
  ~LinearVariableChange() {};

  void setTrajectory(const State &, const State &) {}

  void multiply(Increment &, const oops::Variables &) const {}
  void multiplyInverse(Increment &, const oops::Variables &) const {}
  void multiplyAD(Increment &, const oops::Variables &) const {}
  void multiplyInverseAD(Increment &, const oops::Variables &) const {}

 private:
  void print(std::ostream &) const override { ASSERT(1==2); }

};

}  // namespace umdsst