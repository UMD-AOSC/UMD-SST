/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include <numeric>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

// #include <netcdf>
#include "netcdf"

#include "oisst/Geometry/Geometry.h"
#include "oisst/State/State.h"

#include "eckit/config/Configuration.h"

#include "atlas/field.h"
#include "atlas/array.h"
#include "atlas/option.h"

#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"

// using namespace netCDF;
using atlas::array::make_view;
using atlas::option::name;


namespace oisst {

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const eckit::Configuration & conf)
    : geom_(new Geometry(geom)), time_(conf.getString("date")),
      vars_(conf, "state variables") {

    if (vars_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                       __FILE__, __LINE__);
    }

    // output for debug
    oops::Log::info() << "State::State(): print info for debug." << std::endl;
    oops::Log::warning() << "State::State(): print wn for debug." << std::endl;
    // oops::Log::info() << "State::State(): conf: " << conf.getString("grid")
    //                   << std::endl;
    oops::Log::info() << "State::State(), time_  " << time_.toString()
                      << std::endl;
    // vars_.print(std::cout);

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         name(var));

      atlasFieldSet_->add(fld);
    }

    // Ligang: without this test failed, not automatically initialized to 0.0!
    // Better way to automatically initialized to 0.0?
    zero(); 

    // Ligang: for debug
    size_t size = geom_->atlasFunctionSpace()->size();
    int ny = static_cast<int>(geom_->atlasFunctionSpace()->grid().ny());
    int nx = static_cast<int>(
      ((atlas::RegularLonLatGrid&)(geom_->atlasFunctionSpace()->grid())).nx() );
    const double norm1 = norm(); 
    oops::Log::info() << "State::State(): size = " << size
                      << ", ny = " << ny << ", nx = " << nx << ", norm1 = "
                      << norm1 << std::endl;

    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    std::cout << "State::State(): field_data = " << field_data(0)
                      << std::endl; 
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const oops::Variables & vars,
               const util::DateTime & time)
    : geom_(new Geometry(geom)), time_(time), vars_(vars) {

    if (vars_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                      __FILE__, __LINE__);
    }

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         name(var));
      atlasFieldSet_->add(fld);
    }

    zero();
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const State & other)
    : geom_(new Geometry(geom)), time_(other.time_), vars_(other.vars_) {
    // Change state resolution

    if (vars_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                      __FILE__, __LINE__);
    }

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      // Ligang: should be copied from "other"?
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         name(var));
      atlasFieldSet_->add(fld);
    }

    // Ligang: copy data from object other, better way to initialize?!
    int sz = geom_->atlasFunctionSpace()->size();
    for (int i = 0; i < vars_.size(); i++) {
      auto fd       = make_view<float, 1>(atlasFieldSet_->field(0));
      auto fd_other = make_view<float, 1>(other.atlasFieldSet()->field(0));
      for (int j = 0; j < sz; j++) {  
        fd(j) = fd_other(j);
      } 
    }
  }

// ----------------------------------------------------------------------------

  State::State(const State & other)
    : geom_(new Geometry(*other.geom_)), time_(other.time_),
      vars_(other.vars_) {

    if (vars_.size() != 1) {
      util::abor1_cpp("State::State() needs to be implemented.",
                       __FILE__, __LINE__);
    }

    // output for debug
    oops::Log::info() << "State(&other): print info for debug." << std::endl;
    oops::Log::warning() << "State(&other): print for debug." << std::endl;

    atlasFieldSet_.reset(new atlas::FieldSet());
    for (int i = 0; i < vars_.size(); i++) {
      std::string var = vars_[i];
      atlas::Field fld = geom_->atlasFunctionSpace()->createField<float>(
                         name(var));
      atlasFieldSet_->add(fld);
    }

    // Ligang: copy data from object other
    int sz = geom_->atlasFunctionSpace()->size();
    for (int i = 0; i < vars_.size(); i++) {
      auto fd       = make_view<float, 1>(atlasFieldSet_->field(0));
      auto fd_other = make_view<float, 1>(other.atlasFieldSet()->field(0));
      for (int j = 0; j < sz; j++) {  
        fd(j) = fd_other(j);
      } 
    }
    
  }

// ----------------------------------------------------------------------------

  State::~State() {
  // util::abor1_cpp("State::~State() needs to be implemented.",
  //                __FILE__, __LINE__);
  }

// ----------------------------------------------------------------------------

  State & State::operator+=(const Increment & dx)
  {
  // util::abor1_cpp("State::operator+=(Increment) needs to be implemented.",
  //                __FILE__, __LINE__);

    return *this;
  }

// ----------------------------------------------------------------------------

  void State::accumul(const double &zz, const State &rhs) {
    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));
    auto rhs_field_data = make_view<float, 1>(rhs.atlasFieldSet()->field(0));

    // field_data = field_data + zz*rhs_field_data;
    // Ligang: return size_halo_?
    size_t size = geom_->atlasFunctionSpace()->size();
    oops::Log::info() << "size from State::accumul: " << size << std::endl;
    for (size_t jnode = 0; jnode < size; jnode++) {
      field_data(jnode) = field_data(jnode) + zz*rhs_field_data(jnode);
    }

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
      ((atlas::RegularLonLatGrid&)(geom_->atlasFunctionSpace()->grid())).nx() );
    if (geom_->atlasFunctionSpace()->size() != ny*nx)
      util::abor1_cpp("State::norm() size() != ny*nx.", __FILE__, __LINE__);

    // Ligang: undefined operator "+" for field_data+ny*nx;
    // s = std::inner_product(field_data, field_data+ny*nx, field_data, 0);
    for (int jnode = 0; jnode < ny*nx; jnode++)
      s += field_data(jnode)*field_data(jnode);
    norm = sqrt(s/(1.0*ny*nx));

    return norm;
  }

