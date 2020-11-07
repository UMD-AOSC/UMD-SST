/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OISST_STATE_STATE_H_
#define OISST_STATE_STATE_H_

#include <memory>
#include <ostream>
#include <string>

#include "oisst/Fields/Fields.h"

#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"

// forward declarations
namespace eckit {
  class Configuration;
}
namespace ufo {
  class GeoVaLs;
  class Locations;
}
namespace oisst {
  class Geometry;
  class Increment;
}

// ----------------------------------------------------------------------------

namespace oisst {

  // State class
  class State : public util::Printable,
                private util::ObjectCounter<State>,
                public oisst::Fields {
   public:
    static const std::string classname() {return "oisst::State";}

    // constructors, destructors
    State(const Geometry &, const eckit::Configuration &);
    State(const Geometry &, const oops::Variables &,
          const util::DateTime &);
    State(const Geometry &, const State &);
    State(const State &);
    ~State();

    // wrappers of methods that are fully implemented in Fields
    State & operator+=(const Increment &);


    // I/O
    void read(const eckit::Configuration &);
    void write(const eckit::Configuration &) const;

   private:
     void print(std::ostream &) const;
  };
}  // namespace oisst

#endif  // OISST_STATE_STATE_H_
