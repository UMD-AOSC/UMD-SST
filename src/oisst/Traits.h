/*
 * (C) Copyright 2019-2020 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OISST_TRAITS_H_
#define OISST_TRAITS_H_

#include <string>

#include "oisst/Covariance/Covariance.h"
#include "oisst/Geometry/Geometry.h"
#include "oisst/GetValues/GetValues.h"
#include "oisst/GetValues/LinearGetValues.h"
#include "oisst/Increment/Increment.h"
#include "oisst/ModelAux/ModelAuxControl.h"
#include "oisst/ModelAux/ModelAuxCovariance.h"
#include "oisst/ModelAux/ModelAuxIncrement.h"
#include "oisst/State/State.h"

namespace oisst {

  struct Traits{
    static std::string name() {return "oisst";}
    static std::string nameCovar() {return "oisstCovar";}
    static std::string nameCovar4D() {return "oisstCovar";}

    // Interfaces that oisst has to implement
    // ---------------------------------------------------
    typedef oisst::Covariance          Covariance;
    typedef oisst::Geometry            Geometry;
    typedef oisst::GetValues           GetValues;
    typedef oisst::Increment           Increment;
    typedef oisst::LinearGetValues     LinearGetValues;
    typedef oisst::ModelAuxControl     ModelAuxControl;
    typedef oisst::ModelAuxCovariance  ModelAuxCovariance;
    typedef oisst::ModelAuxIncrement   ModelAuxIncrement;
    typedef oisst::State               State;
  };
}  // namespace oisst

#endif  // OISST_TRAITS_H_
