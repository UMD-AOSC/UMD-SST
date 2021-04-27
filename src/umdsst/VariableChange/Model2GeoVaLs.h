/*
 * (C) Copyright 2021-2021 UCAR, University of Maryland
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UMDSST_VARIABLECHANGE_MODEL2GEOVALS_H_
#define UMDSST_VARIABLECHANGE_MODEL2GEOVALS_H_

#include <memory>
#include <string>

#include "umdsst/Traits.h"

#include "oops/base/VariableChangeBase.h"

namespace umdsst {

class Model2GeoVaLs: public oops::VariableChangeBase<Traits> {
 public:
  static const std::string classname() {return "umdsst::Model2GeoVaLs";}

  Model2GeoVaLs(const Geometry &, const eckit::Configuration &);
  ~Model2GeoVaLs() = default;

  void changeVar(const State &, State &) const override;
  void changeVarInverse(const State &, State &) const override;

 private:
  std::unique_ptr<Geometry> geom_;
  void print(std::ostream &) const override {}
};

}  // namespace umdsst

#endif  // UMDSST_VARIABLECHANGE_MODEL2GEOVALS_H_
