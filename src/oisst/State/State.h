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

#include <boost/shared_ptr.hpp>

#include "eckit/mpi/Comm.h"

#include "atlas/field.h"

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
                private util::ObjectCounter<State> {
   public:
    static const std::string classname() {return "oisst::State";}

    // constructors, destructors
    State(const Geometry &, const eckit::Configuration &);
    State(const Geometry &, const oops::Variables &,
          const util::DateTime &);
    State(const Geometry &, const State &);
    State(const State &);
    ~State();

    // math operators
    State & operator+=(const Increment &);
    void accumul(const double &, const State &);
    double norm() const;
    void zero();

    // I/O
    void read(const eckit::Configuration &);
    void write(const eckit::Configuration &) const;

    // time manipulation
    void updateTime(const util::Duration & dt) { time_ += dt; }
    const util::DateTime & validTime() const { return time_; }
    util::DateTime & validTime() { return time_; }

    // other accessors
    std::shared_ptr<const Geometry> geometry() const {return geom_;}
    const oops::Variables & variables() const { return vars_; }

    std::shared_ptr<atlas::FieldSet> atlasFieldSet() const {
      return atlasFieldSet_;
    }

    // Serialize and deserialize
    size_t serialSize() const { return 0; };
    void serialize(std::vector<double> &) const  { };
    void deserialize(const std::vector<double> &, size_t &) { };

   private:
    void print(std::ostream &) const;

  //boost::shared_ptr<const Geometry> geom_;
    std::shared_ptr<const Geometry> geom_;
    oops::Variables vars_;
    util::DateTime time_;

    std::shared_ptr<atlas::FieldSet> atlasFieldSet_;
  };
}  // namespace oisst

#endif  // OISST_STATE_STATE_H_
