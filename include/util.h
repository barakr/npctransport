/**
# * \file util.h
# * \brief utility methods for npctransport

Simulate an fg and a kap interacting
#
# * Copyright 2007-2019 IMP Inventors. All rights reserved.
# */

#ifndef IMPNPCTRANSPORT_UTIL_H
#define IMPNPCTRANSPORT_UTIL_H

#include "npctransport_config.h"
#include <IMP/npctransport/npctransport_proto.fwd.h>
#include <IMP/npctransport/internal/util.h>
#include <IMP/core/Typed.h>
#include <IMP/core/rigid_bodies.h>
#include <IMP/algebra/Vector3D.h>
#include <IMP/algebra/ReferenceFrame3D.h>
#include <IMP/algebra/Transformation3D.h>
#include "typedefs.h"
#include <IMP/base_types.h>
#include <algorithm>
#include <string>
//#include <boost/range/algorithm_ext/is_sorted.hpp>
//#include <boost/algorithm/cxx11/is_sorted.hpp>
IMPNPCTRANSPORT_BEGIN_NAMESPACE

class SimulationData;

/** returns particles with optimizable coordinates from particles */
IMPNPCTRANSPORTEXPORT
ParticlesTemp get_optimizable_particles
(ParticlesTemp const& particles);

/** returns particles with non-optimizable coordinates from particles */
IMPNPCTRANSPORTEXPORT
ParticlesTemp get_non_optimizable_particles
(ParticlesTemp const& particles);

/** returns particle indexes from a list of particles */
IMPNPCTRANSPORTEXPORT
ParticleIndexes get_particle_indexes
(ParticlesTemp const& particles);

/**
   Converts protobuf configuration file config_txt (which is in pretty
   protobuf textual output format) to binary protobuf format
   (in file config_pb)

   @param config_txt the input textual protobuf config file
   @param config_pb the output binary protobuf file
*/
IMPNPCTRANSPORTEXPORT void get_protobuf_configuration_from_text
(std::string config_txt, std::string config_pb);


#ifndef SWIG

/** finds the index of s.fgs() whose type equals pt.get_string()
    if it does not exist, add it to s
    @param s the statistics message for searching the fg
    @param pt the type of fg to look for
*/
IMPNPCTRANSPORTEXPORT
unsigned int find_or_add_fg_chain_of_type(::npctransport_proto::Statistics* s,
                                    IMP::core::ParticleType pt);

/** finds the index of s.fg_beads() whose type equals pt.get_string()
    if it does not exist, add it to s
    @param s the statistics message for searching the fg
    @param pt the type of fg to look for
*/
IMPNPCTRANSPORTEXPORT
unsigned int find_or_add_fg_bead_of_type(::npctransport_proto::Statistics* s,
                                         IMP::core::ParticleType pt);


/** finds the index of s->floaters() whose type equals pt.get_string()
    if it does not exist, add it to s
    @param s the statistics message for searching t
    @param pt the type to look for
*/
IMPNPCTRANSPORTEXPORT
unsigned int find_or_add_floater_of_type(::npctransport_proto::Statistics* s,
                                         IMP::core::ParticleType pt);


/** finds the index of s.interactions() whose type0 and type1 particle
  type equivalents are equale to it. If it does not exist, add it to s
  @param s the statistics message for searching t
  @param it the type to look for
  @return the index of the interaction type in s.interaction()
*/
IMPNPCTRANSPORTEXPORT
unsigned int find_or_add_interaction_of_type
( ::npctransport_proto::Statistics* s,
  IMP::npctransport::InteractionType it);

/**
   @param p a rigid body particle
   @param local the vector in local coordinates

   @return the global coordinates of local based on the reference frame of the
           rigid body p
*/
inline algebra::Vector3D get_global_from_local_v3( Particle* p,
                                            const algebra::Vector3D& local);


