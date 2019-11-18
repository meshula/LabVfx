
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_DATASTRIPES_H
#define LABVFX_DATASTRIPES_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace lab { namespace vfx {

class DataStripe
{
public:
    enum Kind { kUInt32_1, kFloat32_1, kFloat32_2, kFloat32_3, kFloat32_4 };
    
    DataStripe(Kind k, size_t c, const char* name);
    ~DataStripe();
    
    void resize(size_t c);
    size_t size() const { return _size; }
    
    template<typename T>
    inline
    T* data(Kind k) const {
        if (sizeof(T) != elementSize())
            return 0;
        if (k != _kind)
            return 0;
        
        return reinterpret_cast<T*>(_data);
    }
    
    template<typename T>
    inline
    T* data() const {
        return reinterpret_cast<T*>(_data);
    }
    
    Kind kind() const { return _kind; }
    Kind elementKind() const { return _elKind; }
    
    int elementSize() const {
        switch (_kind) {
            case kUInt32_1:  return sizeof(uint32_t);
            case kFloat32_1: return sizeof(float);
            case kFloat32_2: return sizeof(float);
            case kFloat32_3: return sizeof(float);
            case kFloat32_4: return sizeof(float);
            default:         return 0;
        }
    }
    
    int stride() const {
        switch (_kind) {
            case kUInt32_1:  return sizeof(uint32_t) * 1;
            case kFloat32_1: return sizeof(float) * 1;
            case kFloat32_2: return sizeof(float) * 2;
            case kFloat32_3: return sizeof(float) * 3;
            case kFloat32_4: return sizeof(float) * 4;
            default:         return 0;
        }
    }
    
    const std::string& name() const { return _name; }
    
private:
    std::string _name;
    
    Kind _kind;
    Kind _elKind;
    size_t _size;
    void* _data;
};

class DataStripes
{
public:
    DataStripes() : _firstFree(0) { }
    ~DataStripes() { }
    
    void expire(const std::vector<int>& indices, int count) {
        auto it = indices.begin();
        for (int i = 0; i < count; ++i) {
            int index = *it;
            // the index array is kept tightly packed, so get the last live particle index
            --_firstFree;
            
            // and swap it with the retiring particle (thus moving the retired slot into the last particle's slot,
            // and moving the last particle on top of the retiring slot
            int temp = _redirect[_firstFree];
            _redirect[_firstFree] = _redirect[index];
            _redirect[index] = temp;
        }
    }
    
    void resize(size_t capacity) {
        if (capacity == _redirect.size())
            return;
        
        _redirect.resize(capacity);
        
        for (auto i : _stripes)
            i.second->resize(capacity);
        
        retireAll();
    }
    
    size_t reserve(size_t x) {
        size_t avail = _redirect.size() - _firstFree;
        if (x <= avail)
            return x;
        return avail;
    }
    
    void addStripe(DataStripe::Kind k, const char* name) {
        std::string s(name);
        _stripes[s] = std::shared_ptr<DataStripe>(new DataStripe(k, _redirect.size(), name));
    }
    
    void removeStripe(const char* name) {
        std::string s(name);
        auto i = _stripes.find(s);
        if (i != _stripes.end())
            _stripes.erase(i);
    }
    
    void retireAll() {
        _firstFree = 0;
        int c = static_cast<int>(_redirect.size());
        for (int i = 0; i < c; ++i)
            _redirect[i] = i;
    }
    
    std::shared_ptr<DataStripe> get(const char* name) {
        std::string s(name);
        auto i = _stripes.find(s);
        if (i != _stripes.end())
            return i->second;
        return std::shared_ptr<DataStripe>();
    }
    
    const std::vector<int>& redirect() const { return _redirect; }
    
    size_t activeCount() const { return _firstFree; }
    void setActiveCount(size_t activeCount) { _firstFree = activeCount; }

private:
    size_t _firstFree;
    std::vector<int> _redirect;
    std::map<std::string, std::shared_ptr<DataStripe>> _stripes;
};

}}

#endif
