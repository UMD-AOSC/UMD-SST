/*
 * (C) Copyright 2019-2020 UCAR.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef OISST_INCREMENT_INCREMENT_H_
#define OISST_INCREMENT_INCREMENT_H_

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "oops/base/GeneralizedDepartures.h"
#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/Printable.h"
#include "oops/util/Serializable.h"

// forward declarations
namespace oops {
  class Variables;
}
namespace ufo {
  class GeoVaLs;
  class Locations;
}
namespace oisst {
  class Geometry;
  class State;
}

// ----------------------------------------------------------------------------

namespace oisst {

  // Increment class
  class Increment : public oops::GeneralizedDepartures,
                    public util::Printable,
                    public util::Serializable,
                    private util::ObjectCounter<Increment> {
   public:
    static const std::string classname() {return "oisst::Increment";}

    // Constructor, destructor
    Increment(const Geometry &, const oops::Variables &,
              const util::DateTime &);
    Increment(const Geometry &, const Increment &);
    Increment(const Increment &, const bool);
    Increment(const Increment &);
    ~Increment();

    // Math operators
    Increment & operator =(const Increment &);
    Increment & operator-=(const Increment &);
    Increment & operator+=(const Increment &);
    Increment & operator*=(const double &);
    void accumul(const double &, const State &);
    void axpy(const double &, const Increment &, const bool check = true);
    void diff(const State &, const State &);
    double dot_product_with(const Increment &) const;
    double norm() const;
    void random();
    void schur_product_with(const Increment &);
    void zero();
    void zero(const util::DateTime &);

    // time manipulation
    void updateTime(const util::Duration & dt) { time_ += dt; }
    const util::DateTime & validTime() const { return time_; }
    util::DateTime & validTime() { return time_; }

    // dirac
    void dirac(const eckit::Configuration &);

    // other accessors
    std::shared_ptr<const Geometry> geometry() const { return geom_; }

    // I/O
    void read(const eckit::Configuration &);
    void write(const eckit::Configuration &) const;

    // Serialize and deserialize (not needed by our project)
    size_t serialSize() const override { return 0; }
    void serialize(std::vector<double> &) const override {}
    void deserialize(const std::vector<double> &, size_t &) override {}

   private:
    void print(std::ostream &) const override;

    std::shared_ptr<const Geometry> geom_;
    util::DateTime time_;
    oops::Variables vars_;
  };
}  // namespace oisst

#endif  // OISST_INCREMENT_INCREMENT_H_
