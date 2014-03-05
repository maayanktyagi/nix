// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestSection.hpp"

using namespace std;
using namespace nix;


void TestSection::setUp() {
    startup_time = time(NULL);
    file = File::open("test_section.h5", FileMode::Overwrite);

    section = file.createSection("section", "metadata");
    section_other = file.createSection("other_section", "metadata");
    section_null  = nullptr;
}

void TestSection::tearDown() {
    file.close();
}

void TestSection::testId() {
    CPPUNIT_ASSERT(section.id().size() == 24);
}


void TestSection::testName() {
    CPPUNIT_ASSERT(*section.name() == "section");
    string name = util::createId("", 32);
    section.name(name);
    CPPUNIT_ASSERT(*section.name() == name);
}


void TestSection::testType() {
    CPPUNIT_ASSERT(section.type() == "metadata");
    string typ = util::createId("", 32);
    section.type(typ);
    CPPUNIT_ASSERT(section.type() == typ);
}


void TestSection::testDefinition() {
    string def = util::createId("", 128);
    section.definition(def);
    CPPUNIT_ASSERT(*section.definition() == def);
}


void TestSection::testParent() {
    CPPUNIT_ASSERT(section.parent() == nullptr);

    Section child = section.createSection("child", "section");
    CPPUNIT_ASSERT(child.parent() != nullptr);
    CPPUNIT_ASSERT(child.parent().id() == section.id());

    CPPUNIT_ASSERT(child.parent().parent() == nullptr);
}


void TestSection::testRepository() {
    CPPUNIT_ASSERT(!section.repository());
    string rep = "http://foo.bar/" + util::createId("", 32);
    section.repository(rep);
    CPPUNIT_ASSERT(section.repository() == rep);
}


void TestSection::testLink() {
    CPPUNIT_ASSERT(section.link() == nullptr);

    section.link(section_other);
    CPPUNIT_ASSERT(section.link() != nullptr);
    CPPUNIT_ASSERT(section.link().id() == section_other.id());

    section.link(nullptr);
    CPPUNIT_ASSERT(section.link() == nullptr);
}


void TestSection::testMapping() {
    CPPUNIT_ASSERT(!section.mapping());
    string map = "http://foo.bar/" + util::createId("", 32);
    section.mapping(map);
    CPPUNIT_ASSERT(section.mapping() == map);
}


void TestSection::testSectionAccess() {
    vector<string> names = { "section_a", "section_b", "section_c", "section_d", "section_e" };

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section child_section = section.createSection(*it, "metadata");
        CPPUNIT_ASSERT(child_section.name() == *it);

        ids.push_back(child_section.id());
    }


    CPPUNIT_ASSERT(section.sectionCount() == names.size());
    CPPUNIT_ASSERT(section.sections().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section child_section = section.getSection(*it);
        CPPUNIT_ASSERT(section.hasSection(*it) == true);
        CPPUNIT_ASSERT(child_section.id() == *it);

        section.deleteSection(*it);
    }

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
}


