/*
 * (C) Copyright 2019-2019 UCAR.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "umdsst/Traits.h"
#include "oops/runs/Run.h"
#include "test/interface/ErrorCovariance.h"

int main(int argc,  char ** argv) {
  oops::Run run(argc, argv);
  test::ErrorCovariance<umdsst::Traits> tests;
  return run.execute(tests);
}

