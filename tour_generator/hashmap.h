#ifndef HASHMAP_H
#define HASHMAP_H


#include <string>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;


template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75)
    {
        if (max_load <= 0)
        {
            m_maxLoadFactor = 0.75;
        }
        else
        {
            m_maxLoadFactor = max_load;
        }
        m_numOfBuckets = 10;
        m_numOfItems = 0;
        m_buckets.resize(m_numOfBuckets);
    }
    
    ~HashMap() {}; // destructor; deletes all of the items in the hashmap
    
    
    int size() const{return m_numOfItems;}
    
    
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T (0 for builtin types).
    // It returns a reference to the newly created value in the map.
    T& operator[](const std::string& key)
    {
        T* temp = find(key);
        if (temp == nullptr)
        {
            insert(key, T());
            return *find(key);
        }
        return(*temp);
    }
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value within the map.
    const T* find(const std::string& key) const
    {
        size_t pos = hash<string>()(key) % m_numOfBuckets;

                if (m_buckets[pos].empty())
                    return nullptr;
                for (const auto& pair : m_buckets[pos])
                {
                    if (pair.m_key == key)
                    {
                        return &pair.m_value;
                    }
                }

        return nullptr;
    }
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    
    T* find(const std::string& key)
    {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
    
    void insert(const std::string& key, const T& value)
    {
        
        size_t pos = hash<string>()(key) % m_numOfBuckets;
        //if key is found then update
       
        for (auto& str : m_buckets[pos])
        {
            if (str.m_key == key)
            {
                str.m_value = value;
                return;
            }
        }
    

        
        // otherwise insert
        
        m_buckets[pos].push_back(Map(key, value));
        m_numOfItems += 1;
        
        
        if (m_numOfItems  > m_maxLoadFactor * m_numOfBuckets)
        {
            m_numOfBuckets *= 2;
            vector<list<Map>> temp = std::move(m_buckets);
            m_buckets.clear();
            m_buckets.resize(m_numOfBuckets);
            for (auto& lizt : temp)
            {
                for (auto& pair : lizt)
                {
                    size_t pos = hash<string>()(pair.m_key) % m_numOfBuckets;
                    m_buckets[pos].push_back(Map(std::move(pair)));
                }
            }
            
        }
        // The insert method associates one item (key) with another (value).
        // If no association currently exists with that key, this method inserts
        // a new association into the hashmap with that key/value pair. If there is
        // already an association with that key in the hashmap, then the item
        // associated with that key is replaced by the second parameter (value).
        // Thus, the hashmap must contain no duplicate keys.
    }
    
    
    
private:
    struct Map {
        string m_key;
        T m_value = T();
        Map(std::string key, T val)
        {
            m_key = key;
            m_value = val;
        }
        
    };
    
    vector<list<Map>> m_buckets;
    int m_numOfBuckets;
    int m_numOfItems;
    double m_maxLoadFactor;
};



#endif // HASHMAP_H