inline algebra::Vector3D get_global_from_local_v3( Particle* p,
                                              const algebra::Vector3D& local)
{
  IMP_USAGE_CHECK(core::RigidBody::get_is_setup(p),
                  "Particle must be rigid body in order to use"
                    " its ref frame in get_global_from_local_v3");
  core::RigidBody rb(p);
  algebra::ReferenceFrame3D rf = rb.get_reference_frame();
  algebra::Transformation3D t = rf.get_transformation_to();
  return t.get_transformed(local);
}

#endif  // SWIG

#ifndef SWIG


/**
   returns |old/core| and |cur/old|, i.e., the number of
   items lost from old, and the ones gained in cur.

   @note it is assumed that old and cur are ordered iteratable objects
         (e.g. std::set) whose begin() and end() methods qualify as valid inputs
         for std::set_difference
*/
template<class t_ordered_set>
inline boost::tuple<unsigned int, unsigned int>
  get_n_lost_and_gained(t_ordered_set old, t_ordered_set cur)
{
  t_ordered_set lost, gained;
  IMP_USAGE_CHECK
    (//boost::algorithm::is_sorted(old.begin(), old.end()),
     internal::is_sorted(old),
     "get_n_lost_and_gained() is expecting an ordered set only");
  IMP_USAGE_CHECK
    (//boost::algorithm::is_sorted(cur.begin(), cur.end()),
     internal::is_sorted(cur),
     "get_n_lost_and_gained() is expecting an ordered set only");
  std::set_difference(old.begin(), old.end(), cur.begin(), cur.end(),
                      std::inserter(lost, lost.begin()) );
  std::set_difference(cur.begin(), cur.end(), old.begin(), old.end(),
                      std::inserter(gained, gained.begin()) );
  return boost::make_tuple(lost.size(), gained.size());
}
#endif // SWIG

//! Canonize such that v0>=v1 so order doesn't matter
template<class t_value>
inline std::pair<t_value, t_value>
  make_unordered_pair(t_value v0, t_value v1)
{
  return (v0 >= v1) ? std::make_pair(v0,v1) : std::make_pair(v1,v0);
}

#ifndef SWIG
//! convert vectors to spheres of passed radius
template<typename V3iter>
algebra::Sphere3Ds get_spheres_from_vectors
(V3iter first, V3iter  last, double radius)
{
  algebra::Sphere3Ds ret;
  for(V3iter it = first;it != last; it++) {
    algebra::Vector3D v3(*it);
    ret.push_back(algebra::Sphere3D(v3, radius));
  }
  return ret;
}
#endif

//! convert vectors to spheres of passed radius
IMPNPCTRANSPORTEXPORT
algebra::Sphere3Ds get_spheres_from_vectors
(algebra::Vector3Ds const& vs, double radius);

// return the center of each sphere is spheres, in same order
IMPNPCTRANSPORTEXPORT
algebra::Vector3Ds get_spheres_centers
(algebra::Sphere3Ds const & spheres);


//! Copy XYZ coordinates or RigidBody reference frame from src_p to
//! trg_p, if it is decorated with XYZ or RigidBody. Do nothing
//! otherwise.
IMPNPCTRANSPORTEXPORT
void copy_particle_reference_frame_if_applicable( Particle* src_p,
                                                  Particle* trg_p );

//! Copy XYZ coordinates or RigidBody reference frame from src_pi to trg_pi if applicable,
//! and if src_pi and trg_pi are an atom hierarchy, proceed recursively to their children.
//! If so, assumes identical topology of hierarchies for src_pi and trg_pi
IMPNPCTRANSPORTEXPORT
void copy_hierarchy_reference_frame_recursive( Particle* src_p,
                                               Particle* trg_p );

//! copy coordinates of src_sd to trg_sd for FG repeats only
IMPNPCTRANSPORTEXPORT
void copy_FGs_coordinates(SimulationData const* src_sd,
                          SimulationData* trg_sd);

IMPNPCTRANSPORT_END_NAMESPACE


#endif /* IMPNPCTRANSPORT_UTIL_H */