void TestSection::testFindSection() {
    // prepare
    Section l1n1 = section.createSection("l1n1", "typ1");
    Section l1n2 = section.createSection("l1n2", "typ2");
    Section l1n3 = section.createSection("l1n3", "typ3");

    Section l2n1 = l1n1.createSection("l2n1", "typ1");
    Section l2n2 = l1n1.createSection("l2n2", "typ2");
    Section l2n3 = l1n1.createSection("l2n3", "typ2");
    Section l2n4 = l1n3.createSection("l2n4", "typ2");
    Section l2n5 = l1n3.createSection("l2n5", "typ2");
    Section l2n6 = l1n3.createSection("l2n6", "typ3");

    Section l3n1 = l2n1.createSection("l2n3", "typ1");
    Section l3n2 = l2n3.createSection("l2n3", "typ2");
    Section l3n3 = l2n3.createSection("l2n3", "typ2");
    Section l3n4 = l2n5.createSection("l2n3", "typ2");

    // test depth limit
    CPPUNIT_ASSERT(section.findSections().size() == 14);
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 2).size() == 10);
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 1).size() == 4);
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 0).size() == 1);

    // test filter
    auto filter_typ1 = util::TypeFilter<Section>("typ1");
    auto filter_typ2 = util::TypeFilter<Section>("typ2");

    CPPUNIT_ASSERT(section.findSections(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(section.findSections(filter_typ2).size() == 8);
}

void TestSection::testFindRelated(){
    Section l1n1 = section.createSection("l1n1", "typ1");

    Section l2n1 = l1n1.createSection("l2n1", "t1");
    Section l2n2 = l1n1.createSection("l2n2", "t2");

    Section l3n1 = l2n1.createSection("l3n1", "t3");
    Section l3n2 = l2n2.createSection("l3n2", "t3");
    Section l3n3 = l2n2.createSection("l3n3", "t4");

    Section l4n1 = l3n2.createSection("l4n1", "typ2");
    Section l4n2 = l3n3.createSection("l4n2", "typ2");

    Section l5n1 = l4n1.createSection("l5n1", "typ2");

    string t1 = "t1";
    string t3 = "t3";
    string t4 = "t4";
    string typ2 = "typ2";
    string typ1 = "typ1";

    vector<Section> related = l1n1.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);

    related = l1n1.findRelated(util::TypeFilter<Section>(t3));
    CPPUNIT_ASSERT(related.size() == 2);

    related = l1n1.findRelated(util::TypeFilter<Section>(t4));
    CPPUNIT_ASSERT(related.size() == 1);

    related = l1n1.findRelated(util::TypeFilter<Section>(typ2));
    CPPUNIT_ASSERT(related.size() == 2);

    related = l4n1.findRelated(util::TypeFilter<Section>(typ1));
    CPPUNIT_ASSERT(related.size() == 1);

    related = l4n1.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);

    related = l3n2.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);

    related = l3n2.findRelated(util::TypeFilter<Section>(t3));
    CPPUNIT_ASSERT(related.size() == 0);
    section.deleteSection(l1n1.id());
}


void TestSection::testPropertyAccess() {
    vector<string> names = { "property_a", "property_b", "property_c", "property_d", "property_e" };

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Property prop = section.createProperty(*it);
        CPPUNIT_ASSERT(prop.name() == *it);
        CPPUNIT_ASSERT(section.hasPropertyWithName(*it));

        Property prop_copy = section.getPropertyByName(*it);

        CPPUNIT_ASSERT(prop_copy.id() == prop.id());

        ids.push_back(prop.id());
    }

    CPPUNIT_ASSERT(section.propertyCount() == names.size());
    CPPUNIT_ASSERT(section.properties().size() == names.size());

    section_other.createProperty("some_prop");
    section_other.link(section);
    CPPUNIT_ASSERT(section_other.propertyCount() ==1);
    CPPUNIT_ASSERT(section_other.inheritedProperties().size() == names.size() + 1);

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Property prop = section.getProperty(*it);
        CPPUNIT_ASSERT(section.hasProperty(*it));
        CPPUNIT_ASSERT(prop.id() == *it);

        section.deleteProperty(*it);
    }

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);
}


void TestSection::testOperators() {
    CPPUNIT_ASSERT(section_null == NULL);
    CPPUNIT_ASSERT(section_null == nullptr);

    CPPUNIT_ASSERT(section != NULL);
    CPPUNIT_ASSERT(section != nullptr);

    CPPUNIT_ASSERT(section == section);
    CPPUNIT_ASSERT(section != section_other);

    section_other = section;
    CPPUNIT_ASSERT(section == section_other);

    section_other = nullptr;
    CPPUNIT_ASSERT(section_null == NULL);
    CPPUNIT_ASSERT(section_null == nullptr);
}


void TestSection::testCreatedAt() {
    CPPUNIT_ASSERT(section.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    section.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(section.createdAt() == past_time);
}


void TestSection::testUpdatedAt() {
    CPPUNIT_ASSERT(section.updatedAt() >= startup_time);
}