// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/hdf5/NamedEntityHDF5.hpp>

using namespace std;
using namespace nix::base;
using namespace boost;
namespace nix {
namespace hdf5 {


NamedEntityHDF5::NamedEntityHDF5(File file, Group group, const string &id, const string &_type, const string &_name)
    : NamedEntityHDF5(file, group, id, _type, _name, util::getTime())
{
}


NamedEntityHDF5::NamedEntityHDF5(File file, Group group, const string &id, const string &_type, const string &_name, time_t time)
    : EntityHDF5(file, group, id, time)
{
    type(_type);
    name(_name);
}


void NamedEntityHDF5::type(const string &type) {
    if(type.empty()) {
        throw EmptyString("type");
    }
    else {
        group().setAttr("type", type);
        forceUpdatedAt();
    }
}


string NamedEntityHDF5::type() const {
    string type;
    if(group().hasAttr("type")) {
        group().getAttr("type", type);
        return type;
    } else {
        throw MissingAttr("type");
    }
}


void NamedEntityHDF5::name(const string &name) {
    if(name.empty()) {
        throw EmptyString("name");
    }
    else {
        group().setAttr("name", name);
        forceUpdatedAt();
    }
}


string NamedEntityHDF5::name() const {
    string name;
    if(group().hasAttr("name")) {
        group().getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void NamedEntityHDF5::definition(const string &definition) {
    if(definition.empty()) {
        throw EmptyString("definition");
    }
    else {
        group().setAttr("definition", definition);
        forceUpdatedAt();
    }
}


optional<string> NamedEntityHDF5::definition() const {
    optional<string> ret;
    string definition;
    bool have_attr = group().getAttr("definition", definition);
    if (have_attr) {
        ret = definition;
    }
    return ret;
}


void NamedEntityHDF5::definition(const none_t t) {
    if(group().hasAttr("definition")) {
        group().removeAttr("definition");
    }
    forceUpdatedAt();
}


int NamedEntityHDF5::compare(const INamedEntity &other) const {
    int cmp = 0;
    if (!name().empty() && !other.name().empty()) {
        cmp = (name()).compare(other.name());
    }
    if (cmp == 0) {
        cmp = id().compare(other.id());
    }
    return cmp;
}


NamedEntityHDF5::~NamedEntityHDF5() {}


} // namespace hdf5
} // namespace nix
