/*
 * (C) Copyright 2020-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UMDSST_GETVALUES_GEOVALSWRAPPER_H_
#define UMDSST_GETVALUES_GEOVALSWRAPPER_H_

#include "ufo/GeoVaLs.h"

namespace umdsst {
  class GeoVaLsWrapper {
   public:
    explicit GeoVaLsWrapper(const ufo::GeoVaLs & geovals)
     : geovals_(geovals)
    {}

   private:
     ufo::GeoVaLs geovals_;
  };
}  // namespace umdsst

#endif  // UMDSST_GETVALUES_GEOVALSWRAPPER_H_
