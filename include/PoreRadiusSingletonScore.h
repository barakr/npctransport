/**
 *  \file IMP/npctransport/PoreRadiusSingletonScore.h
 *  \brief - a score on the pore radius equilibrium value
 *           of a slab particle
 *
 *  Copyright 2007-2022 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPNPCTRANSPORT_PORE_RADIUS_SINGLETON_SCORE_H
#define IMPNPCTRANSPORT_PORE_RADIUS_SINGLETON_SCORE_H

#include "npctransport_config.h"
#include <IMP/npctransport/SlabWithPore.h>
#include <IMP/core/GenericAttributeSingletonScore.h>
#include <IMP/core/Harmonic.h>
#include <IMP/score_functor/HarmonicLowerBound.h>

IMPNPCTRANSPORT_BEGIN_NAMESPACE

//! a harmonic score on the pore radius of a slab particle
//! /see SlabWithPore
class IMPNPCTRANSPORTEXPORT
PoreRadiusSingletonScore :
public core::GenericAttributeSingletonScore<core::Harmonic>
{
 private:
  typedef core::GenericAttributeSingletonScore<core::Harmonic> P;

 public:
  //! initialize a harmonic score on the pore radius key,
  //! as defined by SlabWithPore::get_pore_radius_key()
 PoreRadiusSingletonScore(Float mean, Float k) :
  P(new core::Harmonic(mean, k),
    SlabWithPore::get_pore_radius_key())
    { }

  virtual ModelObjectsTemp do_get_inputs(Model *m,
                                         const ParticleIndexes &pis) const
    IMP_OVERRIDE;

  //! set the coefficient of the score harmonic to k
  void set_k(double k){
    P::get_unary_function()->set_k(k);
  }

#ifdef SWIG
  IMP_SINGLETON_SCORE_METHODS(PoreRadiusSingletonScore);
  IMP_OBJECT_METHODS(PoreRadiusSingletonScore);
#endif

};

IMPNPCTRANSPORT_END_NAMESPACE

#endif /* IMPNPCTRANSPORT_PORE_RADIUS_SINGLETON_SCORE_H */
