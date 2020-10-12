/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <numeric>
#include <cmath>
#include <string>

#include <netcdf>

#include "oisst/Geometry/Geometry.h"
#include "oisst/State/State.h"

#include "eckit/config/Configuration.h"

#include "atlas/field.h"

#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"

// using namespace netCDF;


namespace oisst {

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const eckit::Configuration & conf)
    : geom_(new Geometry(geom)), time_(conf.getString("date")),
      vars_(conf, "state variables") {
  //util::abor1_cpp("State::State() needs to be implemented.",
  //                 __FILE__, __LINE__);

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         atlas::field::name(var));
      atlasFieldSet_->add(fld); 
    }
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const oops::Variables & vars,
               const util::DateTime & time)
    : geom_(new Geometry(geom)), time_(time), vars_(vars) {
  //util::abor1_cpp("State::State() needs to be implemented.",
  //                __FILE__, __LINE__);

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         atlas::field::name(var));
      atlasFieldSet_->add(fld); 
    }   
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const State & other)
    : geom_(new Geometry(geom)), time_(other.time_), vars_(other.vars_) {
    // Change state resolution
  //util::abor1_cpp("State::State() needs to be implemented.",
  //                __FILE__, __LINE__);

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      // Ligang: should be copied from "other"?
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         atlas::field::name(var));
      atlasFieldSet_->add(fld); 
    }
  }

// ----------------------------------------------------------------------------

  State::State(const State & other)
    : geom_(new Geometry(*other.geom_)), time_(other.time_),
      vars_(other.vars_) {
  //util::abor1_cpp("State::State() needs to be implemented.",
  //                 __FILE__, __LINE__);

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         atlas::field::name(var));
      atlasFieldSet_->add(fld); 
    }    
  }

// ----------------------------------------------------------------------------

  State::~State() {
  //util::abor1_cpp("State::~State() needs to be implemented.",
  //                __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  State & State::operator+=(const Increment & dx)
  {
  //util::abor1_cpp("State::operator+=(Increment) needs to be implemented.",
  //                __FILE__, __LINE__);

    

    return *this;
  }

// ----------------------------------------------------------------------------

  void State::accumul(const double &zz, const State &rhs) {
  //util::abor1_cpp("State::accumul() needs to be implemented.",
  //                 __FILE__, __LINE__);

    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    auto rhs_field_data = make_view<float, 1>(rhs.atlasFieldSet()->field(0));
    field_data = field_data + zz*rhs_field_data;

    return; 
  }

// ----------------------------------------------------------------------------

  double State::norm() const {
  //util::abor1_cpp("State::norm() needs to be implemented.",
  //                 __FILE__, __LINE__);

    double norm = 0.0, s = 0.0;
    int nx = 0, ny = 0;
 
    // Ligang: just consider 1 var?
    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    ny = static_cast<int>(geom_->atlasFunctionSpace()->grid().ny());
    nx = static_cast<int>(
      ((atlas::RegularLonLatGrid&)(atlasFunctionSpace()->grid())).nx() ); 
    s = std::inner_product(field_data, field_data+ny*nx, field_data, 0);
    norm = sqrt(s/(1.0*ny*nx)); 
 
    return norm;
  }

// ----------------------------------------------------------------------------

  void State::zero() {
  //util::abor1_cpp("State::zero() needs to be implemented.",
  //                __FILE__, __LINE__);

    for (int i = 0; i < vars_.size(); i++) {
    //atlasFieldSet_->field(i).array() = 0.0
      auto field_data = make_view<float, 1>(atlasFieldSet_->field(i));
      field_data = 0.0; // Ligang: Ok? Or do we need for loop?
    }
  }

// ----------------------------------------------------------------------------

  void State::read(const eckit::Configuration & conf) {
  // util::abor1_cpp("State::read() needs to be implemented.",
  //                 __FILE__, __LINE__);

    int iread = 0, lon = 0, lat = 0, bc = 0; 
    int nx, ny;  // ncid, lon_id, lat_id, var_id;
    std::string sdate, filename, record; 

    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));

    if (conf.has("read_from_file"))
      iread = conf.getInt("read_from_file")

    if (iread == 0) {  // Ligang: invent field
      oops::Log::warning() << "State::read: inventing field" << std::endl;
      field_data = 300.0 //Ligang: WARNING, will change to use e.g. climatology
    }
    else { // read field from file
      // get filename
      if (!conf.get("filename", filename))
        util::aborl_cpp("Get filename failed.", __FILE__, __LINE__)
      
      // open netCDF file
      NcFile file(filename.c_str(), NcFile::ReadOnly);
      if (!file.is_valid())
        util::aborl_cpp("Open netCDF file failed.", __FILE__, __LINE__)

      // get file dimensions


    }

  }

// ----------------------------------------------------------------------------

  void State::write(const eckit::Configuration & conf) const {
    util::abor1_cpp("State::write() needs to be implemented.",
                     __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  boost::shared_ptr<const Geometry> State::geometry() const {return geom_;}

// ----------------------------------------------------------------------------

  void State::print(std::ostream & os) const {
    os << "insert diagnostic information about state here "
       << "(min/max/mean for each state var?)"
       << std::endl;

    util::abor1_cpp("State::print() needs to be implemented.",
                    __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
