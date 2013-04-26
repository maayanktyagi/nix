#include <iostream>
#include <hdf5.h>
#include <pandora/DataSet.hpp>
#include <pandora/Selection.hpp>

#include <cmath>

namespace pandora {

DataSet::DataSet(H5::DataSet dset)
	: h5dset(dset)
{
}


DataSet DataSet::create(const H5::CommonFG &parent,
	const std::string &name,
	DataType dtype,
	const PSize &size,
	const PSize *maxsize,
	const PSize *chunks)
{
	H5::DataType fileType = data_type_to_h5_filetype(dtype);
	H5::DataSpace space;

	if (size.size() > 0) {
		int rank = static_cast<int>(size.size());
		const hsize_t *maxdims = maxsize != nullptr ? &(*maxsize)[0] : nullptr;
		space = H5::DataSpace(rank, &size[0], maxdims);
	}

	H5::DSetCreatPropList plcreate = H5::DSetCreatPropList::DEFAULT;

	if (chunks != nullptr) {
		int rank = static_cast<int>(chunks->size());
		plcreate.setChunk(rank, &(*chunks)[0]);
	}

	H5::DataSet dset = parent.createDataSet(name, fileType, space);
	return DataSet(dset);
}

double psize_product(const PSize &dims)
{
	double product = 1;
	std::for_each(dims.begin(), dims.end(), [&](hsize_t val) {
		product *= val;
	});

	return product;
}


#define CHUNK_BASE   16*1024
#define CHUNK_MIN     8*1024
#define CHUNK_MAX  1024*1024

/**
 * Infer the chunk size from the supplied size information
 *
 * @param dims    Size information to base the guessing on
 * @param dtype   The type of the data to guess the chunks for
 *
 * This function is a port of the guess_chunk() function from h5py
 * low-level Python interface to the HDF5 library.\n
 * http://h5py.alfven.org\n
 * 
 * @copyright Copyright 2008 - 2013 Andrew Collette & contributers\n
 * License: BSD 3-clause (see LICENSE.h5py)\n
 *
 * @return An (maybe not at all optimal) guess for chunk size
 */
PSize DataSet::guessChunking(PSize dims, DataType dtype)
{
	// original source:
	//    https://github.com/h5py/h5py/blob/2.1.3/h5py/_hl/filters.py

	if(dims.size() == 0) {
		throw 1;
	}

	PSize chunks(dims);
	const size_t type_size = data_type_to_size(dtype);
	double product = 1;
	std::for_each(dims.begin(), dims.end(), [&](hsize_t &val) {
		//todo: check for +infinity
		if (val == 0)
			val = 1024;

		product *= val;
	});

	product *= type_size;

	double target_size = CHUNK_BASE * pow(2, log10(product/(1024.0L * 1024.0L)));
	if (target_size > CHUNK_MAX)
		target_size = CHUNK_MAX;
	else if (target_size < CHUNK_MIN)
		target_size = CHUNK_MIN;

	size_t i = 0;
	while(true) {

		double csize = psize_product(chunks);
		if (csize == 1.0) {
			break;
		}

		double cbytes = csize * type_size;
		if ((cbytes < target_size || (abs(cbytes - target_size) / target_size) < 0.5)
			&& cbytes < CHUNK_MAX) {
			break;
		}

		//not done yet, one more iteration
		size_t idx = i % chunks.size();
		chunks[idx] = chunks[idx] >> 1; //divide by two
		i++;
	}

	return chunks;
}

void DataSet::extend(const PSize &dims)
{
	//FIXME check for same rank
	h5dset.extend(&dims[0]);
}

Selection DataSet::createSelection() const
{
	H5::DataSpace space = h5dset.getSpace();
	return Selection(space);
}

} //namespace pandora