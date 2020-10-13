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

    if (var_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                       __FILE__, __LINE__);
    }

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

    if (var_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                      __FILE__, __LINE__);
    }

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

    if (var_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                      __FILE__, __LINE__);
    }

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

    if (var_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                       __FILE__, __LINE__);
    }

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
    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    auto rhs_field_data = make_view<float, 1>(rhs.atlasFieldSet()->field(0));
    field_data = field_data + zz*rhs_field_data;

    return; 
  }

// ----------------------------------------------------------------------------

  double State::norm() const {
    double norm = 0.0, s = 0.0;
    int nx = 0, ny = 0;
 
    // Ligang: just consider 1 var for now.
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
    // Ligang: just consider 1 var for now
    //atlasFieldSet_->field(0).array() = 0.0
    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    field_data = 0.0;
  }

// ----------------------------------------------------------------------------

  void State::read(const eckit::Configuration & conf) {
    int iread = 0, time = 0, lon = 0, lat = 0, bc = 0; 
  //int nx, ny;  // ncid, lon_id, lat_id, var_id;
    std::string sdate, filename, record; 

    auto field_data = make_view<float, 2>(atlasFieldSet_->field(0));

    if (conf.has("read_from_file"))
      iread = conf.getInt("read_from_file");

    if (iread == 0) {  // Ligang: invent field
      oops::Log::warning() << "State::read: inventing field" << std::endl;
      field_data = 300.0 //Ligang: WARNING, will change to use e.g. climatology
    }
    else { // read field from file
      // get filename
      if (!conf.get("filename", filename))
        util::aborl_cpp("Get filename failed.", __FILE__, __LINE__);
      
      // open netCDF file
      NcFile file(filename.c_str(), NcFile::ReadOnly);
      if (!file.is_valid())
        util::aborl_cpp("Open netCDF file failed.", __FILE__, __LINE__);

      // Ligang: get file dimensions, for checking
      time = static_cast<int>(file.get_dim("time")->size());
      lon  = static_cast<int>(file.get_dim("lon" )->size());
      lat  = static_cast<int>(file.get_dim("lat" )->size());
      if (time != 1 ||
          lat != static_cast<int>(geom_->atlasFunctionSpace()->grid().ny()) || 
          lon != static_cast<int>(((atlas::RegularLonLatGrid&)
                 (atlasFunctionSpace()->grid()))).nx() ) {
        util::aborl_cpp("lat!=ny or lon!=nx", __FILE__, __LINE__);
      }

      // get sst data 
      NcVar *sstVar;
      if (!(sstVar = file.get_var("sst"))) 
        util::aborl_cpp("Get sst var failed.", __FILE__, __LINE__);
      // Ligang: not sure if ok? or first read in to a var then reassign?
      if (!sstVar->get(&field_data, 1, lat, lon))
        util::aborl_cpp("Get sst data failed.", __FILE__, __LINE__);
    }

  }

// ----------------------------------------------------------------------------

  void State::write(const eckit::Configuration & conf) const {
  //util::abor1_cpp("State::write() needs to be implemented.",
  //                 __FILE__, __LINE__);

    int lat, lon, time = 1;
    std::string filename;

    // get filename
    if (!conf.get("filename", filename))
      util::aborl_cpp("Get filename failed.", __FILE__, __LINE__);
    
    // create netCDF file
    NcFile file(filename.c_str(), NcFile::Replace);
    if (!file.is_valid())
      util::aborl_cpp("Create netCDF file failed.", __FILE__, __LINE__);

    // define dims
    NcDim *timeDim, *latDim, *lonDim;
    lat     = geom_->atlasFunctionSpace()->grid().ny()
    lon     = (atlas::RegularLonLatGrid&)(geom_->atlasFunctionSpace()->grid()).nx()
    timeDim = file.add_dim("time");  //unlimited dim if without size parameter;
    latDim  = file.add_dim("lat" , lat);
    lonDim  = file.add_dim("lon" , lon);
    if (!timeDim || !latDim || !lonDim)
      util::aborl_cpp("Define dims failed.", __FILE__, __LINE__);

    // define coordinate vars "lat" and "lon"
    

    // define units atts for coordinate vars


    // defien data vars
    
  
    // define units atts for data vars


  }

// ----------------------------------------------------------------------------

  boost::shared_ptr<const Geometry> State::geometry() const {return geom_;}

// ----------------------------------------------------------------------------

  void State::print(std::ostream & os) const {
    os << "insert diagnostic information about state here "
       << "(min/max/mean for each state var?)"
       << std::endl;

  //util::abor1_cpp("State::print() needs to be implemented.",
  //                __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
