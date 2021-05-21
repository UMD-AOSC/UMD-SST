/*
 * (C) Copyright 2020-2020 UCAR.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "umdsst/Traits.h"
#include "oops/runs/Run.h"
#include "test/interface/GetValues.h"
#include "ufo/ObsTraits.h"

int main(int argc,  char ** argv) {
  oops::Run run(argc, argv);
  test::GetValues<umdsst::Traits, ufo::ObsTraits> tests;
  return run.execute(tests);
}