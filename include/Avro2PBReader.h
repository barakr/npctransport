/**
 *  \file Avro2PBReader.h
 *  \brief an iterator that reads npctransport protobuf output files one by one
 *         from a list of avro files (used mainly as a wrapper for python usage)
 *
 *  Copyright 2007-2022 IMP Inventors. All rights reserved.
 */

#ifndef IMPNPCTRANSPORT_AVRO2PB_READER_H
#define IMPNPCTRANSPORT_AVRO2PB_READER_H

#include <IMP/npctransport/avro.h>
#include <IMP/npctransport/AvroDataFileData.h>
#include <IMP/value_macros.h>
#include <IMP/types.h>
#include <IMP/showable_macros.h>

#ifndef SWIG
IMP_GCC_PUSH_POP(diagnostic push)
IMP_GCC_PRAGMA(diagnostic ignored "-Wextra")
#include <DataFile.hh>
IMP_GCC_PUSH_POP(diagnostic pop)
#endif

#include <fstream>
#include <iomanip>

IMPNPCTRANSPORT_BEGIN_NAMESPACE

class IMPNPCTRANSPORTEXPORT Avro2PBReader {
 private:
  typedef IMP_NPCTRANSPORT_AVRO_NAMESPACE::DataFileReader<IMP_npctransport::wrapper> t_avro_reader;

 public:
  /** Initiates a reader that goes over all output
      entries in all files specified in avro_filenames
  */
  Avro2PBReader(const Strings& avro_filenames);

  /** Initiates a reader that goes over all output
      entries in all files specified in avro_filenames
  */
  Avro2PBReader(std::string avro_filename);

  /** closes any open files */
  ~Avro2PBReader();

#if defined(SWIG) || defined(IMP_SWIG_WRAPPER)
  typedef std::string ByteBuffer;
  ByteBuffer read_next();
#else
  /**
     Read the next output entry into output and returns it
     as string. If no input is left, returns "" and invalidates
     this object.
  */
  std::string read_next();
#endif

  //! returns true if there are still files to go over
  //! (though possibly no entries left in neither of them)
  bool get_is_valid();

  //! returns the name of the file that is currently being read
  //! or "" if reader is at invalid state
  std::string get_cur_file_name() {
    if (!get_is_valid()) return "";
    return avro_filenames_[cur_file_];
  }

 private:
  //! close any open file if one exists and move cursor to next file index
  void advance_current_reader();

  // called from ctr, this pain is needed since constructor delegation
  // is only supported from g++ 4.7, so we use init() for backward compatibility
  void init(const Strings& avro_filenames);

 private:
  Strings avro_filenames_;  // list of files to go over
  t_avro_reader* avro_reader_;
  unsigned int cur_file_;  // file index we're reading now

 public:
  IMP_SHOWABLE_INLINE(Avro2PBReader,
                      out << "Avro2PBReader with " << avro_filenames_.size()
                          << " input avro files");
};

IMP_VALUES(Avro2PBReader, Avro2PBReaders);

IMPNPCTRANSPORT_END_NAMESPACE

#endif /* IMPNPCTRANSPORT_AVRO2PB_READER_H */
