// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestNDArray.hpp"

#include <nix/NDArray.hpp>

void TestNDArray::setUp() {
}

void TestNDArray::basic() {
    nix::NDSize dims(2, 5);
    nix::NDArray A(nix::DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            A.set<double>(i*5 + j, values++);

    values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_EQUAL(A.get<double>(i*5 + j), static_cast<double>(values++));

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(5), A.get<double>(nix::NDSize({ 1, 0 })));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(18), A.get<double>(nix::NDSize({ 3, 3 })));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(24), A.get<double>(nix::NDSize({ 4, 4 })));

    dims = nix::NDSize({ 3, 4, 5 });
    nix::NDArray B(nix::DataType::Double, dims);
    values = 0;
    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                B.set<double>(nix::NDSize({ i, j, k }), values++);

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(23), B.get<double>(nix::NDSize({ 1, 0, 3 })));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(42), B.get<double>(nix::NDSize({ 2, 0, 2 })));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(26), B.get<double>(nix::NDSize({ 1, 1, 1 })));

}

void TestNDArray::tearDown() {
}