// ----------------------------------------------------------------------------

  void State::zero() {
    // Ligang: just consider 1 var for now
    // atlasFieldSet_->field(0).array() = 0.0
    auto field_data = make_view<float, 1>(atlasFieldSet_->field(0));

    // field_data = 0.0; // Ligang: undefined operator "="!
    for (int i = 0; i < geom_->atlasFunctionSpace()->size(); i++)
      field_data(i) = 0.0;
  }

// ----------------------------------------------------------------------------

  void State::read(const eckit::Configuration & conf) {
    int iread = 0, time = 0, lon = 0, lat = 0, bc = 0;
    // int nx, ny;  // ncid, lon_id, lat_id, var_id;
    std::string sdate, filename, record;

    auto field_data = make_view<float, 2>(atlasFieldSet_->field(0));

    if (conf.has("read_from_file"))
      iread = conf.getInt("read_from_file");

    if (iread == 0) {  // Ligang: invent field
      oops::Log::warning() << "State::read: inventing field" << std::endl;
      // field_data = 300.0 // Ligang: no operator "=" defined! use for loop;
    } else {  // read field from file
      // get filename
      if (!conf.get("filename", filename))
        util::abor1_cpp("Get filename failed.", __FILE__, __LINE__);

      // Ligang: open netCDF file
      netCDF::NcFile file(filename.c_str(), netCDF::NcFile::read);
      if (file.isNull())
        util::abor1_cpp("Create netCDF file failed.", __FILE__, __LINE__);

      // Ligang: get file dimensions, for checking
      time = static_cast<int>(file.getDim("time").getSize());
      lon  = static_cast<int>(file.getDim("lon").getSize());
      lat  = static_cast<int>(file.getDim("lat").getSize());
      if (time != 1 ||
          lat != static_cast<int>(geom_->atlasFunctionSpace()->grid().ny()) ||
          lon != static_cast<int>((((atlas::RegularLonLatGrid&)  // LC: no &?
                 (geom_->atlasFunctionSpace()->grid()))).nx()) ) {
        util::abor1_cpp("lat!=ny or lon!=nx", __FILE__, __LINE__);
      }

      // get sst data
      netCDF::NcVar sstVar;
      sstVar = file.getVar("sst");
      if (sstVar.isNull())
        util::abor1_cpp("Get sst var failed.", __FILE__, __LINE__);

      // Ligang: do we have to use for-loop?
      // sstVar.getVar(field_data); // doesn't work this way
      float sstData[lat][lon];
      sstVar.getVar(sstData);
      for (int i = 0; i < lat; i++)
        for (int j = 0; j < lon; j++)
          field_data(i, j) = sstData[i][j];
    }
  }

// ----------------------------------------------------------------------------

  void State::write(const eckit::Configuration & conf) const {
    // util::abor1_cpp("State::write() needs to be implemented.",
    //                  __FILE__, __LINE__);

    int lat, lon, time = 1;
    std::string filename;

    // get filename
    if (!conf.get("filename", filename))
      util::abor1_cpp("Get filename failed.", __FILE__, __LINE__);

    // create netCDF file
    netCDF::NcFile file(filename.c_str(), netCDF::NcFile::replace);
    if (file.isNull())
      util::abor1_cpp("Create netCDF file failed.", __FILE__, __LINE__);

    // define dims
    lat = geom_->atlasFunctionSpace()->grid().ny();
    lon = (atlas::RegularLonLatGrid&)(geom_->atlasFunctionSpace()->grid()).nx();

    // unlimited dim if without size parameter, what about the size?
    netCDF::NcDim timeDim = file.addDim("time");
    netCDF::NcDim latDim  = file.addDim("lat" , lat);
    netCDF::NcDim lonDim  = file.addDim("lon" , lon);
    if (timeDim.isNull() || latDim.isNull() || lonDim.isNull())
      util::abor1_cpp("Define dims failed.", __FILE__, __LINE__);

    std::vector<netCDF::NcDim> dims;
    dims.push_back(timeDim);
    dims.push_back(latDim);
    dims.push_back(lonDim);

    // Lignag: define coordinate vars "lat" and "lon"

    // Ligang: define units atts for coordinate vars

    // define data vars
    netCDF::NcVar sstVar = file.addVar(std::string("sst"), netCDF::ncFloat
                                       , dims);

    // Ligang: define units atts for data vars

    // write data to the file
    auto field_data = make_view<float, 2>(atlasFieldSet_->field(0));
    sstVar.putVar(&field_data);  // Ligang: should NOT work this way?!

    oops::Log::info() << "Successfully write data to file!" << std::endl;
  }

// ----------------------------------------------------------------------------

  void State::print(std::ostream & os) const {
    os << "insert diagnostic information about state here "
       << "(min/max/mean for each state var?)"
       << std::endl;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
