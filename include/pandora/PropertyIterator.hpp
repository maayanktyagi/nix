#ifndef PAN_SECTIONITERATOR_H_INCLUDED
#define PAN_SECTIONITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/Property.hpp>

namespace pandora {

class PropertyIterator : public std::iterator<std::input_iterator_tag, Property>
{

protected:

  File  file;
  Group group;
  size_t index, size;

public:
  PropertyIterator(File file, Group group);
  PropertyIterator(const PropertyIterator &other);

  Property operator*() const;
  PropertyIterator &operator++();

  PropertyIterator begin() const;
  PropertyIterator end() const;

  void operator=(const PropertyIterator &other);

  bool operator==(const PropertyIterator &other) const;
  bool operator!=(const PropertyIterator &other) const;

  virtual ~PropertyIterator() {}
};

} /* namespace pandora */
#endif /* PAN_PROPERTY_H_INCLUDED */