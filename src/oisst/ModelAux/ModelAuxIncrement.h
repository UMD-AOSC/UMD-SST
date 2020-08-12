/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OISST_MODELAUX_MODELAUXINCREMENT_H_
#define OISST_MODELAUX_MODELAUXINCREMENT_H_

#include <ostream>
#include <string>

#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"

// forward declarations
namespace eckit {
  class Configuration;
}
namespace oisst {
  class Geometry;
  class ModelAuxControl;
}

//-----------------------------------------------------------------------------

namespace oisst {

  // ModelAuxControl class
  class ModelAuxIncrement : public util::Printable,
                            private util::ObjectCounter<ModelAuxIncrement> {
   public:
    static const std::string classname() {return "oisst::ModelAuxIncrement";}

    // constructors / destructor
    ModelAuxIncrement(const ModelAuxIncrement &, const eckit::Configuration &);
    ModelAuxIncrement(const ModelAuxIncrement &, const bool);
    ModelAuxIncrement(const Geometry &, const eckit::Configuration &);
    ~ModelAuxIncrement();

    // math operators
    ModelAuxIncrement & operator*=(const double);
    ModelAuxIncrement & operator+=(const ModelAuxIncrement &);
    ModelAuxIncrement & operator-=(const ModelAuxIncrement &);
    void axpy(const double, const ModelAuxIncrement &);
    void diff(const ModelAuxControl &, const ModelAuxControl &);
    double dot_product_with(const ModelAuxIncrement &) const;
    double norm() const;
    void zero();

   private:
    void print(std::ostream &) const;
  };
}  // namespace oisst
#endif  // OISST_MODELAUX_MODELAUXINCREMENT_H_
