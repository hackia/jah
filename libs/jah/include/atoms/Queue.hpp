#pragma once

#include "Atom.hpp"
#include <vector>

using namespace std;

namespace Jah {
/**
 * @brief Represents a Queue data structure.
 *
 * Provides functionality for storing and manipulating a collection of Atom
 * objects in a queue, including adding, removing, and accessing elements.
 */
class Queue {
public:
  /**
   * @brief Constructs a new Queue object.
   *
   * Initializes an empty queue.
   */
  explicit Queue();

  void add(const Atom &atom);

  /**
   * @brief Removes a specific element from the queue.
   *
   * This method searches for the specified Atom instance in the queue and
   * removes it if found. The comparison is based on all attributes of the Atom.
   *
   * @param atom The Atom instance to be removed from the queue.
   */
  void del(const Atom &atom);

  /**
   * @brief Removes and retrieves the first element from the queue.
   *
   * This method removes the front element of the queue and returns it.
   * It provides access to the oldest element that was added to the queue.
   *
   * @return The front element of the queue as an Atom instance.
   */
  Atom first();

  /**
   * @brief Retrieves the last element from the queue.
   *
   * This method provides access to the most recently added element in the queue
   * without modifying the queue's content.
   *
   * @return The last element of the queue as an Atom instance.
   */
  Atom last();

private:
  vector<Atom> all;
};
} // namespace Jah
