#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/container/stable_vector.hpp>
#include <functional>

namespace Jah {
template <typename T, typename KeyType>
/**
 * @brief A template class for storing and managing a collection of objects,
 * indexed by a key.
 *
 * The ObjectStore provides efficient storage and lookup of objects. Objects are
 * stored in a stable vector, preserving their insertion order, and indexed
 * using a flat_map for fast retrieval by key. A custom function must be
 * provided to extract the key from the stored objects.
 *
 * @tparam T The type of objects to be stored in the ObjectStore.
 * @tparam KeyType The type of the key used to index the objects.
 */
class ObjectStore {
  /**
   * Constructs an ObjectStore with a function used to determine a unique key
   * for objects of type T.
   *
   * @param getKeyFunc A function that takes a reference to an object of type T
   *                   and returns the corresponding unique key of type KeyType.
   *                   This function is used to generate keys for indexing
   * objects within the store.
   * @return An ObjectStore instance initialized with the specified key
   * generating function.
   */
public:
  explicit ObjectStore(std::function<KeyType(const T &)> getKeyFunc)
      : m_getKey(getKeyFunc) {}

  /**
   * Inserts an object into the storage and updates its index.
   *
   * @param object The object to be inserted into the storage.
   */
  void insert(const T &object) {
    size_t index = m_storage.size();
    m_storage.push_back(object);
    m_index[m_getKey(object)] = index;
  }

  /**
   * Searches for an object in the storage by its key.
   *
   * @param key The key associated with the object to search for.
   * @return A pointer to the object if found; nullptr if the object is not
   * found.
   */
  const T *find(const KeyType &key) {
    auto it = m_index.find(key);
    if (it != m_index.end()) {
      return &m_storage[it->second];
    }
    return nullptr;
  }
private:
  using Storage = boost::container::stable_vector<T>;
  using Index = boost::container::flat_map<KeyType, size_t>;

  /**
   * Internal storage for holding objects of type T in a stable vector. This
   * container ensures that elements maintain their stability in memory even as
   * the vector grows. Used to manage and store objects in the `ObjectStore`
   * class.
   */
  Storage m_storage;
  /**
   * @brief Stores a mapping between keys and indices for the stored objects.
   *
   * This variable acts as an index for a quick lookup of objects in the storage.
   * Keys are generated using the provided key extraction function, and each
   * key maps to the corresponding object's position in the storage container.
   *
   * @note The type of the index is a flat map to ensure efficient retrieval
   *       while maintaining low memory overhead.
   */
  Index m_index;
  /**
   * A callable object used to generate a key of type KeyType from an instance
   * of type T.
   *
   * This member function is expected to be provided during the construction of
   * the ObjectStore class. It is primarily used to determine the key
   * associated with an object, allowing for efficient index-based access in the
   * internal storage.
   *
   * It should be a callable (e.g., a lambda function or a function object) that
   * takes a constant reference to an object of type T and returns a value of
   * type KeyType.
   */
  std::function<KeyType(const T &)> m_getKey;
};
} // namespace Jah