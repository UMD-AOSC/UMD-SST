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
#include <iterator>

#include <limits>
#include <iomanip>

// #include <netcdf>
#include "netcdf"

#include "oisst/Geometry/Geometry.h"
#include "oisst/State/State.h"
#include "oisst/Increment/Increment.h"

#include "eckit/config/Configuration.h"

#include "atlas/field.h"
#include "atlas/array.h"
#include "atlas/option.h"

#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"
#include "oops/util/missingValues.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"

// using namespace netCDF;
using atlas::array::make_view;
using atlas::option::name;


namespace oisst {

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const eckit::Configuration & conf)
    : Fields(geom, oops::Variables(conf, "state variables"),
             util::DateTime(conf.getString("date"))) {
    // check and read in data if it has filename
    if (conf.has("filename"))
      read(conf);
  }

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const oops::Variables & vars,
               const util::DateTime & time)
    : Fields(geom, vars, time) {}

// ----------------------------------------------------------------------------

  State::State(const Geometry & geom, const State & other)
    : State(other) {
    // Change state resolution, normally used for interpolation.
  }

// ----------------------------------------------------------------------------

  State::State(const State & other)
    : Fields(other) {}

// ----------------------------------------------------------------------------

  State::~State() {}

// ----------------------------------------------------------------------------

  State & State::operator+=(const Increment & dx) {
    Fields::operator+=(dx);
    return *this;
  }

// ----------------------------------------------------------------------------

  void State::read(const eckit::Configuration & conf) {
    int iread = 0, time = 0, lon = 0, lat = 0, bc = 0;
    std::string sdate, filename, record;

    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));
    const int size = geom_->atlasFunctionSpace()->size();

    // Ligang: conf only has "statefile" part of the yml file.
    // Here we do not actually use iread
    if (conf.has("read_from_file")) {
      iread = conf.getInt("read_from_file");
    } else {
      // you can output keys to check
      std::vector<std::string> keys = conf.keys();
    }

    // get filename
    if (!conf.get("filename", filename))
      util::abor1_cpp("Get filename failed.", __FILE__, __LINE__);

    std::cout << "State::read(), filename = " << filename << std::endl;


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
//  double sstData[lat][lon];
    float  sstData[lat][lon];
    sstVar.getVar(sstData);  // if used double, read-in data would be wrong.

    const double epsilon = 1.0e-6;
    const double missing = util::missingValue(missing);
    const double missing_nc = -32768.0;
    for (int j = 0; j < lat; j++)
      for (int i = 0; i < lon; i++)
        if (abs(sstData[j][i]-(missing_nc)) < epsilon)
          sstData[j][i] = missing;

    int idx = 0;
    for (int j = 0; j < lat; j++)
      for (int i = 0; i < lon; i++)
        fd(idx++) = static_cast<double>(sstData[j][i]);  // float to double.
  }

// ----------------------------------------------------------------------------

  void State::write(const eckit::Configuration & conf) const {
    int lat, lon, time = 1;
    std::string filename;

    // get filename
    if (!conf.get("filename", filename))
      util::abor1_cpp("Get filename failed.", __FILE__, __LINE__);
    else
      oops::Log::info() << "State::write(), filename=" << filename << std::endl;

    // create netCDF file
    netCDF::NcFile file(filename.c_str(), netCDF::NcFile::replace);
    if (file.isNull())
      util::abor1_cpp("Create netCDF file failed.", __FILE__, __LINE__);

    // define dims
    lat = geom_->atlasFunctionSpace()->grid().ny();
    lon =((atlas::RegularLonLatGrid)(geom_->atlasFunctionSpace()->grid())).nx();

    // unlimited dim if without size parameter, then it'll be 0,
    // what about the size?
    netCDF::NcDim timeDim = file.addDim("time", 1);
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
    // if ncDouble, it'll be run-time error.
    netCDF::NcVar sstVar = file.addVar(std::string("sst"), netCDF::ncFloat
                                       , dims);

    // Ligang: define units atts for data vars
    // Ligang: inside OISSTv3/JEDI, use double, read/write use float
    //         to make it consistent with the netCDF files.
//  const double fillvalue = -32768.0;
    const float fillvalue = -32768.0;
    sstVar.putAtt("units", "K");
    sstVar.putAtt("_FillValue", netCDF::NcFloat(), fillvalue);
    sstVar.putAtt("missing_value", netCDF::NcFloat(), fillvalue);

    // write data to the file
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));

    const double missing = util::missingValue(missing);

    float sstData[time][lat][lon];
    int idx = 0;
    for (int j = 0; j < lat; j++)
      for (int i = 0; i < lon; i++) {
        if (fd(idx) == missing)
          sstData[0][j][i] = fillvalue;
        else
          sstData[0][j][i] = static_cast<float>(fd(idx));

        idx++;
      }

    sstVar.putVar(sstData);

    oops::Log::info() << "State::write(), Successfully write data to file!"
                      << std::endl;
  }

// ----------------------------------------------------------------------------

  void State::print(std::ostream & os) const {
    auto fd = make_view<double, 1>(atlasFieldSet_->field(0));
    const int size = geom_->atlasFunctionSpace()->size();
    double missing = util::missingValue(missing);

    double mean = 0.0, sum = 0.0,
           min = std::numeric_limits<double>::max(),
           max = std::numeric_limits<double>::min();
    int nValid = 0;

    for (int i = 0; i < size; i++)
      if (fd(i) != missing) {
        if (fd(i) < min) min = fd(i);
        if (fd(i) > max) max = fd(i);

        sum += fd(i);
        nValid++;
      }

    if (nValid == 0) {
      mean = 0.0;
      oops::Log::debug() << "State::print(), nValid == 0!" << std::endl;
    } else {
      mean = sum / (1.0*nValid);
    }

    os << "min = " << min << ", max = " << max << ", mean = " << mean
       << std::endl;
  }

// ----------------------------------------------------------------------------

}  // namespace oisst
