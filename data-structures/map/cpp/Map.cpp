/**
 * @author: Jennifer Chukwu
 */

#include <iostream>

template <typename K, typename V> class Map {

private:
  /**
   * This represents the template for the object that is stored at the
   * underlying array's indexes
   */
  struct Entry {
    K key;
    V value;
    Entry *next;

    Entry(K key, V value) : key(key), value(value), next(nullptr) {}
  };

  Entry **entries;

  // Non-standard hash algorithm ;)
  int getHashForKey(const K &key) {
    const int PRIME = 31;
    return (std::hash<K>{}(key)*PRIME) % 29;
  }

public:
  Map() {
    entries = new Entry *[29](); // Allocate and initialize with nullptrs
  }

  ~Map() {
    // Destructor to clean up memory
    for (int i = 0; i < 29; ++i) {
      Entry *entry = entries[i];
      while (entry) {
        Entry *next = entry->next;
        delete entry;
        entry = next;
      }
    }
    delete[] entries;
  }

  /**
   * Stores the mapping between K key and V value.
   *
   * @param key key with which the specified value is to be associated
   * @param value value to be associated with the specified key
   */
  void put(const K &key, const V &value) {

    // Generate hash
    int hash = getHashForKey(key);
    Entry *&bucket = entries[hash];

    // Store value, if there's no collision
    if (!bucket) {
      bucket = new Entry(key, value);
      return;
    }

    // If there's a collision, insert entry at tail of bucket (linkedlist)
    Entry *entry = bucket;
    while (entry->next && entry->key != key) {
      entry = entry->next;
    }

    if (entry->key == key) {
      entry->value = value;
    } else {
      entry->next = new Entry(key, value);
    }
  }

  /**
   * Returns the value to which the specified K key is mapped.
   *
   * @param key key whose associated value is to be returned
   * @return returns the associated value or null if the key does not exist
   */

  V get(const K &key) {
    // Generate hash
    int hash = getHashForKey(key);
    Entry *entry = entries[hash];

    while (entry) {
      if (entry->key == key) {
        return entry->value;
      }
      entry = entry->next;
    }

    return V(); // Return default-constructed V if not found
  }

  /**
   * Deletes the value to which the specified K key is mapped.
   * Remember that this is a LinkedList, so there are a few edgecases to handle
   *
   * @param key key whose associated value is to be deleted
   */
  void remove(const K &key) {
    // Generate hash
    int hash = getHashForKey(key);
    Entry *&bucket = entries[hash];

    if (!bucket) {
      return;
    }

    Entry *entry = bucket;
    Entry *prev = nullptr;

    while (entry) {
      if (entry->key == key) {
        if (prev) {
          prev->next = entry->next;
        } else {
          bucket = entry->next;
        }
        delete entry;
        return;
      }
      prev = entry;
      entry = entry->next;
    }
  }
};

// Example usage
int main() {
  Map<int, std::string> myMap;
  myMap.put(1, "Ayomide Oyekanmi");
  std::cout << myMap.get(1) << std::endl; // Outputs: Ayomide Oyekanmi
  myMap.remove(1);
  std::cout << (myMap.get(1).empty() ? "Key not found" : myMap.get(1))
            << std::endl; // Outputs: Key not found
  return 0;
}
