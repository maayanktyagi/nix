// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/util/ReferenceList.hpp
 */

#ifndef PAN_REFERENCE_LIST_H_INCLUDED
#define PAN_REFERENCE_LIST_H_INCLUDED

#include <pandora/Group.hpp>
#include <pandora/PandoraEntity.hpp>

namespace pandora {
namespace util {


class ReferenceList {

  static const PSize MIN_CHUNK_SIZE;
  static const PSize MAX_SIZE_1D;

  Group group;
  std::string ds_name;

public:

  ReferenceList(const ReferenceList &other);

  ReferenceList(const Group &group, const std::string &ds_name);

  bool has(const std::string &id) const;

  inline bool has(const PandoraEntity &entity) const;

  std::vector<std::string> get() const;

  void set(const std::vector<std::string> &ids);

  void add(const std::string &id);

  inline void add(const PandoraEntity &entity);

  bool remove(const std::string &id);

  inline bool remove(const PandoraEntity &entity);

  inline size_t count() const;

  bool operator==(const ReferenceList &other) const;

  bool operator!=(const ReferenceList &other) const;

  ReferenceList& operator=(const ReferenceList &other);

  virtual ~ReferenceList();

};

// implementations of inline methods

bool ReferenceList::has(const PandoraEntity &entity) const {
  return has(entity.id());
}

void ReferenceList::add(const PandoraEntity &entity) {
  add(entity.id());
}

bool ReferenceList::remove(const PandoraEntity &entity) {
  return remove(entity.id());
}

size_t ReferenceList::count() const {
  return get().size();
}

} // namespace util
} // namespace pandora

#endif // PAN_REFERENCE_LIST_H_INCLUDED
